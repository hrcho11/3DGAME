#include "stdafx.h"
#include "cBaseEntity.h"


int cBaseEntity::m_nNextValidID = 0;

cBaseEntity::cBaseEntity()
	:m_nHealth(100)
{
	m_nID = m_nNextValidID;
	m_nNextValidID += 1;
}


cBaseEntity::~cBaseEntity()
{
}

void cBaseEntity::SetID(int value)
{
	assert((value >= m_nNextValidID) && "<BaseEntity::SetID>:invalid ID");

	m_nID = value;

	m_nNextValidID = value + 1;
}
