#include "StdAfx.h"
#include "cCharactorController.h"
#include "iMap.h"

cCharactorController::cCharactorController(void)
	: m_vPosition(0, 0, 0)
	, m_fRotY(0.0f)
	, m_fSpeed(0.1f)
{
}


cCharactorController::~cCharactorController(void)
{
}

void cCharactorController::Update(iMap* pMap)
{
	D3DXVECTOR3 p = m_vPosition;

	if (GetKeyState('A') & 0x8000)
	{
		m_fRotY -= 0.1f;
	}
	else if (GetKeyState('D') & 0x8000)
	{
		m_fRotY += 0.1f;
	}

	D3DXVECTOR3 vDirection(0, 0, -1);
	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_fRotY);
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matRotY);

	if (GetKeyState('W') & 0x8000)
	{
		p = p + vDirection * m_fSpeed;
	}
	else if (GetKeyState('S') & 0x8000)
	{
		p = p - vDirection * m_fSpeed;
	}

	if (pMap)
	{
		if(pMap->GetHeight(p.x, p.y, p.z))
		{
			m_vPosition = p;
		}
	}

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matRotY * matT;
}

D3DXVECTOR3* cCharactorController::GetPosition()
{
	return &m_vPosition;
}

D3DXMATRIXA16* cCharactorController::GetWorldTM()
{
	return &m_matWorld;
}
