// GoTestDlg.h : header file
//

#if !defined(AFX_GOTESTDLG_H__F41B9985_19DD_4C01_A739_0C05B3D1789E__INCLUDED_)
#define AFX_GOTESTDLG_H__F41B9985_19DD_4C01_A739_0C05B3D1789E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>   //CInternetConnection，AfxParseURL
#include <string>

using namespace std;

#define SERVER_URL _T("http://127.0.0.1:5170/ocean")

/////////////////////////////////////////////////////////////////////////////
// CGoTestDlg dialog

class CGoTestDlg : public CDialog
{
// Construction
public:
	CGoTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGoTestDlg)
	enum { IDD = IDD_GOTEST_DIALOG };
	CEdit	m_editSendRequest;
	CEdit	m_editServerReturn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGoTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//发送Json数据http请求
	BOOL PostReqAndGetResUtf8(LPCTSTR lpszURL,
		LPCTSTR              lpszObjectName,
		Json::Value          jRoot,
		CString              &csResponse,
		CString              &strError,
		BOOL                 bSecrity,
		INTERNET_PORT        nPort);

	//Convert headers from ASCII to Unicode
	CString AnsiStringToUnicodeString(char* pIn);

	//转换Unicode字符串为UTF-8格式
	CString UnicodeStringToUtf8String(LPCTSTR lpin);

	//转换Unicode字符串为UTF-8格式
	string UnicodeStringToUtf8String(CString lpin,BOOL bIsString);

	//转换Unicode字符串为string
	string UnicodeToAnsi(CString strJson);

	//转换Unicode字符串为Ascii字符串
	void UnicodeStringToAnsiString(LPCTSTR lpin, char* lpOut);

	//判断Json数据的合法性并返回Json数据
	BOOL ParseJsonData(CString strJsondata, Json::Value &root);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTESTDLG_H__F41B9985_19DD_4C01_A739_0C05B3D1789E__INCLUDED_)
