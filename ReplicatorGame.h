// ReplicatorGame.h : main header file for the REPLICATORGAME application
//

#if !defined(AFX_REPLICATORGAME_H__0E2ABF46_B5A7_4564_ABAD_71CACA4FEB6E__INCLUDED_)
#define AFX_REPLICATORGAME_H__0E2ABF46_B5A7_4564_ABAD_71CACA4FEB6E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameApp:
// See ReplicatorGame.cpp for the implementation of this class
//

class CReplicatorGameApp : public CWinApp
{
public:
	CReplicatorGameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplicatorGameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CReplicatorGameApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLICATORGAME_H__0E2ABF46_B5A7_4564_ABAD_71CACA4FEB6E__INCLUDED_)
