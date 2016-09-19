#include "stdafx.h"
#include "cColliderManager.h"


cColliderManager::cColliderManager(void)
{
}


cColliderManager::~cColliderManager(void)
{
}

void cColliderManager::AddCollider(cCollider* p_pCollider)
{
	if (p_pCollider == nullptr) return;

	m_set_pCollider.insert(p_pCollider);

}

void cColliderManager::RemoveCollider(cCollider* p_pCollider)
{
	if (p_pCollider == nullptr) return;

	m_set_pCollider.erase(p_pCollider);
}

void cColliderManager::Destroy()
{
	assert(m_set_pCollider.empty() && "삭제되지 않은 Collider가 있습니다.");
}

void cColliderManager::Update()
{
	for each(auto it in m_set_pCollider)
	{
		it->Update();
	}

	std::set<cCollider*>::iterator iter1;
	std::set<cCollider*>::iterator iter2;
	iter1 = m_set_pCollider.begin();
	for (; iter1 != m_set_pCollider.end(); ++iter1)
	{
		iter2 = iter1;
		++iter2;
		if (iter2 == m_set_pCollider.end()) break;
		for (; iter2 != m_set_pCollider.end(); ++iter2)
		{
			CollideCollidersPhysically(*iter1, *iter2);
		}
	}
}

void cColliderManager::Render()
{
	for each(auto it in m_set_pCollider)
	{
		it->Render();
	}
}
