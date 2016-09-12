#include "stdafx.h"
#include "cMonsterManager.h"


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

}

void cMonsterManager::Update()
{
	for each(cMonster* p in m_vecMonster)
		p->Update();
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
