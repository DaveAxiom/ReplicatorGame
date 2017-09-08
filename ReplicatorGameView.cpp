// ReplicatorGameView.cpp : implementation of the CReplicatorGameView class
//

#include "stdafx.h"
#include "ReplicatorGame.h"

#include "World.h"
#include "ReplicatorGameDoc.h"
#include "ReplicatorGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameView

IMPLEMENT_DYNCREATE(CReplicatorGameView, CView)

BEGIN_MESSAGE_MAP(CReplicatorGameView, CView)
	//{{AFX_MSG_MAP(CReplicatorGameView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameView construction/destruction

CReplicatorGameView::CReplicatorGameView()
{
	m_blocksize = CSize(15, 15);
}

CReplicatorGameView::~CReplicatorGameView()
{
}

BOOL CReplicatorGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameView drawing

void CReplicatorGameView::OnDraw(CDC* pDC)
{
	CReplicatorGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize viewport_size = pDC->GetViewportExt();
	CWnd* dc_window = pDC->GetWindow();
	RECT rect;
	dc_window->GetClientRect(&rect);

	//Text rendering
	CFont thin_font;
	thin_font.CreatePointFont(120, "Courier", pDC);
	pDC->SelectObject(&thin_font);
	

	CString blue_score;
	blue_score.Format("Blue: %d", pDoc->GameScore(1));

	CString red_score;
	red_score.Format("Red (Computer AI): %d", pDoc->GameScore(2));

	CString game_remaining;
	int occupied = pDoc->GameScore(1) + pDoc->GameScore(2);
	game_remaining.Format("Occupied spaces: %d/%d", occupied, 15*15);
	
	int max_text_width = pDC->GetOutputTextExtent(game_remaining).cx;
	int shortest_side = min(rect.right - max_text_width - 10, rect.bottom);
	int segment = shortest_side / 15;
	m_blocksize.cx = segment;
	m_blocksize.cy = segment;

	TEXTMETRIC textmetrics;
	if (pDC->GetTextMetrics(&textmetrics))
	{
		int text_height = textmetrics.tmHeight + textmetrics.tmDescent;
		pDC->TextOut((pDoc->m_play_field->GetWorldSize()).cx * m_blocksize.cx + 10, 0, blue_score);
		pDC->TextOut((pDoc->m_play_field->GetWorldSize()).cx * m_blocksize.cx + 10, text_height, red_score);
		pDC->TextOut((pDoc->m_play_field->GetWorldSize()).cx * m_blocksize.cx + 10, text_height * 2, game_remaining);
	}

	int loop, loop_x;

	if (!pDoc->m_play_field->isWorld())
	{
		return;
	}

	CSize size_of_data = pDoc->m_play_field->GetWorldSize();
	CPoint block_placement;

	CPen blue_blockpen, red_blockpen;
	blue_blockpen.CreatePen(0, 0, RGB(0,0,200));
	red_blockpen.CreatePen(0, 0, RGB(200,0,0));

	CPen* pOldPen = pDC->SelectObject(&blue_blockpen);
	for (loop = 0; loop < size_of_data.cy; loop++)
	{
		for (loop_x = 0; loop_x < size_of_data.cx; loop_x++)
		{
			block_placement = CPoint(loop_x, loop);
			int color = pDoc->m_play_field->ReadWorld(block_placement);

			switch(color)
			{
			case 1:
				pDC->SelectObject(blue_blockpen);
				BlockPlot(block_placement, pDC);
				break;
			case 2:
				pDC->SelectObject(red_blockpen);
				BlockPlot(block_placement, pDC);
			}
		}
	}

	if (pDoc->m_player_selected != CPoint(-1,-1))
	{
		CRect temp(CPoint(pDoc->m_player_selected.x * m_blocksize.cx, pDoc->m_player_selected.y * m_blocksize.cy), m_blocksize);
		pDC->FrameRect(temp, NULL);
	}

	//Clean up from OnDraw
	pDC->SelectObject(pOldPen);
}

void CReplicatorGameView::BlockPlot(CPoint plotting_at, CDC* pDC)
{
	for (int loop = 0; loop < m_blocksize.cy; loop++)
	{
		pDC->MoveTo(plotting_at.x*m_blocksize.cx, loop + plotting_at.y*m_blocksize.cy);
		pDC->LineTo(plotting_at.x*m_blocksize.cx + m_blocksize.cx, loop + plotting_at.y*m_blocksize.cy);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameView printing

BOOL CReplicatorGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CReplicatorGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CReplicatorGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameView diagnostics

#ifdef _DEBUG
void CReplicatorGameView::AssertValid() const
{
	CView::AssertValid();
}

void CReplicatorGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CReplicatorGameDoc* CReplicatorGameView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReplicatorGameDoc)));
	return (CReplicatorGameDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameView message handlers

void CReplicatorGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CReplicatorGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int x_loc, y_loc;

	x_loc = point.x / m_blocksize.cx;
	y_loc = point.y / m_blocksize.cy;

	CSize world_size = pDoc->m_play_field->GetWorldSize();
	if (world_size.cx <= x_loc || world_size.cy <= y_loc)
	{
		//Out of bounds.
		return;
	}

	//Select the square clicked if it's controlled by player.
	if (pDoc->m_player_selected == CPoint(-1,-1) || pDoc->m_play_field->ReadWorld(x_loc, y_loc) == 1)
	{
		if (pDoc->m_play_field->ReadWorld(x_loc, y_loc) == 1)	//Human player is player #1.
		{
			pDoc->m_player_selected = CPoint(x_loc, y_loc);
			Invalidate(false);
		}
		return;
	}

	//Take control of empty square.
	if (pDoc->m_player_selected != CPoint(-1,-1) && pDoc->m_play_field->ReadWorld(x_loc, y_loc) == 0)
	{
		if (pDoc->Rules(1, pDoc->m_player_selected, CPoint(x_loc, y_loc)))
		{
			pDoc->CompleteMove(1, pDoc->m_player_selected, CPoint(x_loc, y_loc));
			pDoc->m_player_selected = CPoint(-1,-1);

			//Run computer's turn after player's turn is complete.
			pDoc->AI();
			Invalidate(true);
		}
		return;
	}

	if (pDoc->m_player_selected != CPoint(-1,-1))
	{
		if (pDoc->Rules(1, pDoc->m_player_selected, CPoint(x_loc, y_loc)))
		{
			Invalidate(false);
			pDoc->m_player_selected = CPoint(-1,-1);
		}
		return;
	}

	if (pDoc->m_player_selected == CPoint(-1,-1))
	{
		pDoc->m_player_selected = CPoint(x_loc, y_loc);

	}
	else
	{
		if (pDoc->Rules(1, pDoc->m_player_selected, CPoint(x_loc, y_loc)) == false)
		{
			return;
		}

		//Copy if neighboring: copy. Move if a 2 spaces away.
		int check_y, check_x;
		switch(pDoc->m_play_field->ReadWorld(x_loc, y_loc))
		{
		case 0:
			//Empty
			for (check_y = max(0, y_loc - 1); check_y < min(y_loc + 1, 15); check_y++)
			{
				for (check_x = max(0, x_loc - 1); check_x < min(y_loc + 1, 15); check_x++)
				{
					if (pDoc->m_play_field->ReadWorld(check_x, check_y) == 2)
					{
						pDoc->m_play_field->WriteWorld(check_x, check_y, 1);
					}
				}
			}

			break;
		case 1:
			pDoc->m_player_selected = CPoint(x_loc, y_loc);
			break;
		case 2:
			//Opponent.
			break;
		}
	}
	Invalidate(false);
}
