#include "StdAfx.h"
#include "cAction.h"

cAction::cAction(void)
	: m_fPassedActionTime(0.0f)
	, m_pTarget(NULL)
	, m_fActionTime(0.0f)
	, m_pDelegate(NULL)
{
}

cAction::~cAction(void)
{
}

void cAction::Start()
{
	m_fPassedActionTime = 0.f;
}

void cAction::Update()
{
}
