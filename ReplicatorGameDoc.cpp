// ReplicatorGameDoc.cpp : implementation of the CReplicatorGameDoc class
//

#include "stdafx.h"
#include "ReplicatorGame.h"

#include "World.h"
#include "ReplicatorGameDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameDoc

IMPLEMENT_DYNCREATE(CReplicatorGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CReplicatorGameDoc, CDocument)
	//{{AFX_MSG_MAP(CReplicatorGameDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameDoc construction/destruction

CReplicatorGameDoc::CReplicatorGameDoc()
{
	m_play_field = new World;
	m_player_selected = CPoint(-1,-1);
}

CReplicatorGameDoc::~CReplicatorGameDoc()
{
	delete m_play_field;
	m_play_field = NULL;
}

BOOL CReplicatorGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	int dist = 15;
	m_play_field->CreateWorld(dist, dist);
	m_play_field->ZeroWorld();

	//Initial player setup
	m_play_field->WriteWorld(0, 0, 1);
	m_play_field->WriteWorld(dist - 1,dist - 1, 2);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameDoc serialization

void CReplicatorGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameDoc diagnostics

#ifdef _DEBUG
void CReplicatorGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CReplicatorGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReplicatorGameDoc commands

bool CReplicatorGameDoc::Rules(int player, CPoint selected_pos, CPoint target_pos)
{
	if (!m_play_field->isWorld())
	{
		return(false);
	}
	
	CSize world_size = m_play_field->GetWorldSize();
	if (!(world_size.cx > selected_pos.x && world_size.cy > selected_pos.y))
	{
		return(false);
	}

	if (m_play_field->ReadWorld(selected_pos) != player)
	{
		return(false);
	}

	if (m_play_field->ReadWorld(target_pos.x, target_pos.y) != 0)
	{
		return(false);	//Destination is not empty
	}

	//Is it within distance??
	if (abs(target_pos.x - selected_pos.x) < 3 && abs(target_pos.y - selected_pos.y) < 3)
	{
		return(true);
	}

	return(false);
}

void CReplicatorGameDoc::CompleteMove(int player, CPoint selected_pos, CPoint target_pos)
{

	if (m_play_field->ReadWorld(target_pos) == player)
	{
		//Target can't be same as player.
		return;
	}
	int enemy = 0;
	if (player == 1)
	{
		enemy = 2;
	}
	else
	{
		enemy = 1;
	}

	int dist_check_x = abs(target_pos.x - selected_pos.x);
	int dist_check_y = abs(target_pos.y - selected_pos.y);
	if (dist_check_x < 3 && dist_check_y < 3)
	{
		m_play_field->WriteWorld(target_pos, player);	//Place target into array object.
		if (dist_check_x == 2 || dist_check_y == 2)
		{
			m_play_field->WriteWorld(selected_pos, 0);	//Remove selection from array object.
		}
	}
	else
	{
		return;	//Invalid play
	}

	m_play_field->WriteWorld(target_pos, player);	//Place target into array object.

	CSize world_size = m_play_field->GetWorldSize();
	int check_y, check_x;
	for (check_y = max(0, target_pos.y - 1); check_y < min(target_pos.y + 2, world_size.cy); check_y++)
	{
		for (check_x = max(0, target_pos.x - 1); check_x < min(target_pos.x + 2, world_size.cx); check_x++)
		{
			//Capture adjacent material
			if (m_play_field->ReadWorld(check_x, check_y) == enemy)
			{
				m_play_field->WriteWorld(check_x, check_y, player);
			}
		}
	}
}

/*
AI() function doesn't play a perfect game but the structure of the function is 
written out in full to allow those modifications to be made.
*/
void CReplicatorGameDoc::AI()
{
	const int occupied_cell = -1;
	const int computer = 2;

	World decision_world;
	decision_world.Copy((World&)*m_play_field);

	int x, y;
	CSize d_size = decision_world.GetWorldSize();
	for (y = 0; y < d_size.cy; y++)
	{
		for (x = 0; x< d_size.cx; x++)
		{
			if (decision_world.ReadWorld(x, y) != 0)
			{
				decision_world.WriteWorld(x, y, occupied_cell);
			}
		}
	}

	//Check for available moves and surrounding enemies from perspective of cell.
	int search_x, search_y;
	for (y = 0; y < d_size.cy; y++)
	{
		for (x = 0; x < d_size.cx; x++)
		{
			if (decision_world.ReadWorld(x, y) == occupied_cell)
			{
				continue;
			}

			bool far_move = false, close_move = false;
			int enemy = 0;
			for (search_y = max(0, y - 2); search_y < min(y + 3, d_size.cy); search_y++)
			{
				for (search_x = max(0, x - 2); search_x < min(x + 3, d_size.cx); search_x++)
				{
					if (search_x == x && search_y == y)	//Don't count self
					{
						continue;
					}
					if (m_play_field->ReadWorld(search_x, search_y) == 0)	//Avoid empty cells
					{
						continue;
					}

					int dist_check_x = abs(search_x - x);
					int dist_check_y = abs(search_y - y);

					//Eligible cell
					if (m_play_field->ReadWorld(search_x, search_y) == computer)
					{
						if (dist_check_x == 2 || dist_check_y == 2)
						{
							far_move = true;
						}
						else
						{
							close_move = true;
						}
						continue;
					}
					if (dist_check_x != 2 && dist_check_y != 2)
					{
						enemy++;
					}
				}
			}

			int decision_weight = 0;

			if (far_move)
			{
				decision_weight = 1 + enemy;
			}
			if (close_move)
			{
				decision_weight = 3 + enemy;
			}
			if (!close_move && !far_move)
			{
				decision_weight = -1;
			}
			if (decision_world.ReadWorld(x, y) < decision_weight || decision_world.ReadWorld(x, y) == 0)
			{
				decision_world.WriteWorld(x, y, decision_weight);
			}
		}
	}

	//Search for the best decision
	int best_decision = 0;
	CPoint best_location(-1, -1);
	for (y = 0; y < d_size.cy; y++)
	{
		for (x = 0; x < d_size.cx; x++)
		{
			if (decision_world.ReadWorld(x, y) > best_decision)
			{
				best_decision = decision_world.ReadWorld(x, y);
				best_location = CPoint(x, y);
			}
		}
	}
	if (best_location == CPoint(-1,-1))
	{
		TRACE("No moves available to AI.\n");
		return;
	}

	CPoint close_loc(-1,-1), far_loc(-1,-1);

	//Find where to move from to best decision.
	for (search_y = max(0, best_location.y - 2); search_y < min(best_location.y + 3, d_size.cy); search_y++)
	{
		for (search_x = max(0, best_location.x - 2); search_x < min(best_location.x + 3, d_size.cx); search_x++)
		{
			if (search_x == best_location.x && search_y == best_location.y)
			{
				continue;
			}
			if (m_play_field->ReadWorld(search_x, search_y) == 0)
			{
				continue;
			}
			if (m_play_field->ReadWorld(search_x, search_y) == computer)
			{
				int dist_check_x = abs(search_x - best_location.x);
				int dist_check_y = abs(search_y - best_location.y);

				if (dist_check_x == 2 || dist_check_y == 2)
				{
					far_loc = CPoint(search_x, search_y);
				}
				else
				{
					close_loc = CPoint(search_x, search_y);
				}
				continue;
			}
		}
	}

	CPoint final(-1, -1);
	if (close_loc != CPoint(-1,-1))
	{
		final = close_loc;
	}
	else if (far_loc != CPoint(-1,-1))
	{
		final = far_loc;
	}
	else
	{
		TRACE("AI messed up.\n");
		ASSERT(false);
	}

	if (Rules(computer, final, best_location))
	{
		CompleteMove(computer, final, best_location);
	}
	else
	{
		TRACE("AI is cheating\n");
		ASSERT(0);
	}
}

int CReplicatorGameDoc::GameScore(int player)
{
	int x, y, result = 0;
	CSize game_size = m_play_field->GetWorldSize();
	for (y = 0; y < game_size.cy; y++)
	{
		for (x = 0; x < game_size.cx; x++)
		{
			if (m_play_field->ReadWorld(x, y) == player)
			{
				result++;
			}
		}
	}
	return(result);
}
