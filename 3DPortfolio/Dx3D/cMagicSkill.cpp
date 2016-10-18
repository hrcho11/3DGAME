#include "stdafx.h"
#include "cMagicSkill.h"


cMagicSkill::cMagicSkill()
	:m_nAtkDamage(20)
	,m_fMoveSpeed(20.0f)
	,m_pMesh(nullptr)
	, m_vPos(0.0f,0.0f,-1.0f)
	,m_isFire(false)
	, m_fDistance(600.0f)
	, m_vDir(0.0f,0.0f,0.0f)
	, m_vStartPos(0.0f,0.0f,0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXCreateSphere(g_pD3DDevice,
		0.5f,
		20,
		20,
		&m_pMesh,
		NULL);
}


cMagicSkill::~cMagicSkill()
{
}

void cMagicSkill::Init(D3DXVECTOR3& vPos)
{
	m_vPos = m_vStartPos = vPos;
	D3DXMatrixTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);

}

void cMagicSkill::Update()
{
	if (m_isFire)
	{
		//m_vPos += m_vDir  * m_fMoveSpeed;
		if(m_fDistance <=D3DXVec3LengthSq(&(m_vPos - m_vStartPos)))
			SetToOriginal();
		D3DXMatrixTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
	}

}

void cMagicSkill::Destroy()
{
	SAFE_DELETE(m_pCollider);
	SAFE_RELEASE(m_pMesh);
}

void cMagicSkill::Render()
{
	if (m_isFire)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pMesh->DrawSubset(0);
	}

}

void cMagicSkill::SetToOriginal()
{
	m_isFire = false;
	m_vPos = D3DXVECTOR3(300.0f,300.0f,300.0f);
	SAFE_DELETE(m_pCollider);
}

void cMagicSkill::SetDiretion(D3DXVECTOR3 & vDir)
{
	m_vDir = vDir;
	m_pCollider = new cSphereCollider(0.4f, &m_vPos, &m_fMoveSpeed, &m_vDir);
}
