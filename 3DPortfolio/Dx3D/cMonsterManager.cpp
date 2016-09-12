#include "stdafx.h"
#include "cMonsterManager.h"
#include "cPlayer.h"

cMonsterManager::cMonsterManager()
{
	m_vecMonster.reserve(3);
}


cMonsterManager::~cMonsterManager()
{
}

void cMonsterManager::Init()
{
	cMonster* orc = new cMonster;
	orc->Init("Data/Character/orc/", "orc.x");
	cMonster* troll = new cMonster;
	troll->Init("Data/Character/troll/", "troll.x");
	cMonster* undead = new cMonster;
	undead->Init("Data/Character/undead/", "undead.x");

	m_vecMonster.push_back(orc);
	m_vecMonster.push_back(troll);
	m_vecMonster.push_back(undead);

	std::vector<D3DXVECTOR3> vecPos;
	vecPos.push_back(D3DXVECTOR3(-15.0f,0.0f,-15.0f));
	vecPos.push_back(D3DXVECTOR3(15.0f, 0.0f, -15.0f));
	vecPos.push_back(D3DXVECTOR3(15.0f, 0.0f, 15.0f));

	for (size_t i = 0; i < m_vecMonster.size(); ++i)
		m_vecMonster[i]->SetPos(vecPos[i]);
		
}

void cMonsterManager::Update(D3DXVECTOR3& vPos)
{
	for each(cMonster* p in m_vecMonster)
		p->Update(vPos);
}

void cMonsterManager::Destroy()
{
	for each(cMonster* p in m_vecMonster)
	{
		p->Destroy();
		SAFE_RELEASE(p);
	}
		
}

void cMonsterManager::Render()
{
	for each(cMonster* p in m_vecMonster)
		p->Render();
}
