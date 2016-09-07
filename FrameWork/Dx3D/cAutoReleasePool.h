#pragma once

#define g_pAutoReleasePool cAutoReleasePool::GetInstance()

class cAutoReleasePool
{
	SINGLETONE(cAutoReleasePool);
private:
	std::list<cObject*> m_lstObject;

public:
	void AddObject(cObject* pObject);
	void Drain();
};

