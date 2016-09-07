#include "StdAfx.h"
#include "cGameObject.h"
#include "cAction.h"

cGameObject::cGameObject(void)
	: m_vDirection(0, 0, 0)
	, m_vPosition(0, 0, 0)
	, m_pAction(NULL)
	, m_vPrevPosition(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cGameObject::~cGameObject(void)
{
	SAFE_RELEASE(m_pAction);
}


void cGameObject::Update()
{
	if (m_pAction)
	{
		m_pAction->Update();
	}

	D3DXMATRIXA16 matT, matR;

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	D3DXVECTOR3 d = m_vPosition - m_vPrevPosition;
	if(D3DXVec3LengthSq(&d) > D3DX_16F_EPSILON)
	{
		m_vDirection = d;
	}

	D3DXMatrixLookAtLH(&matR, 
		&D3DXVECTOR3(0, 0, 0),
		&m_vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);

	m_matWorld = matR * matT;

	m_vPrevPosition = m_vPosition;
}

void cGameObject::Render(D3DXMATRIXA16* pmat)
{

}
