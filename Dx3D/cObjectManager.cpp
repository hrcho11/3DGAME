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
	assert(m_setObject.empty() && "������ ��ü �� �������� ���� ��ü�� �ֽ��ϴ�.");
}
