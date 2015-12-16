// GoTest.h : main header file for the GOTEST application
//

#if !defined(AFX_GOTEST_H__1CF375CF_4140_4EEE_B69B_225F02E018E2__INCLUDED_)
#define AFX_GOTEST_H__1CF375CF_4140_4EEE_B69B_225F02E018E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGoTestApp:
// See GoTest.cpp for the implementation of this class
//

class CGoTestApp : public CWinApp
{
public:
	CGoTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGoTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTEST_H__1CF375CF_4140_4EEE_B69B_225F02E018E2__INCLUDED_)
