// CIdNameDlg.cpp: 实现文件
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "CIdNameDlg.h"


// CIdNameDlg 对话框

IMPLEMENT_DYNAMIC(CIdNameDlg, CDialogEx)

CIdNameDlg::CIdNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ID_NAME, pParent)
	, m_name(_T(""))
	, m_id(_T(""))
	, m_mark(_T(""))
{

}

CIdNameDlg::~CIdNameDlg()
{
}

void CIdNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT_ID, m_nid);
	//  DDX_Control(pDX, IDC_EDIT_NAME, m_name);
	//  DDX_Control(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_MARK, m_mark);
}


BEGIN_MESSAGE_MAP(CIdNameDlg, CDialogEx)
END_MESSAGE_MAP()


// CIdNameDlg 消息处理程序


BOOL CIdNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CWnd* pWnd = GetDlgItem(IDC_EDIT_ID);
	CWnd* pWnd1 = GetDlgItem(IDC_EDIT_NAME);
	CWnd* pWnd2 = GetDlgItem(IDC_EDIT_MARK);

	pWnd->EnableWindow(m_bIsIdOpen);

	pWnd1->EnableWindow(m_bIsNameOpen);

	pWnd2->EnableWindow(m_bIsMarkOpen);

	
	CWnd* pWnd3 = GetDlgItem(IDC_STATIC_id);
	CWnd* pWnd4 = GetDlgItem(IDC_STATIC_name);
	CWnd* pWnd5 = GetDlgItem(IDC_STATIC_mark);
	pWnd3->SetWindowTextA(m_cId);
	pWnd4->SetWindowTextA(m_cName);
	pWnd5->SetWindowTextA(m_cmark);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
