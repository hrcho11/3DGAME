#include "stdafx.h"
#include "cSkillManager.h"

cSkillManager::cSkillManager()
{

}


cSkillManager::~cSkillManager()
{
}
void cSkillManager::Init()
{
	for (int i = 0; i < OBJECT_QUANTITY; i++)
		m_arrMagicAtk[i].Init(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
	
}

void cSkillManager::Update()
{
	for (int i = 0; i < OBJECT_QUANTITY; i++)
		m_arrMagicAtk[i].Update();
}

void cSkillManager::Destroy()
{
	for (int i = 0; i < OBJECT_QUANTITY; i++)
		m_arrMagicAtk[i].Destroy();
}

void cSkillManager::Render()
{
	for (int i = 0; i < OBJECT_QUANTITY; i++)
		m_arrMagicAtk[i].Render();
		
}

void cSkillManager::Fire(D3DXVECTOR3 & vPos, D3DXVECTOR3 & vDir)
{
	for (int i = 0; i < OBJECT_QUANTITY; i++)
	{
		if (m_arrMagicAtk[i].GetIsFire())continue;
		float speed = 5.0f;
		D3DXVECTOR3 pos = vPos + D3DXVECTOR3(0.0f, 0.5f, 0.0f) + (vDir * 1.0f);
		m_arrMagicAtk[i].Init(pos);
		m_arrMagicAtk[i].SetIsFire(true);
		m_arrMagicAtk[i].SetDiretion(vDir);
		break;
	}
}
