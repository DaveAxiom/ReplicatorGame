// ReplicatorGameView.h : interface of the CReplicatorGameView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPLICATORGAMEVIEW_H__72010480_2CC6_4B27_B21C_891D20F3AB38__INCLUDED_)
#define AFX_REPLICATORGAMEVIEW_H__72010480_2CC6_4B27_B21C_891D20F3AB38__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CReplicatorGameView : public CView
{
protected: // create from serialization only
	CReplicatorGameView();
	DECLARE_DYNCREATE(CReplicatorGameView)

// Attributes
public:
	CReplicatorGameDoc* GetDocument();

private:
	CSize m_blocksize;
	void BlockPlot(CPoint, CDC*);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplicatorGameView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReplicatorGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CReplicatorGameView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ReplicatorGameView.cpp
inline CReplicatorGameDoc* CReplicatorGameView::GetDocument()
   { return (CReplicatorGameDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLICATORGAMEVIEW_H__72010480_2CC6_4B27_B21C_891D20F3AB38__INCLUDED_)
