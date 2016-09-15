#include "stdafx.h"
#include "CharacterController.h"


CharacterController::CharacterController()
	:m_fSpeed(0.05f)
	,m_vDestination(0.0f,0.0f,0.0f)
	, m_vPos(0.0f,0.0f,0.0f)
	, m_vDir(0.0f,0.0f,-1.0f)
	, m_eState(IDLE)
	, m_pSkinnedMesh(nullptr)
{
}


CharacterController::~CharacterController()
{
}

void CharacterController::Init()
{
	m_pSkinnedMesh = new cSkinnedMesh("Data/Character/human/", "human.X");
	m_pSkinnedMesh->SetAnimationIndex(m_eState);
}

void CharacterController::Update()
{
	Attack();
	switch (m_eState)
	{
	case IDLE:
		break;
	case RUN:
		m_pSkinnedMesh->SetPosition(Running());
		break;
	case ATK:
		//if (!m_pSkinnedMesh->GetIsCoolTime())
		//	ChangeState(IDLE);

		if (m_pSkinnedMesh->AtkAnimationMatch())
			g_pSkillManager->Fire(m_vPos, m_vDir);
		break;
	}

}

void CharacterController::Destroy()
{
}

void CharacterController::Render()
{
	m_pSkinnedMesh->UpdateAndRender();
}

D3DXVECTOR3 & CharacterController::Running()
{
	D3DXVECTOR3 vDiff = m_vDestination - m_vPos;
	float fLen = D3DXVec3Length(&vDiff);
	if (fLen >= EPSILON)
	{
		D3DXVECTOR3 vDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXVec3Normalize(&vDirection, &(m_vDestination - m_vPos));

		m_vPos += (vDirection * m_fSpeed);
	}
	else
	{
		ChangeState(IDLE);
	}
		

	return m_vPos;
}

void CharacterController::StartRunning(D3DXVECTOR3& vDestination)
{
	ChangeState(RUN);
	SetDestination(vDestination);
	m_pSkinnedMesh->AddRotY(CalculateRotY());
}


float CharacterController::CalculateRotY()
{
	D3DXVECTOR3 vDir = m_vDir;
	D3DXVECTOR3 vDirDest;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vDirDest,&(m_vDestination - m_vPos));

	float fDot = D3DXVec3Dot(&vDir, &vDirDest);
	float fDirLength = D3DXVec3Length(&vDir);
	float fvDestLength = D3DXVec3Length(&vDirDest);
	float fCos = fDot / (fDirLength * fvDestLength);
	float fAngle = 0.0f;
	
	if (fCos >= 1.0f)
		fAngle = 0.0f;
	else 
		fAngle = acosf(fCos);

	D3DXVECTOR3 vCross;
	D3DXVec3Cross(&vCross, &vDir, &vDirDest);
	

	if (vCross.y < 0.0f)
		fAngle*=-1.0f;

	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, fAngle);
	D3DXVec3TransformCoord(&vDir, &vDir, &matR);
	m_vDir = vDir;

	return fAngle;
}

void CharacterController::Attack()
{
	if(g_KeyManager->IsOnceKeyDown('A'))
	{
		if (m_pSkinnedMesh->GetIsCoolTime())
			return;
		ChangeState(ATK);
	}
}

void CharacterController::ChangeState(eAnimationState state)
{
	m_eState = state;
	m_pSkinnedMesh->SetAnimationIndex(state);
}

