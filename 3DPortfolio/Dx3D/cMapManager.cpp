#include "stdafx.h"
#include "cMapManager.h"

cMapManager::cMapManager()
	: m_pTerrain(NULL)
	, m_pLava(NULL)
{
}


cMapManager::~cMapManager()
{

}

void cMapManager::Init()
{
	m_pLava = new cLava();
	m_pLava->Init();

	m_pTerrain = new cTerrain;
	m_pTerrain->Create();

}

void cMapManager::Update()
{
	if (m_pLava)
		m_pLava->Process();

}

void cMapManager::Destroy()
{
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pLava);
}

void cMapManager::Render(cFrustumCulling* pFrustum)
{
	m_pLava->Render();
	m_pTerrain->Draw(pFrustum);

}
