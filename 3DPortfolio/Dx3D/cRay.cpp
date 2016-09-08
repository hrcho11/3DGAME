#include "stdafx.h"
#include "cRay.h"


cRay::cRay()
	:m_vOrigin(0.0f,0.0f,0.0f)
	,m_vDir(0.0f,0.0f,0.0f)
{

}


cRay::~cRay()
{
}

const cRay& cRay::GetRayVector(int x, int y)
{
	cRay ray;
	D3DVIEWPORT9 viewPort;
	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetViewport(&viewPort);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	ray.m_vDir.x = ((x *2.0f) / viewPort.Width - 1.0f) / matProj._11;
	ray.m_vDir.y = ((-y * 2.0f) / viewPort.Height + 1.0f) / matProj._22;
	ray.m_vDir.z = 0.0f;

	D3DXMATRIXA16 matView, matInvView;

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, NULL, &matView);
	D3DXVec3TransformNormal(&ray.m_vDir, &ray.m_vDir, &matInvView);
	D3DXVec3TransformCoord(&ray.m_vOrigin, &ray.m_vOrigin, &matInvView);

	return ray;
}

