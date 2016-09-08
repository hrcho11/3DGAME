#include "stdafx.h"
#include "cCapsuleCollider.h"


cCapsuleCollider::cCapsuleCollider()
	: m_fRadius		(0.0f)
	, m_pVec3_A_Pos	(nullptr)
	, m_pVec3_B_Pos	(nullptr)
	, m_pMesh_A_	(nullptr)
	, m_pMesh_B_	(nullptr)
{
}

cCapsuleCollider::cCapsuleCollider(float p_fRadius,LPD3DXVECTOR3 p_pVec3A,LPD3DXVECTOR3 p_pVec3B)
	: m_fRadius		(p_fRadius)
	, m_pVec3_A_Pos	(p_pVec3A)
	, m_pVec3_B_Pos	(p_pVec3B)
{
	if (p_pVec3A&&p_pVec3B)
	{

		D3DXCreateSphere(g_pD3DDevice, m_fRadius, 10, 10, &m_pMesh_A_, nullptr);
		D3DXCreateSphere(g_pD3DDevice, m_fRadius, 10, 10, &m_pMesh_B_, nullptr);
	}
}

cCapsuleCollider::~cCapsuleCollider()
{
	SAFE_RELEASE(m_pMesh_A_);
	SAFE_RELEASE(m_pMesh_B_);
}

//메쉬를 삭제한 후 다시 만듬
void cCapsuleCollider::RemodelMesh(float p_fRadius)
{
	SAFE_RELEASE(m_pMesh_A_);
	SAFE_RELEASE(m_pMesh_B_);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMesh_A_, nullptr);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMesh_B_, nullptr);
}

bool cCapsuleCollider::IntersectSphere(cSphereCollider* p_pSphereCol)
{
	if (p_pSphereCol->GetPos() == nullptr)	return false;
	if (this->m_pVec3_A_Pos == nullptr)		return false;
	if (this->m_pVec3_B_Pos == nullptr)		return false;

	D3DXVECTOR3 vec3P = (*p_pSphereCol->GetPos());
	D3DXVECTOR3 vec3A = (*this->m_pVec3_A_Pos);
	D3DXVECTOR3 vec3B = (*this->m_pVec3_B_Pos);
	D3DXVECTOR3 vec3BA = vec3B - vec3A;
	D3DXVECTOR3	vec3PA = vec3P - vec3A;
	float fLength = D3DXVec3Length(&vec3BA);
	float fProjBA = D3DXVec3Dot(&vec3PA, &vec3BA) / fLength;

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
		(this->m_fRadius + p_pSphereCol->GetRadius())*
		(this->m_fRadius + p_pSphereCol->GetRadius());

	return (fSumRadiusSq > fLengthSq);
}

bool cCapsuleCollider::IntersectCapsule(cCapsuleCollider* p_pCapsuleCol)
{
	if (p_pCapsuleCol->Get_A_Pos() == nullptr)	return false;
	if (p_pCapsuleCol->Get_B_Pos() == nullptr)	return false;
	if (this->m_pVec3_A_Pos == nullptr)			return false;
	if (this->m_pVec3_B_Pos == nullptr)			return false;

	float fDistanceFromA = 0.0f;
	float fDistanceFromP = 0.0f;
	float fLengthSq = 0.0f;
	float fSumRadiusSq = 0.0f;
	D3DXVECTOR3 vec3ClosestPosOnAB;
	D3DXVECTOR3 vec3ClosestPosOnPQ;
	D3DXVECTOR3 vec3A = (*this->m_pVec3_A_Pos);
	D3DXVECTOR3 vec3B = (*this->m_pVec3_B_Pos);
	D3DXVECTOR3 vec3P = (*p_pCapsuleCol->Get_A_Pos());
	D3DXVECTOR3 vec3Q = (*p_pCapsuleCol->Get_B_Pos());
	D3DXVECTOR3 vec3BA = vec3B - vec3A;
	D3DXVECTOR3 vec3QP = vec3Q - vec3P;
	D3DXVECTOR3 vec3AP = vec3A - vec3P;
	
	float fBALengthSq = D3DXVec3Dot(&vec3BA, &vec3BA);
	float fQPLengthSq = D3DXVec3Dot(&vec3QP, &vec3QP);
	float fDot_QP_AP = D3DXVec3Dot(&vec3QP, &vec3AP);

	if (fBALengthSq <= FLT_EPSILON&&fQPLengthSq <= FLT_EPSILON)
	{
		vec3ClosestPosOnAB = vec3A;
		vec3ClosestPosOnPQ = vec3P;

		fLengthSq = D3DXVec3LengthSq(&(vec3ClosestPosOnAB - vec3ClosestPosOnPQ));
		fSumRadiusSq =
			(this->m_fRadius + p_pCapsuleCol->m_fRadius)*
			(this->m_fRadius + p_pCapsuleCol->m_fRadius);

		return (fSumRadiusSq > fLengthSq);
	}
	if (fBALengthSq <= FLT_EPSILON)
	{
		fDistanceFromA = 0.0f;
		fDistanceFromP = fDot_QP_AP / fQPLengthSq;
		if (fDistanceFromP < 0.0f)
		{
			fDistanceFromP = 0.0f;
		}
		else if (fDistanceFromP > 1.0f)
		{
			fDistanceFromP = 1.0f;
		}
	}
	else
	{
		float fDot_BA_AP = D3DXVec3Dot(&vec3BA,&vec3AP);
		if (fQPLengthSq <= FLT_EPSILON)
		{
			fDistanceFromP = 0.0f;
			fDistanceFromA = -fDot_BA_AP / fBALengthSq;
			if (fDistanceFromA < 0.0f)
			{
				fDistanceFromA = 0.0f;
			}
			else if (fDistanceFromA > 1.0f)
			{
				fDistanceFromA = 1.0f;
			}
		}
		else
		{
			float fDot_BA_QP = D3DXVec3Dot(&vec3BA, &vec3QP);
			float denom = fBALengthSq * fQPLengthSq - fDot_BA_QP * fDot_BA_QP;

			if (denom != 0.0f)
			{
				fDistanceFromA = (fDot_BA_QP*fDot_QP_AP - fDot_BA_AP*fQPLengthSq) / denom;
				if (fDistanceFromA < 0.0f)
				{
					fDistanceFromA = 0.0f;
				}
				else if (fDistanceFromA > 1.0f)
				{
					fDistanceFromA = 1.0f;
				}
			}
			else
			{
				fDistanceFromA = 0.0f;
			}

			fDistanceFromP = (fDot_BA_QP*fDistanceFromA + fDot_QP_AP) / fQPLengthSq;

			if (fDistanceFromP < 0.0f)
			{
				fDistanceFromP = 0.0f;
				fDistanceFromA = -fDot_BA_AP / fBALengthSq;
				if (fDistanceFromA < 0.0f)
				{
					fDistanceFromA = 0.0f;
				}
				else if (fDistanceFromA > 1.0f)
				{
					fDistanceFromA = 1.0f;
				}
			}
			else if (fDistanceFromP > 1.0f)
			{
				fDistanceFromP = 1.0f;
				fDistanceFromA = (fDot_BA_QP-fDot_BA_AP)/ fBALengthSq;
				if (fDistanceFromA < 0.0f)
				{
					fDistanceFromA = 0.0f;
				}
				else if (fDistanceFromA > 1.0f)
				{
					fDistanceFromA = 1.0f;
				}
			}
		}
	}
	vec3ClosestPosOnAB = vec3A + vec3BA*fDistanceFromA;
	vec3ClosestPosOnPQ = vec3P + vec3QP*fDistanceFromP;

	fLengthSq = D3DXVec3LengthSq(&(vec3ClosestPosOnAB - vec3ClosestPosOnPQ));
	fSumRadiusSq =
		(this->m_fRadius + p_pCapsuleCol->m_fRadius)*
		(this->m_fRadius + p_pCapsuleCol->m_fRadius);

	return (fSumRadiusSq > fLengthSq);
}

//override cCollider

cCollider::E_TYPE cCapsuleCollider::GetType()
{
	return cCollider::E_TYPE::eCAPSULE;
}


bool cCapsuleCollider::IntersectCollider(cCollider* p_pCol)
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


void cCapsuleCollider::Render()
{
	if (m_pVec3_A_Pos&&m_pVec3_B_Pos)
	{
		D3DXMATRIXA16 matT;

		D3DXMatrixTranslation(&matT, m_pVec3_A_Pos->x, m_pVec3_A_Pos->y, m_pVec3_A_Pos->z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
		g_pD3DDevice->SetTexture(0, nullptr);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pMesh_A_->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


		D3DXMatrixTranslation(&matT, m_pVec3_B_Pos->x, m_pVec3_B_Pos->y, m_pVec3_B_Pos->z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
		g_pD3DDevice->SetTexture(0, nullptr);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pMesh_B_->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	}
}