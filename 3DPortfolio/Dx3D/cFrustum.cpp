#include "StdAfx.h"
#include "cFrustum.h"


cFrustum::cFrustum(void)
{
	m_vecOrigProjVertex.clear();
	m_vecOrigProjVertex.push_back(D3DXVECTOR3(-1, 1, 1));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3( 1, 1, 1));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3(-1, 1, 0));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3( 1, 1, 0));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3(-1,-1, 1));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3( 1,-1, 1));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3(-1,-1, 0));
	m_vecOrigProjVertex.push_back(D3DXVECTOR3( 1,-1, 0));

	m_vecPlane.resize(6);
}


cFrustum::~cFrustum(void)
{
}

void cFrustum::Update()
{
	std::vector<D3DXVECTOR3> vecWorldVertex(8);

	D3DXMATRIXA16 matProj, matView;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	
	for (size_t i = 0; i < vecWorldVertex.size(); ++i)
	{
		D3DXVec3Unproject(&vecWorldVertex[i],
			&m_vecOrigProjVertex[i],
			NULL,
			&matProj,
			&matView,
			NULL);
	}

	D3DXPlaneFromPoints(&m_vecPlane[0],
		&vecWorldVertex[2], 
		&vecWorldVertex[3],
		&vecWorldVertex[6]);

	D3DXPlaneFromPoints(&m_vecPlane[1],
		&vecWorldVertex[1], 
		&vecWorldVertex[0],
		&vecWorldVertex[5]);

	D3DXPlaneFromPoints(&m_vecPlane[2],
		&vecWorldVertex[0], 
		&vecWorldVertex[2],
		&vecWorldVertex[4]);

	D3DXPlaneFromPoints(&m_vecPlane[3],
		&vecWorldVertex[3], 
		&vecWorldVertex[1],
		&vecWorldVertex[7]);

	D3DXPlaneFromPoints(&m_vecPlane[4],
		&vecWorldVertex[0], 
		&vecWorldVertex[1],
		&vecWorldVertex[2]);

	D3DXPlaneFromPoints(&m_vecPlane[5],
		&vecWorldVertex[6], 
		&vecWorldVertex[7],
		&vecWorldVertex[4]);
}

bool cFrustum::IsInFrustum( ST_SPHERE* pSphere )
{
	for each(D3DXPLANE p in m_vecPlane)
	{
		if(D3DXPlaneDotCoord(&p, &pSphere->vCenter) > pSphere->fRadius)
		{
			pSphere->isInFrustum = false;
			return false;
		}
	}
	pSphere->isInFrustum = true;
	return true;
}
