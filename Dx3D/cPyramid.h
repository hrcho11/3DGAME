#pragma once
class cPyramid
{
private:
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	D3DXMATRIXA16				m_matWorld;
	D3DMATERIAL9				m_stMtl;

public:
	cPyramid(void);
	~cPyramid(void);

	void Setup(D3DXCOLOR c, D3DXMATRIXA16& matWorld);
	void Render();
};

