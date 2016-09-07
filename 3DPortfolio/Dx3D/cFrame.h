#pragma once
#include "cMtlTex.h"

class cFrame : public cObject
{
	friend class cAseLoader;

protected:
	D3DXMATRIXA16				m_matLocalTM;
	std::vector<cFrame*>		m_vecChild;
	std::vector<ST_POS_SAMPLE>	m_vecPosTrack;
	std::vector<ST_ROT_SAMPLE>	m_vecRotTrack;
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cFrame(void);
	~cFrame(void);

	void Update(int nKeyFrame, D3DXMATRIXA16* pParent);
	void Render();
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcLocalTM(cFrame* pParent);
	void CalcLocalTranslationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat);
	void CalcLocalRotationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat);
};

