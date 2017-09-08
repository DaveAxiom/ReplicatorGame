/*World Class*/
//An all purpose class to hold two dimensional information.

class World
{

// Construction
public:
	World();
	World& operator=(World&);
	World& Copy(World&);

// Attributes
public:
	BOOL isWorld();
	void CreateWorld(int, int);
	SIZE GetWorldSize();
	void RandomWorld();
	void RandomWorld(int);
	void PlotWorld(CDC& dc);

	char ReadWorld(int, int) const;
	char ReadWorld(CPoint) const;

	CPoint AddressToCoordinates(int);
	void WriteWorld(int, int, char) const;
	void WriteWorld(CPoint, char) const;
	void DeleteWorld();
	void ZeroWorld();

private:
	char* m_world;
	void CycleWorld();
	SIZE m_worldsize;


// Operations
public:

// Overrides

// Implementation
public:
	virtual ~World();
};
