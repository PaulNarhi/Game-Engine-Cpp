#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


enum class BlockType
{
	GRASS = 1,
	DIRT  = 2,
	WOOD  = 3
};

struct Vertex
{
	float x, y, z, u, v;     //x,y,z are pos u, v are texture pos
};


class Block
{
private:
	BlockType m_Type;
public:
	Block();
	~Block();


	BlockType GetType();
};