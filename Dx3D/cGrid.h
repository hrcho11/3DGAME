#pragma once

class cPyramid;

class cGrid
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	std::vector<cPyramid*>		m_vecPyramid;

public:
	cGrid(void);
	~cGrid(void);

	void Setup(int nNumLine = 30, float fInterval = 1.0f);
	void Render();
};

