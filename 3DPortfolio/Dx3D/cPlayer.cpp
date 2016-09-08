#include "stdafx.h"
#include "cPlayer.h"


cPlayer::cPlayer()
	:m_pSkinnedMesh(nullptr)
{
}


cPlayer::~cPlayer()
{
}

void cPlayer::Init()
{
	m_pSkinnedMesh = new cSkinnedMesh("Data/Character/human/", "human.X");
	m_pSkinnedMesh->SetAnimationIndex(0);

}

void cPlayer::Update()
{
}

void cPlayer::Destroy()
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void cPlayer::Render()
{
	m_pSkinnedMesh->UpdateAndRender();
}
