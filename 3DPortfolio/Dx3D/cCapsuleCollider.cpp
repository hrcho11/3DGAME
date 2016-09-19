#include "stdafx.h"
#include "cCapsuleCollider.h"

cCapsuleCollider::cCapsuleCollider
	(float			p_fRadius, 
	LPD3DXVECTOR3	p_pVec3RadRot, 
	LPD3DXVECTOR3	p_pVec3Pivot, 
	D3DXVECTOR3&	p_vec3_A_Pos, 
	D3DXVECTOR3&	p_vec3_B_Pos)

	: m_fRadius		(p_fRadius)
	, m_pVec3RadRot	(p_pVec3RadRot)
	, m_pVec3Pivot	(p_pVec3Pivot)
	, m_vec3DistA	(p_vec3_A_Pos)
	, m_vec3DistB	(p_vec3_B_Pos)
	, m_pMesh		(nullptr)
	, m_pMeshA		(nullptr)
	, m_pMeshB		(nullptr)
{
	D3DXCreateCylinder(g_pD3DDevice, p_fRadius, p_fRadius, D3DXVec3Length(&(m_vec3DistA - m_vec3DistB)), 10, 10, &m_pMesh, nullptr);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMeshA, nullptr);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMeshB, nullptr);
}

cCapsuleCollider::~cCapsuleCollider()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMeshA);
	SAFE_RELEASE(m_pMeshB);
}

//메쉬를 삭제한 후 다시 만듬
void cCapsuleCollider::RemodelMesh(float p_fRadius)
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMeshA);
	SAFE_RELEASE(m_pMeshB);
	D3DXCreateCylinder(g_pD3DDevice, p_fRadius, p_fRadius, D3DXVec3Length(&(m_vec3DistA - m_vec3DistB)), 10, 10, &m_pMesh, nullptr);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMeshA, nullptr);
	D3DXCreateSphere(g_pD3DDevice, p_fRadius, 10, 10, &m_pMeshB, nullptr);
}

bool cCapsuleCollider::IntersectSphere(cSphereCollider* p_pSphereCol)
{
	if (this->m_pVec3Pivot			== nullptr) return false;
	if (this->m_pVec3RadRot			== nullptr) return false;
	if (p_pSphereCol->GetPivot()	== nullptr) return false;
	
	D3DXMATRIXA16 matR, matRX, matRY, matRZ;
	D3DXMatrixRotationX(&matRX, this->m_pVec3RadRot->x);
	D3DXMatrixRotationY(&matRY, this->m_pVec3RadRot->y);
	D3DXMatrixRotationZ(&matRZ, this->m_pVec3RadRot->z);
	matR = matRX * matRY * matRZ;

	D3DXVECTOR3 vec3A = (this->m_vec3DistA);
						D3DXVec3TransformCoord(&vec3A, &vec3A, &matR);
						vec3A += *this->m_pVec3Pivot;
	D3DXVECTOR3 vec3B = (this->m_vec3DistB);
						D3DXVec3TransformCoord(&vec3B, &vec3B, &matR);
						vec3B += *this->m_pVec3Pivot;
	D3DXVECTOR3 vec3P = (*p_pSphereCol->GetPivot());
	D3DXVECTOR3 vec3AB = vec3B - vec3A;
	D3DXVECTOR3	vec3AP = vec3P - vec3A;
	float fLength = D3DXVec3Length(&vec3AB);
	float fProjAB = D3DXVec3Dot(&vec3AP, &vec3AB) / fLength;

	D3DXVECTOR3 vec3ClosestPos;
	if (fProjAB < 0)
	{
		vec3ClosestPos = vec3A;
	}
	else if (fProjAB > fLength)
	{
		vec3ClosestPos = vec3B;
	}
	else
	{
		D3DXVec3Normalize(&vec3AB, &vec3AB);
		vec3ClosestPos = vec3A + fProjAB*vec3AB;
	}

	float fLengthSq = D3DXVec3LengthSq(&(vec3P - vec3ClosestPos));
	float fSumRadiusSq =
		(this->m_fRadius + p_pSphereCol->GetRadius())*
		(this->m_fRadius + p_pSphereCol->GetRadius());

	return (fSumRadiusSq > fLengthSq);
}

bool cCapsuleCollider::IntersectCapsule(cCapsuleCollider* p_pCapsuleCol)
{
	if (this->m_pVec3Pivot				== nullptr) return false;
	if (this->m_pVec3RadRot				== nullptr) return false;
	if (p_pCapsuleCol->GetPivot()		== nullptr) return false;
	if (p_pCapsuleCol->GetRadRotation()	== nullptr) return false;

	float fDistanceFromA = 0.0f;
	float fDistanceFromP = 0.0f;
	float fLengthSq = 0.0f;
	float fSumRadiusSq = 0.0f;
	D3DXVECTOR3 vec3ClosestPosOnAB;
	D3DXVECTOR3 vec3ClosestPosOnPQ;

	D3DXMATRIXA16 matR_AB, matR_PQ, matRX, matRY, matRZ;
	D3DXMatrixRotationX(&matRX, this->m_pVec3RadRot->x);
	D3DXMatrixRotationY(&matRY, this->m_pVec3RadRot->y);
	D3DXMatrixRotationZ(&matRZ, this->m_pVec3RadRot->z);
	matR_AB = matRX * matRY * matRZ;
	D3DXMatrixRotationX(&matRX, p_pCapsuleCol->GetRadRotation()->x);
	D3DXMatrixRotationY(&matRY, p_pCapsuleCol->GetRadRotation()->y);
	D3DXMatrixRotationZ(&matRZ, p_pCapsuleCol->GetRadRotation()->z);
	matR_PQ = matRX * matRY * matRZ;

	D3DXVECTOR3 vec3A = (this->m_vec3DistA);
						D3DXVec3TransformCoord(&vec3A, &vec3A, &matR_AB);
						vec3A += *this->m_pVec3Pivot;
	D3DXVECTOR3 vec3B = (this->m_vec3DistB);
						D3DXVec3TransformCoord(&vec3B, &vec3B, &matR_AB);
						vec3B += *this->m_pVec3Pivot;
	D3DXVECTOR3 vec3P = (p_pCapsuleCol->GetDistanceA());
						D3DXVec3TransformCoord(&vec3P, &vec3P, &matR_PQ);
						vec3P += *p_pCapsuleCol->GetPivot();
	D3DXVECTOR3 vec3Q = (p_pCapsuleCol->GetDistanceB());
						D3DXVec3TransformCoord(&vec3Q, &vec3Q, &matR_PQ);
						vec3Q += *p_pCapsuleCol->GetPivot();
	D3DXVECTOR3 vec3AB = vec3B - vec3A;
	D3DXVECTOR3 vec3PQ = vec3Q - vec3P;
	D3DXVECTOR3 vec3PA = vec3A - vec3P;
	
	float fABLengthSq = D3DXVec3Dot(&vec3AB, &vec3AB);
	float fPQLengthSq = D3DXVec3Dot(&vec3PQ, &vec3PQ);
	float fDot_PQ_PA = D3DXVec3Dot(&vec3PQ, &vec3PA);

	if (fABLengthSq <= FLT_EPSILON && fPQLengthSq <= FLT_EPSILON)
	{
		vec3ClosestPosOnAB = vec3A;
		vec3ClosestPosOnPQ = vec3P;

		fLengthSq = D3DXVec3LengthSq(&(vec3ClosestPosOnAB - vec3ClosestPosOnPQ));
		fSumRadiusSq =
			(this->m_fRadius + p_pCapsuleCol->m_fRadius)*
			(this->m_fRadius + p_pCapsuleCol->m_fRadius);

		return (fSumRadiusSq > fLengthSq);
	}
	if (fABLengthSq <= FLT_EPSILON)
	{
		fDistanceFromA = 0.0f;
		fDistanceFromP = fDot_PQ_PA / fPQLengthSq;
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
		float fDot_AB_PA = D3DXVec3Dot(&vec3AB,&vec3PA);
		if (fPQLengthSq <= FLT_EPSILON)
		{
			fDistanceFromP = 0.0f;
			fDistanceFromA = -fDot_AB_PA / fABLengthSq;
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
			float fDot_AB_PQ = D3DXVec3Dot(&vec3AB, &vec3PQ);
			float denom = fABLengthSq * fPQLengthSq - fDot_AB_PQ * fDot_AB_PQ;

			if (denom != 0.0f)
			{
				fDistanceFromA = (fDot_AB_PQ * fDot_PQ_PA - fDot_AB_PA * fPQLengthSq) / denom;
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

			fDistanceFromP = (fDot_AB_PQ * fDistanceFromA + fDot_PQ_PA) / fPQLengthSq;

			if (fDistanceFromP < 0.0f)
			{
				fDistanceFromP = 0.0f;
				fDistanceFromA = -fDot_AB_PA / fABLengthSq;
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
				fDistanceFromA = (fDot_AB_PQ - fDot_AB_PA) / fABLengthSq;
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
	vec3ClosestPosOnAB = vec3A + vec3AB * fDistanceFromA;
	vec3ClosestPosOnPQ = vec3P + vec3PQ * fDistanceFromP;

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

//미구현
bool cCapsuleCollider::CollidePhysically(cCollider* p_pCol)
{
	return false;
}

void cCapsuleCollider::Update()
{

}

//보류
void cCapsuleCollider::Render()
{
	
	//if (this->m_pVec3Pivot && this->m_pVec3RadRot && this->m_pMesh)
	//{
	//	D3DXMATRIXA16 matR, matRX, matRY, matRZ, matT;
	//	D3DXMatrixRotationX(&matRX, this->m_pVec3RadRot->x);
	//	D3DXMatrixRotationY(&matRY, this->m_pVec3RadRot->y);
	//	D3DXMatrixRotationZ(&matRZ, this->m_pVec3RadRot->z);
	//	matR = matRX * matRY * matRZ;
	//	D3DXMatrixTranslation(&matT, this->m_pVec3Pivot->x, this->m_pVec3Pivot->y, this->m_pVec3Pivot->z);
	//
	//	g_pD3DDevice->SetTransform(D3DTS_WORLD, &(matR*matT));
	//	g_pD3DDevice->SetTexture(0, nullptr);
	//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//	m_pMesh->DrawSubset(0);
	//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//
	//}
}