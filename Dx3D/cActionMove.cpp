#include "StdAfx.h"
#include "cActionMove.h"


cActionMove::cActionMove(void)
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
{
}

cActionMove::~cActionMove(void)
{
}

void cActionMove::Start()
{
	if(!m_pTarget)
		return;

	m_pTarget->SetPosition(m_vFrom);
	m_fPassedActionTime = 0.0f;
}

void cActionMove::Update()
{
	if(!m_pTarget)
		return;

	m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
	if(m_fPassedActionTime < m_fActionTime)
	{
		float t = m_fPassedActionTime / m_fActionTime;
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
		m_pTarget->SetPosition(p);
	}
	else
	{
		if (m_pDelegate)
		{
			m_pDelegate->OnActionFinish(this);
		}
	}
}
