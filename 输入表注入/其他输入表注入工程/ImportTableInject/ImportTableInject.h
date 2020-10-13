// ImportTableInject.h : main header file for the IMPORTTABLEINJECT application
//

#if !defined(AFX_IMPORTTABLEINJECT_H__C4BE715D_EF23_4772_BB5C_AF20F2CB27CD__INCLUDED_)
#define AFX_IMPORTTABLEINJECT_H__C4BE715D_EF23_4772_BB5C_AF20F2CB27CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImportTableInjectApp:
// See ImportTableInject.cpp for the implementation of this class
//

class CImportTableInjectApp : public CWinApp
{
public:
	CImportTableInjectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportTableInjectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImportTableInjectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTTABLEINJECT_H__C4BE715D_EF23_4772_BB5C_AF20F2CB27CD__INCLUDED_)
