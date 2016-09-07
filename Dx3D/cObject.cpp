#include "StdAfx.h"
#include "cObject.h"


cObject::cObject(void)
	: m_nRefCount(1)
{
	g_pObjectManager->AddObject(this);
}

cObject::~cObject(void)
{
	g_pObjectManager->RemoveObject(this);
	assert(m_nRefCount <= 0 && "Release를 이용해서 객체를 해제하세요.");
}

void cObject::AddRef()
{
	++m_nRefCount;
}

void cObject::Release()
{
	--m_nRefCount;
	if (m_nRefCount <= 0)
	{
		delete this;
	}
}

void cObject::AutoRelease()
{
	g_pAutoReleasePool->AddObject(this);
}
