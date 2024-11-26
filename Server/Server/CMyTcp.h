#pragma once
#include"CCrc32.h"
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <vector>
#include <map>
#pragma comment (lib,"ws2_32.lib")
#define MAX_BUF_SIZE   1459  //最大数据长度


//初始化socket
__inline BOOL InitSocket() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return FALSE;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
//反初始化
__inline void UnInitSocket() {
	WSACleanup();
}


enum PkgHeadFlag
{
	//PT_NULL = 0,	 //空返回
	PT_SYN = 0x1,    //请求连接
	PT_ACK = 0x2,    //回答
	PT_PSH = 0x4,    //数据
	PT_FIN = 0x8,    //断开连接
};




#pragma pack(1)
//首部数据包
struct Package {
	Package() {};
	Package(char flag, int nSeq, char* buf = NULL, int nLen = 0) :m_chFlag(flag), m_nSeq(nSeq) {
		if (buf != NULL && nLen > 0 && nLen < MAX_BUF_SIZE) {
			m_nCheck = CCrc32::crc32((unsigned char*)buf, nLen);
			RtlCopyMemory(m_buf, buf, nLen);
			m_nLen = nLen;
		}
	}
	int8_t m_chFlag = 0;             //标志
	int8_t m_nSeq = 0;               //确认
	int8_t m_nCheck = 0;             //检查
	int m_nLen = 0;                  //长度
	char m_buf[MAX_BUF_SIZE] = { 0 };//数据
};
#pragma pack()

//Pkg外一层包 收数据用 附加定时器
struct PackageInfo {
	PackageInfo() {}
	PackageInfo(Package* pPkg) :m_pPkg(pPkg) {
		m_time = GetTickCount();  //记录当前时间
	}
	Package* m_pPkg = NULL;    //收到的数据包
	BOOL m_isSended = FALSE;  //是否发送过
	DWORD m_time = 0;         //定时器
};





class CMyTcp {
public:
	//1.数据
	SOCKET m_sock;                            //Socket
	sockaddr_in m_siSelf = {};                //服务器地址

	sockaddr_in m_siDest = {};                //客户端地址

	int m_nNextSendSeq = 0;                   //下一个发送包的seq
	int m_nNextRecvSeq = 0;                   //下一个接收包的seq
	map<int, PackageInfo>m_mpSend;            //待发送的数据-----定时器：超时重传
	map<int, Package*>m_mpRecv;               //待接收的数据
	vector<char>m_buf;                        //接受数缓冲区

	BOOL m_bIsClose = FALSE;                  //是否关闭
	HANDLE m_hSendThread = NULL;              //发送数据的线程句柄
	HANDLE m_hRecvThread = NULL;              //接受数据的线程句柄
	HANDLE m_hDataThread = NULL;              //处理数据的线程句柄

	CRITICAL_SECTION m_CriSend = {};
	CRITICAL_SECTION m_CriRecv = {};
	CRITICAL_SECTION m_CriData = {};
	//2.基础操作
		//接收连接
	BOOL Accept(const char* szIP, unsigned short nPort) {
		//1.初始化
		InitSocket();
		m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_sock == INVALID_SOCKET) {
			return FALSE;
		}
		m_siSelf.sin_family = AF_INET;
		m_siSelf.sin_port = htons(nPort);
		m_siSelf.sin_addr.S_un.S_addr = inet_addr(szIP);
		int ret = ::bind(m_sock, (sockaddr*)&m_siSelf, sizeof(m_siSelf));
		if (ret == SOCKET_ERROR) {
			Close();
			return FALSE;
		}
		//2.三次连接
			//1.收
		Package pkgRecv;
		int nLen = sizeof(m_siDest);
		ret = recvfrom(m_sock, (char*)&pkgRecv, sizeof(pkgRecv), 0, (sockaddr*)&m_siDest, &nLen);
		if (ret <= 0) {
			Close();
			return FALSE;
		}
		//检查请求连接标志
		if (!pkgRecv.m_chFlag & PT_SYN) {
			Close();
			return FALSE;
		}
		m_nNextRecvSeq = pkgRecv.m_nSeq + 1;


		//2.发
		Package pkgSend(PT_ACK, m_nNextSendSeq);
		ret = sendto(m_sock, (char*)&pkgSend, sizeof(pkgSend), 0, (sockaddr*)&m_siDest, sizeof(m_siDest));
		if (ret <= 0) {
			Close();
			return FALSE;
		}
		m_nNextSendSeq++;


		//3.收
		memset(&pkgRecv, 0, sizeof(pkgRecv));
		nLen = sizeof(m_siDest);
		ret = recvfrom(m_sock, (char*)&pkgRecv, sizeof(pkgRecv), 0, (sockaddr*)&m_siDest, &nLen);
		if (ret <= 0) {
			Close();
			return FALSE;
		} 
		//3.开启线程
		InitializeCriticalSection(&m_CriSend); //初始化一个临界区对象
		InitializeCriticalSection(&m_CriRecv);
		InitializeCriticalSection(&m_CriData);
		//开启发包 收包 数据处理线程
		m_hSendThread = CreateThread(NULL, 0, SendThread, this, 0, NULL);
		m_hRecvThread = CreateThread(NULL, 0, RecvThread, this, 0, NULL);
		m_hDataThread = CreateThread(NULL, 0, DataThread, this, 0, NULL);
		return TRUE;
	}

	//申请连接
	BOOL Connect(const char* szIP, unsigned short nPort) {
		//1.初始化
		InitSocket();
		m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_sock == INVALID_SOCKET) {
			return FALSE;
		}
		m_siDest.sin_family = AF_INET;
		m_siDest.sin_port = htons(nPort);
		m_siDest.sin_addr.S_un.S_addr = inet_addr(szIP);  

		//2.三次握手
			//1.发
		Package pkgSend(PT_SYN, m_nNextSendSeq);
		int ret = sendto(m_sock, (char*)&pkgSend, sizeof(pkgSend), 0, (sockaddr*)&m_siDest, sizeof(m_siDest));
		if (ret <= 0) {
			Close();
			return FALSE;
		}
		m_nNextSendSeq++;


		//2.收
		Package pkgRecv;
		int nLen = sizeof(m_siDest);
		ret = recvfrom(m_sock, (char*)&pkgRecv, sizeof(pkgRecv), 0, (sockaddr*)&m_siDest, &nLen);
		if (ret <= 0) {
			Close();
			return FALSE;
		}
		if (!(pkgRecv.m_chFlag & PT_ACK)) {
			Close();
			return FALSE;
		}
		m_nNextRecvSeq = pkgRecv.m_nSeq + 1;
		//3.发
		Package pkgSend2(PT_SYN, 0);
		ret = sendto(m_sock, (char*)&pkgSend2, sizeof(pkgSend2), 0, (sockaddr*)&m_siDest, sizeof(m_siDest));
		if (ret <= 0) {
			Close();
			return FALSE;
		}


		//3.开启线程
		InitializeCriticalSection(&m_CriSend);
		InitializeCriticalSection(&m_CriRecv);
		InitializeCriticalSection(&m_CriData);
		m_hSendThread = CreateThread(NULL, 0, SendThread, this, 0, NULL);
		m_hRecvThread = CreateThread(NULL, 0, RecvThread, this, 0, NULL);
		m_hDataThread = CreateThread(NULL, 0, DataThread, this, 0, NULL);
		return TRUE;
	}

	//发送数据
	BOOL Send(char* szBuf, int nBufsize) {
		if (szBuf == NULL || nBufsize <= 0) {
			return FALSE;
		}
		//计算包数量
		int nCount = nBufsize / MAX_BUF_SIZE;
		if (nBufsize % MAX_BUF_SIZE != 0) {
			nCount++;
		}
		//数据写入map，等待线程发包
		for (int i = 0; i < nCount; i++) {
			EnterCriticalSection(&m_CriSend);
			Package* pPkgSend = NULL;
			if (i == nCount - 1) {
				pPkgSend = new Package(PT_PSH, m_nNextSendSeq, szBuf + i * MAX_BUF_SIZE, nBufsize - i * MAX_BUF_SIZE);
			}
			else {
				pPkgSend = new Package(PT_PSH, m_nNextSendSeq, szBuf + i * MAX_BUF_SIZE, MAX_BUF_SIZE);
			}
			m_mpSend[m_nNextSendSeq] = PackageInfo(pPkgSend);
			m_nNextSendSeq++;
			LeaveCriticalSection(&m_CriSend);
		}
		return TRUE;
	}
	//接收数据
	int Recv(char* szBuf, int nBufsize) {
		if (szBuf == NULL || nBufsize <= 0) {
			return -1;
		}
		int nLen = 0;
		while (1) {
			if (m_bIsClose == TRUE) {
				nLen = -1;
				break;
			}
			EnterCriticalSection(&m_CriData);
			if (m_buf.size() > 0) {
				nLen = min((int)m_buf.size(), nBufsize);
				memcpy(szBuf, m_buf.data(), nLen);
				vector<char>::iterator itBegin = m_buf.begin();
				m_buf.erase(itBegin, itBegin + nLen);
			}
			LeaveCriticalSection(&m_CriData);
			if (nLen > 0) {
				break;
			}
			Sleep(1);
		}
		return nLen;
	}

	//关闭
	void Close() {
		Package pkgSend(PT_FIN, 0);
		sendto(m_sock, (char*)&pkgSend, sizeof(pkgSend), 0, (sockaddr*)&m_siDest, sizeof(m_siDest));
		Package pkgRecv = {};
		int nLen = sizeof(m_siDest);
		recvfrom(m_sock, (char*)&pkgRecv, sizeof(pkgRecv), 0, (sockaddr*)&m_siDest, &nLen);
		m_bIsClose = TRUE;
		if (m_sock != INVALID_SOCKET) {
			closesocket(m_sock);
			m_sock = INVALID_SOCKET;
		}
		if (m_hDataThread != NULL) {
			WaitForSingleObject(m_hDataThread, -1);
			m_hDataThread = NULL;
		}
		if (m_hRecvThread != NULL) {
			WaitForSingleObject(m_hRecvThread, -1);
			m_hRecvThread = NULL;
		}
		if (m_hSendThread != NULL) {
			WaitForSingleObject(m_hSendThread, -1);
			m_hSendThread = NULL;
		}
		DeleteCriticalSection(&m_CriData);
		DeleteCriticalSection(&m_CriRecv);
		DeleteCriticalSection(&m_CriSend);
		for (auto it : m_mpSend) {
			if (it.second.m_pPkg != NULL) {
				delete it.second.m_pPkg;
			}
		}
		m_mpSend.clear();
		for (auto it : m_mpRecv) {
			if (it.second != NULL) {
				delete it.second;
			}
		}
		m_buf.clear();
		m_bIsClose = FALSE;
		UnInitSocket();
	}


	//3.线程
		//1.发送线程
	static DWORD WINAPI SendThread(LPVOID param) {
		CMyTcp* pMyTCP = (CMyTcp*)param;
		if (pMyTCP == NULL) {
			return 0;
		}
		while (1) {
			if (pMyTCP->m_bIsClose == TRUE) {
				break;
			}
			EnterCriticalSection(&pMyTCP->m_CriSend);
			for (auto& it : pMyTCP->m_mpSend) {
				if (it.second.m_isSended == FALSE || GetTickCount() - it.second.m_time >= 3000) {
					sendto(pMyTCP->m_sock, (char*)it.second.m_pPkg, sizeof(Package), 0, (sockaddr*)&pMyTCP->m_siDest, sizeof(pMyTCP->m_siDest));
					it.second.m_isSended = TRUE;
					it.second.m_time = GetTickCount();
				}
			}
			LeaveCriticalSection(&pMyTCP->m_CriSend);
			Sleep(1);//切线程
		}
		return 0;
	}
	//2.收包线程
	static DWORD WINAPI RecvThread(LPVOID param) {
		CMyTcp* pMyTCP = (CMyTcp*)param;
		if (pMyTCP == NULL) {
			return 0;
		}
		while (1) {
			//tcp关闭
			if (pMyTCP->m_bIsClose == TRUE) {
				break;
			}
			Package* pPkgRecv = new Package();
			int nLen = sizeof(pMyTCP->m_siDest);
			int ret = recvfrom(pMyTCP->m_sock, (char*)pPkgRecv, sizeof(Package), 0, (sockaddr*)&pMyTCP->m_siDest, &nLen);
			if (ret <= 0) {
				pMyTCP->m_bIsClose = TRUE;
				break;
			}
			//回答
			if (pPkgRecv->m_chFlag & PT_ACK) {  
				EnterCriticalSection(&pMyTCP->m_CriSend);
				Package* pPkg = pMyTCP->m_mpSend[pPkgRecv->m_nSeq].m_pPkg;
				if (pPkg != NULL) {
					delete pPkg;
				}
				pMyTCP->m_mpSend.erase(pPkgRecv->m_nSeq);
				delete pPkgRecv;
				pPkgRecv = NULL;
				LeaveCriticalSection(&pMyTCP->m_CriSend);
			}
			//注册
			else if (pPkgRecv->m_chFlag & PT_SYN) {
				pMyTCP->m_nNextRecvSeq = pPkgRecv->m_nSeq + 1;


				//2.发
				Package pkgSend(PT_ACK, pMyTCP->m_nNextSendSeq);
				ret = sendto(pMyTCP->m_sock, (char*)&pkgSend, sizeof(pkgSend), 0, (sockaddr*)&pMyTCP->m_siDest, sizeof(m_siDest));
				if (ret <= 0) {
					break;
				}
				pMyTCP->m_nNextSendSeq++;


				//3.收
				memset(&pPkgRecv, 0, sizeof(pPkgRecv));
				nLen = sizeof(m_siDest);
				ret = recvfrom(pMyTCP->m_sock, (char*)&pPkgRecv, sizeof(pPkgRecv), 0, (sockaddr*)&pMyTCP->m_siDest, &nLen);
				break;

			}
			//数据
			else if (pPkgRecv->m_chFlag & PT_PSH) {
			   //包校验
				if (pPkgRecv->m_nCheck = CCrc32::crc32((unsigned char*)pPkgRecv->m_buf, pPkgRecv->m_nLen)) {
					//回答
					Package pckSend(PT_ACK, pPkgRecv->m_nSeq);
					ret = sendto(pMyTCP->m_sock, (char*)&pckSend, sizeof(pckSend), 0, (sockaddr*)&pMyTCP->m_siDest, sizeof(pMyTCP->m_siDest));
					if (ret <= 0) {
						pMyTCP->m_bIsClose = TRUE;
						break;
					}
					//存放到接收map中
					EnterCriticalSection(&pMyTCP->m_CriRecv);
					pMyTCP->m_mpRecv[pPkgRecv->m_nSeq] = pPkgRecv;
					LeaveCriticalSection(&pMyTCP->m_CriRecv);
				}
			}
			//断开
			else if (pPkgRecv->m_chFlag & PT_FIN) {
				//发送确认
				Package pkgSend(PT_ACK, 0);
				sendto(pMyTCP->m_sock, (char*)&pkgSend, sizeof(pkgSend), 0, (sockaddr*)&pMyTCP->m_siDest, sizeof(pMyTCP->m_siDest));
				pMyTCP->m_bIsClose = TRUE;
				break;
			}
		}
		return 0;
	}
	//3.数据存入缓冲区
	static DWORD WINAPI DataThread(LPVOID param) {
		CMyTcp* pMyTCP = (CMyTcp*)param;
		if (pMyTCP == NULL) {
			return 0;
		}
		while (1) {
			if (pMyTCP->m_bIsClose == TRUE) {
				break;
			}
			EnterCriticalSection(&pMyTCP->m_CriRecv);
			//循环判断是否有数据----按照map的序号放入缓冲区
			if (pMyTCP->m_mpRecv.find(pMyTCP->m_nNextRecvSeq) != pMyTCP->m_mpRecv.end()) {
				//拷贝数据到缓冲区
				EnterCriticalSection(&pMyTCP->m_CriData);
				//拿到map中的pkg包数据-----下一个包序号
				Package* pPkg = pMyTCP->m_mpRecv[pMyTCP->m_nNextRecvSeq];
				if (pPkg != NULL) {
					//获取数据长度
					int nBufLen = pMyTCP->m_buf.size();
					pMyTCP->m_buf.resize(nBufLen + pPkg->m_nLen);
					//数据写到缓冲区
					memcpy(pMyTCP->m_buf.data() + nBufLen, pPkg->m_buf, pPkg->m_nLen);
					delete pPkg;
					pMyTCP->m_mpRecv.erase(pMyTCP->m_nNextRecvSeq);
				}
				LeaveCriticalSection(&pMyTCP->m_CriData);
				pMyTCP->m_nNextRecvSeq++;
			}
			LeaveCriticalSection(&pMyTCP->m_CriRecv);
			Sleep(1);
		}
		return 0;
	}
};


