#pragma once
#include "cTerrain.h"
#include "cLava.h"

class cMapManager
{
private:
	cTerrain*					m_pTerrain;
	cLava*						m_pLava;
public:
	cMapManager();
	~cMapManager();

	void Init();
	void Update();
	void Destroy();
	void Render(cFrustumCulling* pFrustum);


	cTerrain*			GetTerrain(void) { return m_pTerrain; }
};

