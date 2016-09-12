#include "stdafx.h"
#include "cMonster.h"
#include "cMonsterOwnedStates.h"

cMonster::cMonster()
	:m_pSkinnedMesh(nullptr)
	,m_pStateMachine(nullptr)
{
}

cMonster::~cMonster()
{
}

void cMonster::Init(char* path, char* fileName)
{
	m_pStateMachine = new cStateMachine<cMonster>(this);
	m_pStateMachine->SetCurrentState(cState_Idle::Get());
	LoadModel(path, fileName);
}

void cMonster::Update()
{
}

void cMonster::Destroy()
{
	SAFE_DELETE(m_pStateMachine);
}

void cMonster::Render()
{
	m_pSkinnedMesh->UpdateAndRender();
}

void cMonster::LoadModel(char * path, char * fileName)
{
	m_pSkinnedMesh = new cSkinnedMesh(path, fileName);
}
