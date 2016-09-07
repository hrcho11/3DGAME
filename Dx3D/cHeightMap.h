#pragma once
#include "iMap.h"

class cHeightMap : public iMap
{
protected:
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

protected:
	D3DMATERIAL9				m_stMtl;
	LPD3DXMESH					m_pMesh;
	std::vector<D3DXVECTOR3>	m_vecVertex;

public:
	cHeightMap(void);
	virtual ~cHeightMap(void);

	virtual void Load(char* szFullPath, D3DXMATRIXA16* pmat) override;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) override;
	virtual void Render() override;
};

