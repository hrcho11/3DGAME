#pragma once
class cFrustum
{
private:
	std::vector<D3DXVECTOR3>	m_vecOrigProjVertex;
	std::vector<D3DXPLANE>		m_vecPlane;

public:
	cFrustum(void);
	~cFrustum(void);

	void Update();
	bool IsInFrustum(ST_SPHERE* pSphere);
};

