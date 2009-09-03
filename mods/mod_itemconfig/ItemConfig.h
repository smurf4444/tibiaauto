#if !defined(AFX_ITEMCONFIG_H__2B7D4FF7_3CD1_48AC_9D96_98AFC9020887__INCLUDED_)
#define AFX_ITEMCONFIG_H__2B7D4FF7_3CD1_48AC_9D96_98AFC9020887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemConfig.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CItemAdd dialog

class CItemAdd : public CDialog {
// Construction
public:
	char name[64];
	int itemId;
	CItemAdd();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemAdd)
	enum { IDD = IDD_ITEM_ADD };
	CEdit m_ItemName;
	CEdit m_ItemID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemAdd)
	protected:
	virtual void OnCommit();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemAdd)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////
// CItemEdit dialog

class CItemEdit : public CDialog {
// Construction
public:
	char name[64];
	int itemId;
	CItemEdit(int, char*);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemEdit)
	enum { IDD = IDD_ITEM_EDIT };
	CEdit m_ItemName;
	CEdit m_ItemID;\
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemEdit)
	protected:
	virtual void OnCommit();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemEdit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////
// CFoodAdd dialog

class CFoodAdd : public CDialog
{
// Construction
public:
	char name[64];
	int itemId;
	int eatTime;
	CFoodAdd();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFoodAdd)
	enum { IDD = IDD_FOOD_ADD };
	CEdit m_ItemName;
	CEdit m_ItemID;
	CEdit m_EatTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFoodAdd)
	protected:
	virtual void OnCommit();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFoodAdd)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////
// CFoodEdit dialog

class CFoodEdit : public CDialog
{
// Construction
public:
	char name[64];
	int itemId;
	int eatTime;
	CFoodEdit(int, char*, int);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFoodEdit)
	enum { IDD = IDD_FOOD_EDIT };
	CEdit m_ItemName;
	CEdit m_ItemID;
	CEdit m_EatTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFoodEdit)
	protected:
	virtual void OnCommit();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFoodEdit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////
// CLootAdd dialog

class CLootAdd : public CDialog
{
// Construction
public:
	char name[64];
	int itemId;
	CLootAdd();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLootAdd)
	enum { IDD = IDD_LOOT_ADD };
	CEdit m_ItemName;
	CEdit m_ItemID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLootAdd)
	protected:
	virtual void OnCommit();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLootAdd)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////
// CLootEdit dialog

class CLootEdit : public CDialog
{
// Construction
public:
	char name[64];
	int itemId;
	CLootEdit(int, char*);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLootEdit)
	enum { IDD = IDD_LOOT_EDIT };
	CEdit m_ItemName;
	CEdit m_ItemID;\
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLootEdit)
	protected:
	virtual void OnCommit();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLootEdit)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMCONFIG_H__2B7D4FF7_3CD1_48AC_9D96_98AFC9020887__INCLUDED_)