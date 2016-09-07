#pragma once
#include "cMtlTex.h"

class cGroup : public cObject
{
protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	int							m_nNumTri;
	//std::vector<ST_PNT_VERTEX>	m_vecVertex;
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup(void);
	~cGroup(void);

	void Render();
	void SetVertex(std::vector<ST_PNT_VERTEX>& vecVertex);
};

