#include "StdAfx.h"
#include "cAutoReleasePool.h"


cAutoReleasePool::cAutoReleasePool(void)
{
}


cAutoReleasePool::~cAutoReleasePool(void)
{
}

void cAutoReleasePool::AddObject( cObject* pObject )
{
	m_lstObject.push_back(pObject);
}

void cAutoReleasePool::Drain()
{
	for each(auto p in m_lstObject)
		SAFE_RELEASE(p);
	m_lstObject.clear();
}
