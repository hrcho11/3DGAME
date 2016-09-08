#include "stdafx.h"
#include "cSphereCollider.h"


cSphereCollider::cSphereCollider()
	: m_fRadius		(0.0f)
	, m_pVec3Pos	(nullptr)
	, m_pMesh		(nullptr)
{
}

cSphereCollider::cSphereCollider(float p_fRadius, LPD3DXVECTOR3 p_pVec3Pos)
	: m_fRadius		(p_fRadius)
	, m_pVec3Pos	(p_pVec3Pos)
{
	D3DXCreateSphere(g_pD3DDevice, m_fRadius, 10, 10, &m_pMesh, nullptr);
}

cSphereCollider::~cSphereCollider()
{
	SAFE_RELEASE(m_pMesh);
}

//메쉬를 삭제한 후 다시 만듬
void cSphereCollider::RemodelMesh(float p_fRadius)
{
	SAFE_RELEASE(m_pMesh);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMesh, nullptr);
}

bool cSphereCollider::IntersectSphere(cSphereCollider* p_pSphereCol)
{
	if (this->m_pVec3Pos == nullptr)		return false;
	if (p_pSphereCol->GetPos() == nullptr)	return false;

	float fLengthSq =
		D3DXVec3LengthSq(
		&(
		(*this->m_pVec3Pos) - (*p_pSphereCol->GetPos())
		));
	float fSumRadiusSq =
		(this->m_fRadius + p_pSphereCol->GetRadius())*
		(this->m_fRadius + p_pSphereCol->GetRadius());

	return (fSumRadiusSq > fLengthSq);
}

bool cSphereCollider::IntersectCapsule(cCapsuleCollider* p_pCapsuleCol)
{
	if (p_pCapsuleCol->Get_A_Pos() == nullptr)	return false;
	if (p_pCapsuleCol->Get_B_Pos() == nullptr)	return false;
	if (this->m_pVec3Pos == nullptr)			return false;

	D3DXVECTOR3 vec3A = (*p_pCapsuleCol->Get_A_Pos());
	D3DXVECTOR3 vec3B = (*p_pCapsuleCol->Get_B_Pos());
	D3DXVECTOR3 vec3P = (*this->m_pVec3Pos);
	D3DXVECTOR3 vec3BA = vec3B - vec3A;
	D3DXVECTOR3	vec3PA = vec3P - vec3A;
	float fLength = D3DXVec3Length(&vec3BA);
	float fProjBA = D3DXVec3Dot(&vec3PA,&vec3BA)/ fLength;
	
	D3DXVECTOR3 vec3ClosestPos;
	if (fProjBA < 0)
	{
		vec3ClosestPos = vec3A;
	}
	else if (fProjBA > fLength)
	{
		vec3ClosestPos = vec3B;
	}
	else
	{
		D3DXVec3Normalize(&vec3BA, &vec3BA);
		vec3ClosestPos = vec3A + fProjBA*vec3BA;
	}

	float fLengthSq = D3DXVec3LengthSq(&(vec3P - vec3ClosestPos));
	float fSumRadiusSq =
		(this->m_fRadius + p_pCapsuleCol->GetRadius())*
		(this->m_fRadius + p_pCapsuleCol->GetRadius());

	return (fSumRadiusSq > fLengthSq);
}

//override cCollider

cCollider::E_TYPE cSphereCollider::GetType()
{
	return cCollider::E_TYPE::eSPHERE;
}


bool cSphereCollider::IntersectCollider(cCollider* p_pCol)
{
	if (p_pCol == nullptr)return false;

	switch (p_pCol->GetType())
	{
	case cCollider::E_TYPE::eSPHERE:
		{
			cSphereCollider* pSphereCol = (cSphereCollider*)p_pCol;
			return this->IntersectSphere(pSphereCol);
		}
		break;
	case cCollider::E_TYPE::eCAPSULE:
		{
			cCapsuleCollider* pCapsuleCol = (cCapsuleCollider*)p_pCol;
			return this->IntersectCapsule(pCapsuleCol);
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return false;
}


void cSphereCollider::Render()
{
	if (m_pVec3Pos)
	{
		D3DXMATRIXA16 matT;

		D3DXMatrixTranslation(&matT, m_pVec3Pos->x, m_pVec3Pos->y, m_pVec3Pos->z);

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
		g_pD3DDevice->SetTexture(0, nullptr);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}