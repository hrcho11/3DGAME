#pragma once

class cCube: public cGameObject
{
private:
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	D3DMATERIAL9				m_stMtl;
	
public:
	cCube(void);
	virtual ~cCube(void);
	
	virtual void Setup(D3DXMATRIXA16* pmat = NULL);
	//virtual void Update();
	virtual void Render(D3DXMATRIXA16* pmat) override;
	virtual void SetMtlColor(D3DXCOLOR c);
};

