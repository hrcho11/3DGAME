#pragma once
#include "cMonster.h"

class Player;

#define MONSTER_QUANTITY 3

class cMonsterManager
{
private:
	std::vector<cMonster*>		m_vecMonster;

	enum
	{
		ORC,
		TROLL,
		UNDEAD
	};


public:
	cMonsterManager();
	~cMonsterManager();

	void Init();
	void Update(D3DXVECTOR3& vPos);
	void Destroy();
	void Render();
};

