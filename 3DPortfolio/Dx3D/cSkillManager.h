#pragma once
#include "cMagicSkill.h"

#define g_pSkillManager	cSkillManager::GetInstance()
#define OBJECT_QUANTITY 8

class cSkillManager
{
private:
	SINGLETONE(cSkillManager);
	cMagicSkill		m_arrMagicAtk[OBJECT_QUANTITY];

public:
	void Init();
	void Update();
	void Destroy();
	void Render();

	void Fire(D3DXVECTOR3& vPos, D3DXVECTOR3& vDir);
};

