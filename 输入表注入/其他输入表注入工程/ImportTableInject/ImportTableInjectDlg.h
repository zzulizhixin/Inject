// ImportTableInjectDlg.h : header file
//

#if !defined(AFX_IMPORTTABLEINJECTDLG_H__5A3421DB_ECF7_426B_B2E7_293C78A4A068__INCLUDED_)
#define AFX_IMPORTTABLEINJECTDLG_H__5A3421DB_ECF7_426B_B2E7_293C78A4A068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImportTableInjectDlg dialog

class CImportTableInjectDlg : public CDialog
{
// Construction
public:
	CImportTableInjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImportTableInjectDlg)
	enum { IDD = IDD_IMPORTTABLEINJECT_DIALOG };
	CString	m_strPEPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportTableInjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImportTableInjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTTABLEINJECTDLG_H__5A3421DB_ECF7_426B_B2E7_293C78A4A068__INCLUDED_)
