#pragma once

class cCube;

class cLinearInterpolation : public cObject
{
private:
	std::vector<ST_PC_VERTEX>	m_vecLineVertex;
	cCube*						m_pCube;

public:
	cLinearInterpolation(void);
	~cLinearInterpolation(void);

	void Setup();
	void Update();
	void Render();
};

