
// ClientDlg.h: 头文件
//

#pragma once
#include "CMyTcp.h"


// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	CMyTcp mytcp;
	CListCtrl m_List;
	afx_msg void OnBnClickedStu();
	afx_msg void OnBnClickedAddCou();
	afx_msg void OnBnClickedAddCla();
	afx_msg void OnBnClickedDelStu();
	afx_msg void OnBnClickedDelCla();
	afx_msg void OnBnClickedCou();
	afx_msg void OnBnClickedStuChaName();
	afx_msg void OnBnClickedCouChaName();
	afx_msg void OnBnClickedClaChaName();
	afx_msg void OnBnClickedAddStuCla();
	afx_msg void OnBnClickedAddStuCou();
	afx_msg void OnBnClickedDelStuCou();
	afx_msg void OnBnClickedAllStu();
	afx_msg void OnBnClickedAllCou();
	afx_msg void OnBnClickedAllCla();

	void ShowAllStu();
	void ShowAllCou();
	void ShowAllCla();
	afx_msg void OnBnClickedFindClaStu();
	afx_msg void OnBnClickedFindStuCou();
	afx_msg void OnBnClickedFindCouStu();
	afx_msg void OnBnClickedSeeIdStu();
	afx_msg void OnBnClickedSeeIdCou();
	afx_msg void OnBnClickedSeeIdCla();
	afx_msg void OnBnClickedStuMarkOne();
	afx_msg void OnBnClickedOrStu();
	afx_msg void OnBnClickedOrCou();
	afx_msg void OnBnClickedOrCla();
	afx_msg void OnBnClickedOrStuCou();
	afx_msg void OnBnClickedButtonStuCou();
	afx_msg void OnBnClickedButtonClaStu();
};
