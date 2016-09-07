#include "StdAfx.h"
#include "cObjectManager.h"


cObjectManager::cObjectManager(void)
{
}

cObjectManager::~cObjectManager(void)
{
}

void cObjectManager::AddObject( cObject* pObject )
{
	m_setObject.insert(pObject);
}

void cObjectManager::RemoveObject( cObject* pObject )
{
	m_setObject.erase(pObject);
}

void cObjectManager::Destroy()
{
	assert(m_setObject.empty() && "생성된 객체 중 삭제되지 않은 객체가 있습니다.");
}
