#include "stdafx.h"
#include "cFrustumCulling.h"

cFrustumCulling::cFrustumCulling()
{
}


cFrustumCulling::~cFrustumCulling()
{
}


void cFrustumCulling::Initialize()
{
	m_vtxProjection[0] = D3DXVECTOR3(-1.f, -1.f, 0.f);
	m_vtxProjection[1] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_vtxProjection[2] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_vtxProjection[3] = D3DXVECTOR3(-1.f, -1.f, 1.f);
	m_vtxProjection[4] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_vtxProjection[5] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_vtxProjection[6] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vtxProjection[7] = D3DXVECTOR3(-1.f, 1.f, 1.f);

	D3DXMATRIXA16 matView, matProj;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_vecWorldVertex.clear();
	m_vecWorldVertex.resize(8);
	for (int i = 0; i < 8; ++i)
		D3DXVec3Unproject(
			&m_vecWorldVertex[i],
			&m_vtxProjection[i],
			nullptr,
			&matProj,
			&matView,
			nullptr);


	m_vPos = (m_vecWorldVertex[0] + m_vecWorldVertex[5]) / 2;

	D3DXPlaneFromPoints(&m_plane[0], &m_vecWorldVertex[4], &m_vecWorldVertex[7], &m_vecWorldVertex[6]);//상
	D3DXPlaneFromPoints(&m_plane[1], &m_vecWorldVertex[0], &m_vecWorldVertex[1], &m_vecWorldVertex[2]);//하
	
	D3DXPlaneFromPoints(&m_plane[2], &m_vecWorldVertex[0], &m_vecWorldVertex[4], &m_vecWorldVertex[5]);//근
	D3DXPlaneFromPoints(&m_plane[3], &m_vecWorldVertex[2], &m_vecWorldVertex[6], &m_vecWorldVertex[7]);//원
	
	D3DXPlaneFromPoints(&m_plane[4], &m_vecWorldVertex[0], &m_vecWorldVertex[3], &m_vecWorldVertex[7]);//좌
	D3DXPlaneFromPoints(&m_plane[5], &m_vecWorldVertex[1], &m_vecWorldVertex[5], &m_vecWorldVertex[6]);//우


}

void cFrustumCulling::Update()
{
	D3DXMATRIXA16 matView, matProj;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_vecWorldVertex.clear();
	m_vecWorldVertex.resize(8);
	for (int i = 0; i < 8; ++i)
		D3DXVec3Unproject(
			&m_vecWorldVertex[i],
			&m_vtxProjection[i],
			nullptr,
			&matProj,
			&matView,
			nullptr);
	
	D3DXPlaneFromPoints(&m_plane[0], &m_vecWorldVertex[4], &m_vecWorldVertex[7], &m_vecWorldVertex[6]);
	D3DXPlaneFromPoints(&m_plane[1], &m_vecWorldVertex[0], &m_vecWorldVertex[1], &m_vecWorldVertex[2]);
	D3DXPlaneFromPoints(&m_plane[2], &m_vecWorldVertex[0], &m_vecWorldVertex[4], &m_vecWorldVertex[5]);
	D3DXPlaneFromPoints(&m_plane[3], &m_vecWorldVertex[2], &m_vecWorldVertex[6], &m_vecWorldVertex[7]);
	D3DXPlaneFromPoints(&m_plane[4], &m_vecWorldVertex[0], &m_vecWorldVertex[3], &m_vecWorldVertex[7]);
	D3DXPlaneFromPoints(&m_plane[5], &m_vecWorldVertex[1], &m_vecWorldVertex[5], &m_vecWorldVertex[6]);

	m_vPos = (m_vecWorldVertex[0] + m_vecWorldVertex[5]) / 2;
}

void cFrustumCulling::Render()
{
	WORD index[] = { 0,1,2,
						 0,2,3,
						 4,7,6,
						 4,6,5,
						 1,5,6,
						 1,6,2,
						 0,3,7,
						 0,7,4,
						 0,4,5,
						 0,5,1,
						 3,7,6,
						 3,6,2 };

	
	D3DXVECTOR3 vtx[8];
	for (int i = 0; i < 8; ++i)
		vtx[i] = m_vecWorldVertex[i];

	g_pD3DDevice->SetFVF(D3DFVF_XYZ);
	g_pD3DDevice->SetStreamSource(0, NULL, 0, sizeof(D3DXVECTOR3));
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetIndices(0);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	g_pD3DDevice->SetMaterial(&mtrl);
	g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
		0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	g_pD3DDevice->SetMaterial(&mtrl);
	g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
		0, 8, 4, index + 4*3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
	
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	g_pD3DDevice->SetMaterial(&mtrl);
	g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
		0, 8, 4, index + 8*3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

bool cFrustumCulling::IsIn(D3DXVECTOR3 * pv)
{
	float fDist;
	fDist = D3DXPlaneDotCoord(&m_plane[3], pv);
		if (fDist > 0.0001f) return false;
	fDist = D3DXPlaneDotCoord(&m_plane[4], pv);
	if (fDist > 0.0001f) return false;
	fDist = D3DXPlaneDotCoord(&m_plane[5], pv);
	if (fDist > 0.0001f) return false;

	return true;
}

bool cFrustumCulling::IsInSphere(D3DXVECTOR3 * pv, float radius)
{
	float fDist = 0.0f;
	fDist = D3DXPlaneDotCoord(&m_plane[3], pv);
	if (fDist > radius + FRUSTUM_SPACE) return false;
	fDist = D3DXPlaneDotCoord(&m_plane[4], pv);
	if (fDist > radius + FRUSTUM_SPACE) return false;
	fDist = D3DXPlaneDotCoord(&m_plane[5], pv);
	if (fDist > radius + FRUSTUM_SPACE) return false;

	return true;
}

