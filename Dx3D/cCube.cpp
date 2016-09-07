#include "StdAfx.h"
#include "cCube.h"

cCube::cCube(void)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}

cCube::~cCube(void)
{
}

void cCube::Setup(D3DXMATRIXA16* pmat)
{	
	std::vector<D3DXVECTOR3> vecP;
	vecP.push_back(D3DXVECTOR3(-1.0f,-1.0f,-1.0f));
	vecP.push_back(D3DXVECTOR3(-1.0f, 1.0f,-1.0f));
	vecP.push_back(D3DXVECTOR3( 1.0f, 1.0f,-1.0f));
	vecP.push_back(D3DXVECTOR3( 1.0f,-1.0f,-1.0f));
	vecP.push_back(D3DXVECTOR3(-1.0f,-1.0f, 1.0f));
	vecP.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	vecP.push_back(D3DXVECTOR3( 1.0f, 1.0f, 1.0f));
	vecP.push_back(D3DXVECTOR3( 1.0f,-1.0f, 1.0f));

	if(pmat)
	{
		for (size_t i = 0; i < vecP.size(); ++i)
		{
			D3DXVec3TransformCoord(&vecP[i], &vecP[i], pmat);
		}
	}
	D3DXVECTOR3 n;

	n = D3DXVECTOR3( 0, 0,-1);
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[0], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[1], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[2], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[0], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[2], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[3], n));

	n = D3DXVECTOR3( 0, 0, 1);
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[4], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[6], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[5], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[4], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[7], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[6], n));
	
	n = D3DXVECTOR3(-1, 0, 0);
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[4], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[5], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[1], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[4], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[1], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[0], n));
	
	n = D3DXVECTOR3( 1, 0, 0);
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[3], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[2], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[6], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[3], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[6], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[7], n));
	
	n = D3DXVECTOR3( 0, 1, 0);
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[1], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[5], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[6], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[1], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[6], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[2], n));
	
	n = D3DXVECTOR3( 0, -1, 0);
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[4], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[0], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[3], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[4], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[3], n));
	m_vecVertex.push_back(ST_PN_VERTEX(vecP[7], n));
}

void cCube::Render(D3DXMATRIXA16* pmat)
{
	if(pmat)
		m_matWorld = *pmat;

	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PN_VERTEX));
}

void cCube::SetMtlColor( D3DXCOLOR c )
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = c;
}

