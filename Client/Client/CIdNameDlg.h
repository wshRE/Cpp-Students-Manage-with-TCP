#pragma once
#include "afxdialogex.h"


// CIdNameDlg 对话框

class CIdNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIdNameDlg)

public:
	CIdNameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CIdNameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ID_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	BOOL m_bIsIdOpen = TRUE;
	BOOL m_bIsNameOpen = TRUE;
	BOOL m_bIsMarkOpen = TRUE;

	CString m_cId = "id";
	CString m_cName = "名字";
	CString m_cmark = "成绩";


	virtual BOOL OnInitDialog();
	CString m_name;
	CString m_id;
	CString m_mark;
};
