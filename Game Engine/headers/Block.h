#pragma once


enum class BlockType
{
	GRASS = 1,
	DIRT  = 2,
	WOOD  = 3
};

struct Triangle //this needs to be done so that vertex is x,y,z (vec3d) triangle is ved3d points[3] and mesh is vector<triangle>
{
	float x, y, z;
};

struct mesh
{
	Triangle* Triangles;
};


class Block
{
private:
	BlockType m_Type;
	mesh m_Coords;
public:
	Block();
	~Block();


	BlockType GetType();
	mesh GetCoords();
};