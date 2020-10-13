// ImportTableInjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportTableInject.h"
#include "ImportTableInjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportTableInjectDlg dialog

CImportTableInjectDlg::CImportTableInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportTableInjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportTableInjectDlg)
	m_strPEPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImportTableInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportTableInjectDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strPEPath);
	DDV_MaxChars(pDX, m_strPEPath, 260);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImportTableInjectDlg, CDialog)
	//{{AFX_MSG_MAP(CImportTableInjectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnBtnFix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportTableInjectDlg message handlers

BOOL CImportTableInjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImportTableInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImportTableInjectDlg::OnPaint() 
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
HCURSOR CImportTableInjectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD PEAlign(DWORD dwTarNum,DWORD dwAlignTo)
{	
	return(((dwTarNum+dwAlignTo-1)/dwAlignTo)*dwAlignTo);
}

//
//增加导入表项
//
BOOL AddNewSection(LPCTSTR lpStrModulePath, DWORD dwNewSectionSize)
{
	bool   bSuccess = false;
	LPVOID lpMemModule = NULL;
	LPBYTE lpData = NULL;
	DWORD  dwNewSecFileSize, dwNewSecMemSize;
	HANDLE hFile = INVALID_HANDLE_VALUE, hFileMapping = INVALID_HANDLE_VALUE;
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	PIMAGE_SECTION_HEADER pNewSection = NULL, pLastSection = NULL;

	OutputDebugString("[!] AddNewSection Enter!\n");

	//TODO:可能还涉及关闭windows文件保护
	__try
	{
		//pe文件映射到内存
		hFile = CreateFile(
			     lpStrModulePath,
				 GENERIC_READ | GENERIC_WRITE,
				 FILE_SHARE_READ | FILE_SHARE_WRITE,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL
				 );
		if ( INVALID_HANDLE_VALUE == hFile )
		{
			OutputDebugString("[-] AddNewSection CreateFile fail!\n");
			goto _EXIT_;
		}

		DWORD dwFileSize = GetFileSize(hFile, NULL);
		hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE/* | SEC_IMAGE*/, 0, dwFileSize, "WINSUN_MAPPING_FILE");
		if ( NULL == hFileMapping )
		{
			
			OutputDebugString("[-] AddNewSection CreateFileMapping fail!\n");
			goto _EXIT_;

		}

		lpMemModule = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, dwFileSize);
		if ( NULL == lpMemModule )
		{
			OutputDebugString("[-] AddNewSection MapViewOfFile fail!\n");
			goto _EXIT_;
		}

		lpData = (LPBYTE)lpMemModule;
		//判断是否是PE文件
        if (((PIMAGE_DOS_HEADER)lpData)->e_magic != IMAGE_DOS_SIGNATURE )
        {
			OutputDebugString("[-] AddNewSection PE Header MZ error!\n");
			goto _EXIT_;
        }

		pNtHeader = (PIMAGE_NT_HEADERS)(lpData + ((PIMAGE_DOS_HEADER)(lpData))->e_lfanew);
		if ( pNtHeader->Signature != IMAGE_NT_SIGNATURE )
		{
			OutputDebugString("[-] AddNewSection PE Header PE error!\n");
			goto _EXIT_;
		}
		
		//判断是否可以增加一个新节
		if ( ((pNtHeader->FileHeader.NumberOfSections + 1) * sizeof(IMAGE_SECTION_HEADER)) > (pNtHeader->OptionalHeader.SizeOfHeaders) )
		{
			OutputDebugString("[-] AddNewSection cannot add a new section!\n");
			goto _EXIT_;
		}

        pNewSection  = (PIMAGE_SECTION_HEADER)(pNtHeader+1) + pNtHeader->FileHeader.NumberOfSections;
		pLastSection = pNewSection - 1;

		
		DWORD rsize,vsize,roffset,voffset;
		//对齐偏移和RVA
		rsize=PEAlign(dwNewSectionSize,
				pNtHeader->OptionalHeader.FileAlignment);

		roffset=PEAlign(pLastSection->PointerToRawData+pLastSection->SizeOfRawData,
				pNtHeader->OptionalHeader.FileAlignment);
		
		vsize=PEAlign(dwNewSectionSize,
				pNtHeader->OptionalHeader.SectionAlignment);

		voffset=PEAlign(pLastSection->VirtualAddress+pLastSection->Misc.VirtualSize,
				pNtHeader->OptionalHeader.SectionAlignment);

		//填充新节表
		memcpy(pNewSection->Name, "WINSUN", strlen("WINSUN"));
		pNewSection->VirtualAddress = voffset;
		pNewSection->PointerToRawData = roffset;
		pNewSection->Misc.VirtualSize = vsize;
		pNewSection->SizeOfRawData = rsize;
		pNewSection->Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;

		//修改IMAGE_NT_HEADERS，增加新节表
		pNtHeader->FileHeader.NumberOfSections++;
		pNtHeader->OptionalHeader.SizeOfImage += vsize;
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;

		//增加新节到文件尾部
		DWORD dwWriteBytes;
		SetFilePointer(hFile,0,0,FILE_END);
		PBYTE pbNewSectionContent = new BYTE[rsize];
		ZeroMemory(pbNewSectionContent, rsize);
		bSuccess = WriteFile(hFile, pbNewSectionContent, rsize, &dwWriteBytes, NULL);
		if (!bSuccess)
		{
			MessageBox(NULL,"新增节失败","error",MB_OK);
			goto _EXIT_;
		}
		
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
        OutputDebugString("[-] AddImportTableItem  Exception!\n");
		return false;
	}
  OutputDebugString("[!] AddNewSection Exit!\n");
	bSuccess = true;
_EXIT_:

	if ( hFile )
	{
         CloseHandle(hFile);
	}

	if ( lpMemModule)
	{
		UnmapViewOfFile(lpMemModule);
	}

	if ( hFileMapping )
	{
		CloseHandle(hFileMapping);
	}
	return true;
}

//
PIMAGE_SECTION_HEADER ImageRVA2Section(PIMAGE_NT_HEADERS pImgNTHeader, DWORD dwRVA)
{
	int i;
	PIMAGE_SECTION_HEADER pSectionHeader  = (PIMAGE_SECTION_HEADER)(pImgNTHeader+1);
	for(i=0;i<pImgNTHeader->FileHeader.NumberOfSections;i++)
	{
		if((dwRVA>=(pSectionHeader+i)->VirtualAddress) && (dwRVA<=((pSectionHeader+i)->VirtualAddress+(pSectionHeader+i)->SizeOfRawData)))
		{
			return ((PIMAGE_SECTION_HEADER)(pSectionHeader+i));
		}
	}
	return(NULL);
}


//
// calulates the Offset from a RVA
// Base    - base of the MMF
// dwRVA - the RVA to calculate
// returns 0 if an error occurred else the calculated Offset will be returned
DWORD RVA2Offset(PIMAGE_NT_HEADERS pImgNTHeader, DWORD dwRVA)
{
	DWORD _offset;
	PIMAGE_SECTION_HEADER section;
	section=ImageRVA2Section(pImgNTHeader,dwRVA);//ImageRvaToSection(pimage_nt_headers,Base,dwRVA);
	if(section==NULL)
	{
		return(0);
	}
	_offset=dwRVA+section->PointerToRawData-section->VirtualAddress;
	return(_offset);
}

BOOL AddNewImportDescriptor(const char * szPEFilePath,char * szInjectDllName, char *szImportFuncName)
{
	BOOL bSuccess = FALSE;
	LPVOID lpMemModule = NULL;
	LPBYTE lpData = NULL;
	DWORD  dwNewSecFileSize, dwNewSecMemSize;
	HANDLE hFile = INVALID_HANDLE_VALUE, hFileMapping = INVALID_HANDLE_VALUE;
	PIMAGE_NT_HEADERS pNtHeader = NULL;
	PIMAGE_IMPORT_DESCRIPTOR pstImportTable = NULL;
	PIMAGE_SECTION_HEADER    pstSectionHeader = NULL;
	__try
	{
			//pe文件映射到内存
		hFile = CreateFile(
			     szPEFilePath,
				 GENERIC_READ | GENERIC_WRITE,
				 FILE_SHARE_READ | FILE_SHARE_WRITE,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL
				 );
		if ( INVALID_HANDLE_VALUE == hFile )
		{
			OutputDebugString("[-] AddNewImportDescriptor CreateFile fail!\n");
			goto _EXIT_;
		}

		DWORD dwFileSize = GetFileSize(hFile, NULL);
		hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE/* | SEC_IMAGE*/, 0, dwFileSize, "WINSUN_MAPPING_FILE");
		if ( NULL == hFileMapping )
		{
			
			OutputDebugString("[-] AddNewImportDescriptor CreateFileMapping fail!\n");
			goto _EXIT_;

		}

		lpMemModule = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, dwFileSize);
		if ( NULL == lpMemModule )
		{
			OutputDebugString("[-] AddNewImportDescriptor MapViewOfFile fail!\n");
			goto _EXIT_;
		}

		lpData = (LPBYTE)lpMemModule;
		//判断是否是PE
        if (((PIMAGE_DOS_HEADER)lpData)->e_magic != IMAGE_DOS_SIGNATURE )
        {
			OutputDebugString("[-] AddNewImportDescriptor PE Header MZ error!\n");
			goto _EXIT_;
        }

		pNtHeader = (PIMAGE_NT_HEADERS)(lpData + ((PIMAGE_DOS_HEADER)(lpData))->e_lfanew);
		if ( pNtHeader->Signature != IMAGE_NT_SIGNATURE )
		{
			OutputDebugString("[-] AddNewImportDescriptor PE Header PE error!\n");
			goto _EXIT_;
		}
		pstImportTable = (PIMAGE_IMPORT_DESCRIPTOR)(lpData + RVA2Offset(pNtHeader,pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
		BOOL bBoundImport = FALSE;
		if (pstImportTable->Characteristics == 0 && pstImportTable->FirstThunk != 0)
		{
			bBoundImport = TRUE;
			pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
			pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
		}
		
		pstSectionHeader = (PIMAGE_SECTION_HEADER)(pNtHeader+1)+pNtHeader->FileHeader.NumberOfSections-1;
		PBYTE pbNewSection = pstSectionHeader->PointerToRawData + lpData;
		int i = 0;
		while(pstImportTable->FirstThunk != 0)
		{
			memcpy(pbNewSection, pstImportTable, sizeof(IMAGE_IMPORT_DESCRIPTOR));
			pstImportTable++;
			pbNewSection += sizeof(IMAGE_IMPORT_DESCRIPTOR);
			i++;
		}
		memcpy(pbNewSection, (pbNewSection-sizeof(IMAGE_IMPORT_DESCRIPTOR)), sizeof(IMAGE_IMPORT_DESCRIPTOR));
	

		
		DWORD dwDelt = pstSectionHeader->VirtualAddress - pstSectionHeader->PointerToRawData;
		
		//avoid import not need table
		PIMAGE_THUNK_DATA pImgThunkData = (PIMAGE_THUNK_DATA)(pbNewSection + sizeof(IMAGE_IMPORT_DESCRIPTOR)*2);
		


		//import dll name
		PBYTE pszDllNamePosition = (PBYTE)(pImgThunkData + 2);
		memcpy(pszDllNamePosition, szInjectDllName, strlen(szInjectDllName));
		pszDllNamePosition[strlen(szInjectDllName)] = 0;

		//确定IMAGE_IMPORT_BY_NAM的位置
		PIMAGE_IMPORT_BY_NAME pImgImportByName = (PIMAGE_IMPORT_BY_NAME)(pszDllNamePosition + strlen(szInjectDllName) + 1);
		

		//init IMAGE_THUNK_DATA
		pImgThunkData->u1.Ordinal = dwDelt + (DWORD)pImgImportByName - (DWORD)lpData ;


		//init IMAGE_IMPORT_BY_NAME
		pImgImportByName->Hint = 1;
		memcpy(pImgImportByName->Name, szImportFuncName, strlen(szImportFuncName)); //== dwDelt + (DWORD)pszFuncNamePosition - (DWORD)lpData ;
		pImgImportByName->Name[strlen(szImportFuncName)] = 0;
		//init OriginalFirstThunk
		if (bBoundImport)
		{
			((PIMAGE_IMPORT_DESCRIPTOR)pbNewSection)->OriginalFirstThunk = 0;
		}
		else
			((PIMAGE_IMPORT_DESCRIPTOR)pbNewSection)->OriginalFirstThunk = dwDelt + (DWORD)pImgThunkData - (DWORD)lpData;
		//init FirstThunk
		((PIMAGE_IMPORT_DESCRIPTOR)pbNewSection)->FirstThunk = dwDelt + (DWORD)pImgThunkData - (DWORD)lpData;
		//init Name
		((PIMAGE_IMPORT_DESCRIPTOR)pbNewSection)->Name = dwDelt + (DWORD)pszDllNamePosition-(DWORD)lpData;

		//改变导入表
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress =  pstSectionHeader->VirtualAddress; 
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size			 =  (i+1)*sizeof(IMAGE_IMPORT_DESCRIPTOR);
		
		
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
        OutputDebugString("[-] AddNewImportDescriptor  Exception!\n");
		return false;
	}

	_EXIT_:

	if ( hFile )
	{
         CloseHandle(hFile);
	}

	if ( lpMemModule)
	{
		UnmapViewOfFile(lpMemModule);
	}

	if ( hFileMapping )
	{
		CloseHandle(hFileMapping);
	}
	return true;
}

BOOL AddImportTable(const char * szPEFilePath, char * szInjectDllName,char *szFuncName)
{
	BOOL bSuccess = FALSE;
	try
	{
		//增加一个叫"WINSUN"的节
		bSuccess = AddNewSection(szPEFilePath, 256);
		if (!bSuccess)
		{
			MessageBox(NULL,"add new section fail", "error", MB_OK);
			return bSuccess;
		}
		//增加一个导入表
		AddNewImportDescriptor(szPEFilePath, szInjectDllName,szFuncName);
	}
	catch (CException* e)
	{
		return bSuccess;
	}
	return bSuccess;
}

void BackupPE(char * pszPeFilePath)
{

	   CHAR szPath[MAX_PATH] = {0};
	   PCHAR pszPath = pszPeFilePath;

		pszPath = strrchr(pszPath, '\\');
		*pszPath = '\0';
		strcpy(szPath, pszPeFilePath);
		strcat(szPath, "\\backup_");
		strcat(szPath, pszPath+1);
		*pszPath = '\\';

		CopyFile(pszPeFilePath, szPath, FALSE);
		strncpy(pszPeFilePath, szPath, MAX_PATH);
		return;
}

void CImportTableInjectDlg::OnBtnFix() 
{


#define DLG_FILTER "ALL Files (*.*)|*.*||" 
#define DLG_EXT   "* " 
 	
 	CFileDialog dlg(TRUE, _T(DLG_EXT), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T(DLG_FILTER)); 
 	
 	if (dlg.DoModal() == IDOK) 
 	{ 
 		POSITION pos = dlg.GetStartPosition(); 
 		
 		while (pos != 0) 
 		{ 
 			m_strPEPath = dlg.GetNextPathName(pos); 
 		} 
 		UpdateData(FALSE);
 	} 
	CHAR szPath[MAX_PATH] = {0};
	strcpy(szPath,(LPCTSTR)m_strPEPath);
	BackupPE(szPath);
	AddImportTable((LPCTSTR)m_strPEPath, "WaiGuaDll.dll", "InjectFunc");
	SetDlgItemText(IDC_EDIT2,szPath);
}
