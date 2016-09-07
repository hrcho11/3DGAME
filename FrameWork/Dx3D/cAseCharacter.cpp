#include "StdAfx.h"
#include "cAseCharacter.h"
#include "cFrame.h"
#include "cAseLoader.h"

cAseCharacter::cAseCharacter(void)
	: m_pRoot(NULL)
{
}

cAseCharacter::~cAseCharacter(void)
{
	m_pRoot->Destroy();
}

void cAseCharacter::Load( char* szFullPath )
{
	cAseLoader* pAseLoader = new cAseLoader;
	m_pRoot = pAseLoader->Load(szFullPath, &m_stScene);
	m_pRoot->AddRef();
	SAFE_RELEASE(pAseLoader);
}

void cAseCharacter::Update(D3DXMATRIXA16* pmatWorld)
{
	int nFirstTick = m_stScene.nFirstFrame * m_stScene.nTicksPerFrame;
	int nLastTick = m_stScene.nLastFrame * m_stScene.nTicksPerFrame;
	int nTickSpeed = m_stScene.nFrameSpeed * m_stScene.nTicksPerFrame;
	int nKeyFrame = ((int)((nTickSpeed / 1000.f) * GetTickCount()) % (nLastTick - nFirstTick) + nFirstTick);

	if(m_pRoot)
		m_pRoot->Update(nKeyFrame, pmatWorld);
}

void cAseCharacter::Render()
{
	if(m_pRoot)
		m_pRoot->Render();
}
