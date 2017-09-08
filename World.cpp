/*World Class*/
//An all purpose class to hold two dimensional information.

#include "stdafx.h"
#include "World.h"

//Prototypes:
int random(int);

World::World()
{
	m_world = NULL;
	m_worldsize	= CSize(0,0);
}

World::~World()
{
	DeleteWorld();
}

void World::CreateWorld(int width, int height)
{
	DeleteWorld();
	m_worldsize.cx = width;
	ASSERT(m_worldsize.cx > 0);

	m_worldsize.cy = height;
	ASSERT(m_worldsize.cy > 0);

	ASSERT(0 < m_worldsize.cx * m_worldsize.cy);
	m_world = new char[m_worldsize.cx * m_worldsize.cy];
}


void World::RandomWorld()
{
	int loop;
	ASSERT(isWorld());

	for (loop = 0; loop < m_worldsize.cx * m_worldsize.cy; loop++)
	{
		WriteWorld(AddressToCoordinates(loop), random(255));
	}
}

void World::RandomWorld(int rand_max)
{
	int loop;
	ASSERT(isWorld());

	for (loop = 0; loop < m_worldsize.cx * m_worldsize.cy; loop++)
	{
		WriteWorld(AddressToCoordinates(loop), random(rand_max));
	}
}

int random(int range)
{
	if (range == 0)
	{
		return(0);
	}

	ASSERT(range);
	range = abs(range);	//Avoid dividing by 0!
	ASSERT(range);

	return(rand() % range);
}

void World::WriteWorld(int x, int y, char value) const
{
	CPoint writespot(x,y);

	if (writespot.x < 0)
	{
		writespot.x = m_worldsize.cx - abs(writespot.x);
	}

	if (writespot.y < 0)
	{
		writespot.y = m_worldsize.cy - abs(writespot.y);
	}

	if (writespot.x >= m_worldsize.cx)
	{
		writespot.x = writespot.x % m_worldsize.cx;
	}

	if (writespot.y >= m_worldsize.cy)
	{
		writespot.y = writespot.y % m_worldsize.cy;
	}
	
	WriteWorld(writespot, value);
}

void World::WriteWorld(CPoint point, char value) const
{
	ASSERT(point.x < m_worldsize.cx);
	ASSERT(point.y < m_worldsize.cy);

	int spot;
	spot = (point.y * m_worldsize.cx) + point.x;
	ASSERT(spot < (m_worldsize.cx * m_worldsize.cy));
	ASSERT(spot >= 0);
	
	m_world[spot] = value;	//Do the real write.
}

char World::ReadWorld(int x, int y) const
{
	CPoint readspot(x,y);

	if (readspot.x < 0)
	{
		readspot.x = m_worldsize.cx - abs(readspot.x);
	}

	if (readspot.y < 0)
	{
		readspot.y = m_worldsize.cy - abs(readspot.y);
	}

	if (readspot.x >= m_worldsize.cx)
	{
		readspot.x-=m_worldsize.cx;
	}

	if (readspot.y >= m_worldsize.cy)
	{
		readspot.y-=m_worldsize.cy;
	}

	return(ReadWorld(readspot));
}

char World::ReadWorld(CPoint point) const
{
	ASSERT(point.x < m_worldsize.cx);
	ASSERT(point.x >= 0);
	ASSERT(point.y < m_worldsize.cy);
	ASSERT(point.y >= 0);
	
	int spot;
	spot = (point.y * m_worldsize.cx) + point.x;
	ASSERT(spot < m_worldsize.cx * m_worldsize.cy);
	ASSERT(spot >= 0);

	return(m_world[spot]);	//Do the real read.
}

void World::DeleteWorld()
{
	if (isWorld())
	{
		delete[] m_world;
		m_world = NULL;
		m_worldsize.cx = 0;
		m_worldsize.cy = 0;
	}
}

CPoint World::AddressToCoordinates(int number)
{
	ASSERT(number % m_worldsize.cx <= m_worldsize.cx);
	ASSERT(number/m_worldsize.cx <= m_worldsize.cy);

	return(CPoint(number % m_worldsize.cx, number/m_worldsize.cx));
}

void World::CycleWorld()
{
	//This does the maintenance. It must be customized to the app.
	return;
}

BOOL World::isWorld()	//Is this environment setup?
{
	if (m_world == NULL || m_worldsize.cx == 0 || m_worldsize.cy == 0)
	{
		return((BOOL)false);
	}
	return(true);
}

SIZE World::GetWorldSize()
{
#ifdef _DEBUG
	if (isWorld() == false)
	{
		TRACE("This World object is incomplete.\n");
		ASSERT(isWorld());
	}
#endif

	return(m_worldsize);
}

World& World::operator=(World& in_world)
{
	CSize world_size = in_world.GetWorldSize();
	CreateWorld(world_size.cx, world_size.cy);

	for (int width = 0; width < world_size.cx; width++)
	{
		for (int height = 0; height < world_size.cy; height++)
		{
			WriteWorld(width, height, in_world.ReadWorld(width, height));
		}
	}
	return(*this);
}

World& World::Copy(World& in_world)
{
	CSize world_size = in_world.GetWorldSize();
	CreateWorld(world_size.cx, world_size.cy);

	for (int width = 0; width < world_size.cx; width++)
	{
		for (int height = 0; height < world_size.cy; height++)
		{
			WriteWorld(width, height, in_world.ReadWorld(width, height));
		}
	}
	return(*this);
}

void World::ZeroWorld()
{
	if (!isWorld())
	{
		return;
	}
	memset(m_world, 0, m_worldsize.cx * m_worldsize.cy);
}