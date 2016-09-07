#include "stdafx.h"
#include "cSphereCollider.h"


cSphereCollider::cSphereCollider()
	: m_fRadius		(1.0f)
	, m_pVec3Pos	(nullptr)
{
	D3DXCreateSphere(g_pD3DDevice, m_fRadius, 10, 10, &m_pMesh, nullptr);
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


//override cCollider
cCollider::E_TYPE cSphereCollider::GetType()
{
	return cCollider::E_TYPE::eSPHERE;
}


bool cSphereCollider::IntersectCollider(cCollider* p_pCol)
{
	switch (p_pCol->GetType())
	{
	case cCollider::E_TYPE::eSPHERE:
		{
			if (p_pCol==nullptr)return false;

			cSphereCollider* sphereCol = (cSphereCollider*)p_pCol;

			if (this->m_pVec3Pos == nullptr)return false;
			if (sphereCol->GetPos() == nullptr)return false;

			float fLengthSq =
				D3DXVec3LengthSq(
				&(
				(*this->m_pVec3Pos) - (*sphereCol->GetPos())
				));
			float fSumRadiusSq =
				(this->m_fRadius + sphereCol->GetRadius())*
				(this->m_fRadius + sphereCol->GetRadius());

			return (fSumRadiusSq > fLengthSq);
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
	D3DXMATRIXA16 mat;
	if (m_pVec3Pos)
	{
		D3DXMatrixTranslation(&mat, m_pVec3Pos->x, m_pVec3Pos->y, m_pVec3Pos->z);
	}
	else
	{
		D3DXMatrixTranslation(&mat, 0.0f, 0.0f, 0.0f);
	}
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetTexture(0, nullptr);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pMesh->DrawSubset(0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}