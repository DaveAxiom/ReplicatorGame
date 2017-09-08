// ReplicatorGameDoc.h : interface of the CReplicatorGameDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPLICATORGAMEDOC_H__935E1B7B_808B_4D20_902C_CE2DED069299__INCLUDED_)
#define AFX_REPLICATORGAMEDOC_H__935E1B7B_808B_4D20_902C_CE2DED069299__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CReplicatorGameDoc : public CDocument
{
protected: // create from serialization only
	CReplicatorGameDoc();
	DECLARE_DYNCREATE(CReplicatorGameDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplicatorGameDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReplicatorGameDoc();

	World* m_play_field;
	CPoint m_player_selected;
	bool Rules(int, CPoint, CPoint);
	void CompleteMove(int, CPoint, CPoint);
	void AI();
	int GameScore(int);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CReplicatorGameDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLICATORGAMEDOC_H__935E1B7B_808B_4D20_902C_CE2DED069299__INCLUDED_)
