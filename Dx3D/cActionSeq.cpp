#include "StdAfx.h"
#include "cActionSeq.h"


cActionSeq::cActionSeq(void)
	: m_nCurrIndex(0)
{
}

cActionSeq::~cActionSeq(void)
{
	for each(auto p in m_vecAction)
	{
		SAFE_RELEASE(p);
	}
}

void cActionSeq::AddAction( cAction* pAction )
{
	SAFE_ADDREF(pAction);
	pAction->SetDelegate(this);
	m_vecAction.push_back(pAction);
}

void cActionSeq::Start()
{
	if (m_vecAction.empty())
		return;
	
	m_nCurrIndex = 0;
	m_vecAction[0]->Start();
}

void cActionSeq::Update()
{
	if (m_nCurrIndex < m_vecAction.size())
	{
		m_vecAction[m_nCurrIndex]->Update();
	}
}

void cActionSeq::OnActionFinish( cAction* pSender )
{
	++m_nCurrIndex;
	if (m_nCurrIndex >= m_vecAction.size())
	{
		if(m_pDelegate)
			m_pDelegate->OnActionFinish(this);
	}
	else
	{
		m_vecAction[m_nCurrIndex]->Start();
	}
}
