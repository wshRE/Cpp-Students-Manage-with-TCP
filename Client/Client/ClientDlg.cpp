
// ClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CIdNameDlg.h"
#include "DataSend.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedFindCouStu();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_BN_CLICKED(SEE_COU_STU, &CAboutDlg::OnBnClickedFindCouStu)
END_MESSAGE_MAP()


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ADD_STU, &CClientDlg::OnBnClickedStu)
	ON_BN_CLICKED(ADD_COU, &CClientDlg::OnBnClickedAddCou)
	ON_BN_CLICKED(ADD_CLA, &CClientDlg::OnBnClickedAddCla)
	ON_BN_CLICKED(DEL_STU, &CClientDlg::OnBnClickedDelStu)
	ON_BN_CLICKED(DEL_CLA, &CClientDlg::OnBnClickedDelCla)
	ON_BN_CLICKED(DEL_COU, &CClientDlg::OnBnClickedCou)
	ON_BN_CLICKED(CHA_STU_NAME, &CClientDlg::OnBnClickedStuChaName)
	ON_BN_CLICKED(CHA_COU_NAME, &CClientDlg::OnBnClickedCouChaName)
	ON_BN_CLICKED(CHA_CLA_NAME, &CClientDlg::OnBnClickedClaChaName)
	ON_BN_CLICKED(ADD_STU_CLA, &CClientDlg::OnBnClickedAddStuCla)
	ON_BN_CLICKED(ADD_STU_COU, &CClientDlg::OnBnClickedAddStuCou)
	ON_BN_CLICKED(DEL_STU_COU, &CClientDlg::OnBnClickedDelStuCou)
	ON_BN_CLICKED(SEE_STU, &CClientDlg::OnBnClickedAllStu)
	ON_BN_CLICKED(SEE_COU, &CClientDlg::OnBnClickedAllCou)
	ON_BN_CLICKED(SEE_CLA, &CClientDlg::OnBnClickedAllCla)
	ON_BN_CLICKED(SEE_CLA_STU, &CClientDlg::OnBnClickedFindClaStu)
	ON_BN_CLICKED(SEE_STU_COU, &CClientDlg::OnBnClickedFindStuCou)
	ON_BN_CLICKED(SEE_COU_STU, &CClientDlg::OnBnClickedFindCouStu)
	ON_BN_CLICKED(IDC_SEE_ID_STU, &CClientDlg::OnBnClickedSeeIdStu)
	ON_BN_CLICKED(IDC_SEE_ID_COU, &CClientDlg::OnBnClickedSeeIdCou)
	ON_BN_CLICKED(IDC_SEE_ID_CLA, &CClientDlg::OnBnClickedSeeIdCla)
	ON_BN_CLICKED(SEE_STU_MARK_ONE, &CClientDlg::OnBnClickedStuMarkOne)
	ON_BN_CLICKED(SEE_OR_STU, &CClientDlg::OnBnClickedOrStu)
	ON_BN_CLICKED(SEE_OR_COU, &CClientDlg::OnBnClickedOrCou)
	ON_BN_CLICKED(SEE_OR_CLA, &CClientDlg::OnBnClickedOrCla)
	ON_BN_CLICKED(SEE_OR_STU_COU, &CClientDlg::OnBnClickedOrStuCou)
	ON_BN_CLICKED(IDC_BUTTON_STU_COU, &CClientDlg::OnBnClickedButtonStuCou)
	ON_BN_CLICKED(IDC_BUTTON_CLA_STU, &CClientDlg::OnBnClickedButtonClaStu)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	DWORD dwStyle = m_List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）  
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）  
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件  
	m_List.SetExtendedStyle(dwStyle); //设置扩展风格  
	int nColIdx = 0;
	m_List.InsertColumn(nColIdx++, "ID", LVCFMT_CENTER, 200);
	m_List.InsertColumn(nColIdx++, "名字", LVCFMT_CENTER, 200);
	m_List.InsertColumn(nColIdx++, "成绩（如果有）", LVCFMT_CENTER, 200);


	//初始化
	int n = mytcp.Connect("127.0.0.1", 0x9527);
	if (n == -1) {
		return FALSE;
	}




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//增加学生
void CClientDlg::OnBnClickedStu()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cName;

	CIdNameDlg dlg;
	dlg.m_bIsIdOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cName = dlg.m_name;
	}
	else {
		return;
	}
	//发送
	char* str = cName.GetBuffer(cName.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_ADD_STU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;

}


//添加课程
void CClientDlg::OnBnClickedAddCou()
{
	CString cName;

	CIdNameDlg dlg;
	dlg.m_bIsIdOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cName = dlg.m_name;
	}
	else {
		return;
	}
	//发送
	char* str = cName.GetBuffer(cName.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_ADD_COU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;
}


//添加班级
void CClientDlg::OnBnClickedAddCla()
{
	CString cName;

	CIdNameDlg dlg;
	dlg.m_bIsIdOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cName = dlg.m_name;
	}
	else {
		return;
	}
	//发送
	char* str = cName.GetBuffer(cName.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_ADD_CLA;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;
}



//删除学生
void CClientDlg::OnBnClickedDelStu()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_DEL_STU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;
}



//删除班级
void CClientDlg::OnBnClickedDelCla()
{
	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_DEL_CLA;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;
}

//删除课程
void CClientDlg::OnBnClickedCou()
{
	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_DEL_COU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;
}



//修改名字
void CClientDlg::OnBnClickedStuChaName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cid;
	CString cname;

	CIdNameDlg dlg;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
		cname = dlg.m_name;
	}
	else {
		return;
	}
	char* szName = cname.GetBuffer(cname.GetLength());
	int nid = atoi(cid);
	DataHead dh;
	dh.m_Opt = TASK_CHA_STU_NAME;
	dh.m_nLen = sizeof(nid) + strlen(szName) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&nid, sizeof(nid));
	memcpy(ss + sizeof(DataHead)+ sizeof(nid), szName, strlen(szName));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);   
}


//修改课程
void CClientDlg::OnBnClickedCouChaName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cid;
	CString cname;

	CIdNameDlg dlg;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
		cname = dlg.m_name;
	}
	else {
		return;
	}
	char* szName = cname.GetBuffer(cname.GetLength());
	int nid = atoi(cid);
	DataHead dh;
	dh.m_Opt = TASK_CHA_COU_NAME;
	dh.m_nLen = sizeof(nid) + strlen(szName) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&nid, sizeof(nid));
	memcpy(ss + sizeof(DataHead) + sizeof(nid), szName, strlen(szName));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
}

//修改班级名字
void CClientDlg::OnBnClickedClaChaName()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cid;
	CString cname;

	CIdNameDlg dlg;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid  = dlg.m_id;
		cname = dlg.m_name;
	}
	else {
		return;
	}
	char* szName = cname.GetBuffer(cname.GetLength());
	int nid = atoi(cid);
	DataHead dh;
	dh.m_Opt = TASK_CHA_CLA_NAME;
	dh.m_nLen = sizeof(nid) + strlen(szName) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&nid, sizeof(nid));
	memcpy(ss + sizeof(DataHead) + sizeof(nid), szName, strlen(szName));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
}



//添加学生班级
void CClientDlg::OnBnClickedAddStuCla()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cOne;
	CString cTwo;

	CIdNameDlg dlg;
	dlg.m_cId = "班级id";
	dlg.m_cName = "学生id";
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
	}
	else {
		return;
	}
	IdTwo it;
	it.m_nOne = atoi(cOne); //班级
	it.m_nTwo = atoi(cTwo); //学生
	DataHead dh;
	dh.m_Opt = TASK_ADD_STU_CLA;
	dh.m_nLen = sizeof(it);
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&it, sizeof(it));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
}

//添加学生课程
void CClientDlg::OnBnClickedAddStuCou()
{
	CString cOne;//课程id
	CString cTwo;//学生id
	CString cThree;//成绩

	CIdNameDlg dlg;
	dlg.m_cId = "课程id";
	dlg.m_cName = "学生id";
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
		cThree = dlg.m_mark;
	}
	else {
		return;
	}
	IdThree it;
	it.m_nOne = atoi(cOne);
	it.m_nTwo = atoi(cTwo);
	it.m_nThree = atoi(cThree);
	DataHead dh;
	dh.m_Opt = TASK_ADD_STU_COU;
	dh.m_nLen = sizeof(it);
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&it, sizeof(it));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
}

//删除学生课程
void CClientDlg::OnBnClickedDelStuCou()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cOne; //课程id
	CString cTwo; //学生id

	CIdNameDlg dlg;
	dlg.m_cId = "课程id";
	dlg.m_cName = "学生id";
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
	}
	else {
		return;
	}

	IdTwo it;
	it.m_nOne = atoi(cOne);
	it.m_nTwo = atoi(cTwo);
	DataHead dh;
	dh.m_Opt = TASK_DEL_STU_COU;
	dh.m_nLen = sizeof(it);
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&it, sizeof(it));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
}



//显示所有学生
void CClientDlg::OnBnClickedAllStu()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowAllStu();
}

//显示所有课程
void CClientDlg::OnBnClickedAllCou()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowAllCou();
}


//显示所有班级
void CClientDlg::OnBnClickedAllCla()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowAllCla();
}

void CClientDlg::ShowAllStu()
{
	m_List.DeleteAllItems();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("学生表");

	DataHead dh;
	dh.m_Opt = TASK_SEE_STU;
	dh.m_nLen = 0;
	char* ss = new char[sizeof(DataHead)];
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	//发送
	mytcp.Send(ss, sizeof(DataHead));
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}

void CClientDlg::ShowAllCou()
{
	m_List.DeleteAllItems();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("课程表");

	DataHead dh;
	dh.m_Opt = TASK_SEE_COU;
	dh.m_nLen = 0;
	char* ss = new char[sizeof(DataHead)];
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	//发送
	mytcp.Send(ss, sizeof(DataHead));
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);
	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}

void CClientDlg::ShowAllCla()
{
	m_List.DeleteAllItems();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("班级表");

	DataHead dh;
	dh.m_Opt = TASK_SEE_CLA;
	dh.m_nLen = 0;
	char* ss = new char[sizeof(DataHead)];
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	//发送
	mytcp.Send(ss, sizeof(DataHead));
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}



//查找班级的学生
void CClientDlg::OnBnClickedFindClaStu()
{
	// TODO: 在此添加控件通知处理程序代码
	m_List.DeleteAllItems();

	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}

	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_CLA_STU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	    //读取个数
	int nCount = 0;
	memcpy((char*)&nCount, szTemp,sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount;i++) {
	    memcpy((char*)&nOne, szTemp, sizeof(int));
	    szTemp += sizeof(int);
	    memcpy((char*)&nSize, szTemp, sizeof(int));
	    szTemp += sizeof(int);
	    ZeroMemory(szA, 1459);
	    memcpy(szA, szTemp, nSize);
	    szTemp += nSize + 1;
		//显示
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}


//查找学生的课程
void CClientDlg::OnBnClickedFindStuCou()
{
	// TODO: 在此添加控件通知处理程序代码
	m_List.DeleteAllItems();
	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}



	char *str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_STU_COU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount = 0;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	int nMark; //成绩
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
	    memcpy((char*)&nOne, szTemp, sizeof(int));
	    szTemp += sizeof(int);
	    memcpy((char*)&nSize, szTemp, sizeof(int));
	    szTemp += sizeof(int);
	    memcpy((char*)&nMark, szTemp, sizeof(int));
	    szTemp += sizeof(int);
	    ZeroMemory(szA, 1459);
	    memcpy(szA, szTemp, nSize);
	    szTemp += nSize + 1;
		//显示
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
		cOne.Format(("%d"), nMark);
		m_List.SetItemText(nNextRow, 2, cOne); //名字插入
	}
}


//查找课程的学生
void CClientDlg::OnBnClickedFindCouStu()
{
	// TODO: 在此添加控件通知处理程序代码
	m_List.DeleteAllItems();

	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}

	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_COU_STU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount = 0;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	int nMark; //成绩
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nMark, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//显示
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
		cOne.Format(("%d"), nMark);
		m_List.SetItemText(nNextRow, 2, cOne); //名字插入
	}
}





//id查找学生
void CClientDlg::OnBnClickedSeeIdStu()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("学生查询结果");
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_ID_STU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}


//id查找课程
void CClientDlg::OnBnClickedSeeIdCou()
{
	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("课程查询结果");
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_ID_COU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);
	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}


}

//id查找班级
void CClientDlg::OnBnClickedSeeIdCla()
{
	// TODO: 在此添加控件通知处理程序代码

	CString cid;

	CIdNameDlg dlg;
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("班级查询结果");
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_ID_CLA;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}


}





//查看课程 id xx 成绩在 xx 到 xx的学生id
void CClientDlg::OnBnClickedStuMarkOne()
{
	CString cOne;//课程id
	CString cTwo;//分数左
	CString cThree;//分数右

	CIdNameDlg dlg;
	dlg.m_cId = "课程id";
	dlg.m_cName = "分数左边界";
	dlg.m_cmark = "分数右边界";
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
		cThree = dlg.m_mark;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	IdThree it;
	it.m_nOne = atoi(cOne);
	it.m_nTwo = atoi(cTwo);
	it.m_nThree = atoi(cThree);
	DataHead dh;
	dh.m_Opt = TASK_SEE_STU_MARK_ONE;
	dh.m_nLen = sizeof(it);
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&it, sizeof(it));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);



	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount = 0;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	int nMark; //成绩
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nMark, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//显示
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
		cOne.Format(("%d"), nMark);
		m_List.SetItemText(nNextRow, 2, cOne); //名字插入
	}
}


//学生： id范围 为xx-xx 或者 名字为 xx
void CClientDlg::OnBnClickedOrStu()
{
	CString cOne;//学生名
	CString cTwo;//id左
	CString cThree;//id右

	CIdNameDlg dlg;
	dlg.m_cId = "学生名";
	dlg.m_cName = "id左边界";
	dlg.m_cmark = "id右边界";
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
		cThree = dlg.m_mark;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	char* szName = cOne.GetBuffer(cOne.GetLength());
	int nOnen = atoi(cTwo);
	int nTwo = atoi(cThree);
	DataHead dh;
	dh.m_Opt = TASK_SEE_OR_STU;
	dh.m_nLen = sizeof(int)*2 + strlen(szName) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&nOnen, sizeof(nOnen));
	memcpy(ss + sizeof(DataHead) + sizeof(int), (char*)&nTwo, sizeof(nTwo));
	memcpy(ss + sizeof(DataHead) + sizeof(int)*2, szName, strlen(szName));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}


//课程： id范围 为xx-xx 或者 名字为 xx
void CClientDlg::OnBnClickedOrCou()
{
	CString cOne;//学生名
	CString cTwo;//id左
	CString cThree;//id右

	CIdNameDlg dlg;
	dlg.m_cId = "课程名";
	dlg.m_cName = "id左边界";
	dlg.m_cmark = "id右边界";
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
		cThree = dlg.m_mark;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	char* szName = cOne.GetBuffer(cOne.GetLength());
	int nOnen = atoi(cTwo);
	int nTwo = atoi(cThree);
	DataHead dh;
	dh.m_Opt = TASK_SEE_OR_COU;
	dh.m_nLen = sizeof(int) * 2 + strlen(szName) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&nOnen, sizeof(nOnen));
	memcpy(ss + sizeof(DataHead) + sizeof(int), (char*)&nTwo, sizeof(nTwo));
	memcpy(ss + sizeof(DataHead) + sizeof(int) * 2, szName, strlen(szName));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}

//班级： id范围 为xx-xx 或者 名字为 xx
void CClientDlg::OnBnClickedOrCla()
{
	CString cOne;//学生名
	CString cTwo;//id左
	CString cThree;//id右

	CIdNameDlg dlg;
	dlg.m_cId = "班级名";
	dlg.m_cName = "id左边界";
	dlg.m_cmark = "id右边界";
	if (dlg.DoModal() == IDOK) {
		cOne = dlg.m_id;
		cTwo = dlg.m_name;
		cThree = dlg.m_mark;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	char* szName = cOne.GetBuffer(cOne.GetLength());
	int nOnen = atoi(cTwo);
	int nTwo = atoi(cThree);
	DataHead dh;
	dh.m_Opt = TASK_SEE_OR_CLA;
	dh.m_nLen = sizeof(int) * 2 + strlen(szName) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), (char*)&nOnen, sizeof(nOnen));
	memcpy(ss + sizeof(DataHead) + sizeof(int), (char*)&nTwo, sizeof(nTwo));
	memcpy(ss + sizeof(DataHead) + sizeof(int) * 2, szName, strlen(szName));
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}


//每门课都在范围内的同学
//TASK_SEE_OR_STU_COU
void CClientDlg::OnBnClickedOrStuCou()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cid;

	CIdNameDlg dlg;
	dlg.m_cId = "小于某个分数";
	dlg.m_bIsNameOpen = FALSE;
	dlg.m_bIsMarkOpen = FALSE;
	if (dlg.DoModal() == IDOK) {
		cid = dlg.m_id;
	}
	else {
		return;
	}
	m_List.DeleteAllItems();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("学生查询结果");
	//发送
	char* str = cid.GetBuffer(cid.GetLength());
	DataHead dh;
	dh.m_Opt = TASK_SEE_OR_STU_COU;
	dh.m_nLen = strlen(str) + 1;
	char* ss = new char[sizeof(DataHead) + dh.m_nLen];
	ZeroMemory(ss, sizeof(DataHead) + dh.m_nLen);
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	memcpy(ss + sizeof(DataHead), str, strlen(str));
	//发送
	mytcp.Send(ss, sizeof(DataHead) + dh.m_nLen);

	delete[] ss;


	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nSize; //第二个int 第三个char的大小
	char szA[1459]; //第三个
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {
		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nSize, szTemp, sizeof(int));
		szTemp += sizeof(int);
		ZeroMemory(szA, 1459);
		memcpy(szA, szTemp, nSize);
		szTemp += nSize + 1;
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		m_List.SetItemText(nNextRow, 1, szA); //名字插入
	}
}





//所有学生课程表
void CClientDlg::OnBnClickedButtonStuCou()
{
	m_List.DeleteAllItems();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("学生-课程表");

	DataHead dh;
	dh.m_Opt = TASK_SEE_STU_AND_COU;
	dh.m_nLen = 0;
	char* ss = new char[sizeof(DataHead)];
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	//发送
	mytcp.Send(ss, sizeof(DataHead));
	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nTwo;  //第二个int
	int nThree;  //第三个int
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {

		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nTwo, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nThree, szTemp, sizeof(int));
		szTemp += sizeof(int);
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		cOne.Format(("%d"), nTwo);
		m_List.SetItemText(nNextRow, 1, cOne); //名字插入
		cOne.Format(("%d"), nThree);
		m_List.SetItemText(nNextRow, 2, cOne); //名字插入
	}
}

//所有班级学生表
void CClientDlg::OnBnClickedButtonClaStu()
{
	m_List.DeleteAllItems();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LIST);
	pWnd->SetWindowTextA("班级-学生表");

	DataHead dh;
	dh.m_Opt = TASK_SEE_CLA_AND_STU;
	dh.m_nLen = 0;
	char* ss = new char[sizeof(DataHead)];
	memcpy(ss, (char*)&dh, sizeof(DataHead));
	//发送
	mytcp.Send(ss, sizeof(DataHead));



	//收包
	mytcp.Recv((char*)&dh, sizeof(dh));
	char* szBuff = new char[dh.m_nLen];
	mytcp.Recv(szBuff, dh.m_nLen);
	char* szTemp = szBuff;
	//读取个数
	int nCount;
	memcpy((char*)&nCount, szTemp, sizeof(int));
	szTemp += sizeof(int);

	int nOne;  //第一个int
	int nTwo;  //第二个int
	int nNextRow = 0; //列表行索引
	for (int i = 0; i < nCount; i++) {

		memcpy((char*)&nOne, szTemp, sizeof(int));
		szTemp += sizeof(int);
		memcpy((char*)&nTwo, szTemp, sizeof(int));
		szTemp += sizeof(int);
		//插入
		nNextRow = m_List.GetItemCount();// 下一行的索引
		CString cOne;
		cOne.Format(("%d"), nOne);
		m_List.InsertItem(nNextRow, cOne); //id插入
		cOne.Format(("%d"), nTwo);
		m_List.SetItemText(nNextRow, 1, cOne); //名字插入
	}
}
