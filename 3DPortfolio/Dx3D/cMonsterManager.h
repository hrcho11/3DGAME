#pragma once
#include "cMonster.h"
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
	void Update();
	void Destroy();
	void Render();
};

