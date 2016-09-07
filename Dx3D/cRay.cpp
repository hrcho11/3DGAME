#include "StdAfx.h"
#include "cRay.h"


cRay::cRay(void)
	: m_vOrg(0, 0, 0)
	, m_vDir(0, 0, 0)
	, m_eRaySpace(E_NONE)
{
}


cRay::~cRay(void)
{
}

cRay cRay::RayAtViewSpace( int x, int y )
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	cRay r;
	r.m_vDir.x = (( x * 2.0f) / vp.Width  - 1.0f) / matProj._11;
	r.m_vDir.y = ((-y * 2.0f) / vp.Height + 1.0f) / matProj._22;
	r.m_vDir.z = 1.0f;

	r.m_eRaySpace = E_VIEW;

	return r;
}

cRay cRay::RayAtWorldSpace( int x, int y )
{
	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);

	cRay r = cRay::RayAtViewSpace(x, y);
	D3DXVec3TransformNormal(&r.m_vDir, &r.m_vDir, &matInvView);
	D3DXVec3TransformCoord(&r.m_vOrg, &r.m_vOrg, &matInvView);

	r.m_eRaySpace = E_WORLD;

	return r;
}

bool cRay::IsPicked( ST_SPHERE* pSphere )
{
	assert(m_eRaySpace == E_WORLD);
	D3DXVECTOR3 vLocalOrg = m_vOrg - pSphere->vCenter;
	float qv = D3DXVec3Dot(&vLocalOrg, &m_vDir);
	float vv = D3DXVec3Dot(&m_vDir, &m_vDir);
	float qq = D3DXVec3Dot(&vLocalOrg, &vLocalOrg);
	float rr = pSphere->fRadius * pSphere->fRadius;
	return qv * qv - vv * (qq - rr) >= 0;
}
