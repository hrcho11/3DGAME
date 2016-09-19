#pragma once

#define g_pColliderManager cColliderManager::GetInstance()

class cCollider;
class cColliderManager
{
	SINGLETONE(cColliderManager);
private:
	std::set<cCollider*> m_set_pCollider;
public:
	void AddCollider(cCollider* p_pCollider);
	void RemoveCollider(cCollider* p_pCollider);
	void Destroy();
	void Update();
	void Render();
};