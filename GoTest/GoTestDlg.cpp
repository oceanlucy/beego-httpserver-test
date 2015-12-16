// GoTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoTest.h"
#include "GoTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGoTestDlg dialog

CGoTestDlg::CGoTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGoTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGoTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGoTestDlg)
	DDX_Control(pDX, IDC_EDIT_SENDREQUEST, m_editSendRequest);
	DDX_Control(pDX, IDC_EDIT_SERVERRETURN, m_editServerReturn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGoTestDlg, CDialog)
	//{{AFX_MSG_MAP(CGoTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoTestDlg message handlers

BOOL CGoTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_editSendRequest.SetWindowText(_T("cmd:sys.ping"));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGoTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGoTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGoTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/************************************************************************/
/* 
描述：发送Json数据http请求
参数：URL，对象名，可选数据，响应数据字符串，是否安全，端口，是否压缩
返回：返回布尔值，判断是否传输成功。
/************************************************************************/
BOOL CGoTestDlg::PostReqAndGetResUtf8(LPCTSTR lpszURL, LPCTSTR lpszObjectName, Json::Value jRoot, CString &csResponse,CString &strError, BOOL bSecrity , INTERNET_PORT nPort)
{
	BOOL bRet = FALSE;
	CString csHeaders, csContentLength, strResponse, strResult, strTemp;
	CString strErr;
	string sJson, sRequest;
	
	CHttpFile* pMyHttpFile = NULL;
	CHttpConnection* pConnection = NULL;
	CInternetSession mySession(0);
	
	csHeaders += _T("Accept-Language: zh-cn,en-us;q=0.5\n");	
	csHeaders += _T("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\n");
	csHeaders += _T("Proxy-Connection: Keep-Alive\n");
	csHeaders += _T("\r\n");

	try
	{
		const int nTimeOut = 20000;
		mySession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);   //联接超时设置
		mySession.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3);          //重试3次   
 		mySession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,nTimeOut*5); 
		mySession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT,nTimeOut*5); 
 
		if (bSecrity)
		{
			pConnection = mySession.GetHttpConnection(lpszURL, INTERNET_FLAG_SECURE,443,NULL,NULL);
		}
		else
		{
			pConnection = mySession.GetHttpConnection(lpszURL, NULL, nPort, NULL, NULL);  //127.0.0.1
		}

		if (bSecrity)
		{
			pMyHttpFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, lpszObjectName,NULL,
													0,NULL,NULL,
													INTERNET_FLAG_RELOAD                    |
													INTERNET_FLAG_NO_CACHE_WRITE            |
													INTERNET_FLAG_SECURE                    |
													INTERNET_FLAG_IGNORE_CERT_CN_INVALID    |
													INTERNET_FLAG_IGNORE_CERT_DATE_INVALID); 
		}
		else
		{
			pMyHttpFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, lpszObjectName, NULL);
		}

		//替换_T("\\")字符
		sJson = jRoot.toStyledString();
		strTemp = sJson.c_str();
		strTemp.Replace(_T("\\"),_T(""));
		sRequest =UnicodeStringToUtf8String(strTemp, TRUE);
		
		//发送消息请求（header+body）
  		bRet=  pMyHttpFile->SendRequest(csHeaders, (DWORD)lstrlen(csHeaders), (void *)sRequest.c_str(), sRequest.length());
		if (bRet)
		{
			DWORD dwStatus = 0.0;
			DWORD dwBuffLen = sizeof(dwStatus);

			//查找文件长度
			long iTotolPos=0;
			strTemp.Empty();
			if(pMyHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,strTemp))	
			{
				iTotolPos = _wtol(strTemp);
				if (iTotolPos == 0)
				{
					strTemp.Format(L"QueryInfo length error!");
					OutputDebugString(strTemp);
					strError = strTemp;
				}
			}
			else
			{
				DWORD dwErrCode = GetLastError();

				strTemp.Format(L"QueryInfo error!error code:\r\n%d", dwErrCode);
				OutputDebugString(strTemp);
				strError = strTemp;
			}
			
			//获取数据内容
			pMyHttpFile->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
			
			if ( HTTP_STATUS_OK == dwStatus )
			{
				char data[2*1024];
				CString  myData;
				
				memset(data,0,2*1024);
				csResponse.Empty();	
					
				char* buffer = new char[iTotolPos+1];
				memset(buffer, 0, iTotolPos+1);
				
				long nReadCount=0,nTotlal=0;
				char* p = buffer;

				while((nReadCount = pMyHttpFile->Read(data, 1024)) > 0)
				{
					nTotlal +=	nReadCount;
					if (nTotlal > iTotolPos) break;
					
					memcpy(p, data, nReadCount);				
					p += nReadCount;
					
					memset(data, 0, 2*1024);
				}
				
				//注意宽字符字节，汉字
				TCHAR* pSrc = NULL;	
				//确定转换为Unicode需要多少长度的缓冲区(返回值也包含了最后一个NULL字符)。
				int i = MultiByteToWideChar(CP_UTF8, 0, (char*)buffer, -1, NULL, 0);  
				
				pSrc = new TCHAR[i+1];
				ZeroMemory(pSrc, (i+1) * sizeof(TCHAR));
				
				//该函数映射一个字符串到一个宽字符（unicode）的字符串。由该函数映射的字符串没必要是多字节字符组。
				int j = MultiByteToWideChar(CP_UTF8, 0, (char*)buffer, -1, pSrc, i);	
				csResponse = pSrc;
				OutputDebugString(csResponse);

				delete pSrc;
				pSrc = NULL;

				delete buffer;
				buffer = NULL;

				pMyHttpFile->Close();
				bRet = TRUE;
			}
		}
	}
	catch ( CInternetException* pEx )
	{
		bRet = FALSE;
		TCHAR  szErr[1024]={0};
		
		pEx->GetErrorMessage(szErr,1024);
		OutputDebugString(szErr);
		strError = szErr;
		pEx->Delete();

		//无法解析服务器的名称或地址:12007,请注意127.0.0.1
	}
	catch(CFileException& fe)
	{
		strErr.Format(L"FileException occur!\r\n%d", fe.m_lOsError);
		OutputDebugString(strErr);
		strError = strErr;
	}
	catch(CMemoryException* e)
	{
		strErr.Format(L"MemoryException occur!\r\n%d", e->ReportError());
		OutputDebugString(strErr);
		strError = strErr;
	}
	catch(CException* e)
	{
		e->GetErrorMessage(strErr.GetBuffer(1024),1);
		strErr.ReleaseBuffer(-1);

		OutputDebugString(strErr);
		strError = strErr;
	}
	catch(...)
	{
		DWORD dwError = GetLastError();

		strErr.Format(L"Unknown Exception occur!\r\n%d", dwError);
		OutputDebugString(strErr);
		strError = strErr;
	}
	
	if ( pMyHttpFile!=NULL )
	{
		pMyHttpFile->Close();
	}
	if ( pConnection!=NULL )
	{
		pConnection->Close();
	}
	mySession.Close();

	return bRet;
}

//*****************************************************
//Convert headers from ASCII to Unicode
//*****************************************************
CString CGoTestDlg::AnsiStringToUnicodeString(char* pIn)
{
	CString strTemp;
	DWORD dwMinSize=0.0;

	dwMinSize = MultiByteToWideChar(CP_ACP, 0, (char*)pIn, -1, NULL, 0);
	WCHAR *pChar = new WCHAR[dwMinSize];
	memset(pChar, 0, sizeof(WCHAR) * (dwMinSize));
	
	MultiByteToWideChar(CP_ACP, 0, (char*)pIn, -1, pChar, dwMinSize); 		
	strTemp.Format(_T("%s"), pChar);

	delete pChar;
	pChar = NULL;
	
	return strTemp;
}

//转换Unicode字符串为Utf8字符串
CString CGoTestDlg::UnicodeStringToUtf8String(LPCTSTR lpin)
{
	DWORD dwNum=0.0;
	CString strOut;

	dwNum = WideCharToMultiByte(CP_UTF8, NULL, (LPCWSTR)lpin, -1, NULL, 0, NULL, FALSE);	
	if (dwNum == 0) return _T("");
	
	char* psText = new char[dwNum];

	memset(psText, 0, dwNum);
	if(psText == NULL)
	{
		delete psText;
		psText = NULL;
		return _T("");
	}	
	WideCharToMultiByte(CP_UTF8, NULL, (LPCWSTR)lpin, -1, psText, dwNum, NULL, NULL);	
	
	strOut = AnsiStringToUnicodeString(psText);
	delete psText;
	psText = NULL;

	return strOut;
}

//转换Unicode字符串为Utf8格式
string CGoTestDlg::UnicodeStringToUtf8String(CString lpin,BOOL bIsString)
{
	DWORD dwNum=0.0;

	dwNum = WideCharToMultiByte(CP_UTF8,NULL,lpin,-1,NULL,0,NULL,FALSE);	
	if (dwNum == 0) return "";
	
	char* psText = new char[dwNum];
	memset(psText,0,dwNum);
	if(psText == NULL)
	{
		delete[] psText;
		psText = NULL;
		return "";
	}	

	WideCharToMultiByte (CP_UTF8,NULL,lpin,-1,psText,dwNum,NULL,NULL);	
	string str = psText;

	delete psText;
	psText = NULL;
	
	return str;
	
}

/************************************************************************/
/*
描述：转换CString字符串为string
参数：用户输入字符串
/************************************************************************/
string CGoTestDlg::UnicodeToAnsi(CString strJson)
{
	int len = 0;  
	string sJsonData;
	
	//对于中文字符，要注意长度
	len = strJson.GetLength()*4;                     
	char* data = new char[len + 1];
	memset(data, 0, len + 1);
	
	UnicodeStringToAnsiString(strJson,data);	
	sJsonData = data;

	delete data;
	data = NULL;
	
	return sJsonData;
}

//转换Unicode字符串为Ascii字符串
void CGoTestDlg::UnicodeStringToAnsiString(LPCTSTR lpin, char* lpOut)
{
	DWORD dwNum=0.0;
	int wLen=0,nLen=0;

	dwNum = WideCharToMultiByte(CP_OEMCP,NULL,lpin,-1,NULL,0,NULL,FALSE);	
	
	char* psText = new char[dwNum];
	memset(psText,0,dwNum);
	if(psText == NULL)
	{
		delete psText;
		psText = NULL;
		return;
	}
	wLen = wcslen((WCHAR *)lpin);
	WideCharToMultiByte(CP_OEMCP,NULL,lpin,-1,psText,dwNum+1,NULL,FALSE);
	nLen = strlen(psText);
	
	ASSERT(nLen <= dwNum);

	memcpy(lpOut,psText,nLen);	

	delete psText;
	psText = NULL;
}

/************************************************************************/
/* 
描述：判断Json数据的合法性并返回Json数据
参数：字符串，返回的Json数据
返回：Json格式的数据
/************************************************************************/
BOOL CGoTestDlg::ParseJsonData(CString strJsondata, Json::Value &root )
{
	Json::Reader reader;
	string strResjson;
	long len=0;

	len	= strJsondata.GetLength();
	if (len <= 0) return NULL;
	
	len *= 5;
	
	// 解析JSON格式
	try
	{
		char* data = new char[len + 1];
		memset(data, 0, len + 1);

		UnicodeStringToAnsiString(strJsondata,data);	
		strResjson = data;
		
		delete data;
		data = NULL;
		
		return reader.parse(strResjson, root, false);
	}
	catch (...)
	{
		return FALSE; 
	}		
}

void CGoTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strResponse, strUrl, strError, strURL;
	CString strUriValue, strErr;
	CString strRequest, strObject;
	CString strTemp,strCmd;
	string sTemp;
	
	Json::Value jRoot,jRetValue;
	INTERNET_PORT wPort = 0;
	DWORD dwType = 0;
	
	BOOL bResult =  AfxParseURL(SERVER_URL, dwType, strURL, strObject, wPort);
	if(!bResult)
	{
		strErr = _T("服务器URL分析失败！");
		return;
	}
	
	//请求信息body
	m_editSendRequest.GetWindowText(strRequest);
	strRequest.Replace(_T("cmd:"), _T(""));
	if (strRequest.IsEmpty())
	{
		strRequest = _T("sys.ping");
	}
	jRoot["cmd"] = UnicodeToAnsi(strRequest);
	jRoot["ver"] = 1;
	
	//发送和接受请求消息
	if (!PostReqAndGetResUtf8(strURL, strObject, jRoot, strResponse,  strErr,FALSE, wPort))
	{
		return;
	}
	
	//打印response
	m_editServerReturn.SetWindowText(strResponse);
	
	//转换为json对象
	if (!ParseJsonData(strResponse,jRetValue)) return;
	if (!jRetValue.isNull())
	{
		strCmd = jRetValue["cmd"].asCString();
	}
}
