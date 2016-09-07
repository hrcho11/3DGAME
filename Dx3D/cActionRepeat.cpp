#include "StdAfx.h"
#include "cActionRepeat.h"


cActionRepeat::cActionRepeat(void)
	: m_pAction(NULL)
{
}

cActionRepeat::~cActionRepeat(void)
{
	SAFE_RELEASE(m_pAction);
}

void cActionRepeat::Start()
{
	m_pAction->Start();
}

void cActionRepeat::Update()
{
	m_pAction->Update();
}

void cActionRepeat::OnActionFinish( cAction* pSender )
{
	m_pAction->Start();
}
