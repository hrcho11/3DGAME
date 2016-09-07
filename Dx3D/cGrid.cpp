#include "StdAfx.h"
#include "cGrid.h"
#include "cPyramid.h"

cGrid::cGrid(void)
{
}

cGrid::~cGrid(void)
{
}

void cGrid::Setup(int nNumLine, float fInterval)
{	
	int nNumHalfLine = nNumLine / 2;
	float fMax = fInterval * nNumHalfLine;
	D3DCOLOR c;

	for (int i = 1; i <= nNumHalfLine; ++i)
	{
		c = (i % 5 == 0) ? D3DCOLOR_XRGB(255, 255, 255) : D3DCOLOR_XRGB(128, 128, 128);
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0.0f, i), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0.0f, i), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0.0f,-i), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0.0f,-i), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i, 0.0f,-fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i, 0.0f, fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i, 0.0f,-fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i, 0.0f, fMax), c));
	}
	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0.0f, 0.0f), c));
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0.0f, 0.0f), c));

	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0.0f,-fMax, 0.0f), c));
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0.0f, fMax, 0.0f), c));

	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f,-fMax), c));
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0.0f, 0.0f, fMax), c));

	D3DXMATRIXA16 matS, matR, matWorld;

	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);

	cPyramid* pPyramid = NULL;
	pPyramid = new cPyramid;
	D3DXMatrixRotationZ(&matR, D3DX_PI / 2.0f);
	matWorld = matS * matR;
	pPyramid->Setup(D3DXCOLOR(0.7f, 0, 0, 1.0f), matWorld);
	m_vecPyramid.push_back(pPyramid);

	pPyramid = new cPyramid;
	D3DXMatrixRotationX(&matR, D3DX_PI);
	matWorld = matS * matR;
	pPyramid->Setup(D3DXCOLOR(0, 0.7f, 0, 1.0f), matWorld);
	m_vecPyramid.push_back(pPyramid);

	pPyramid = new cPyramid;
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	matWorld = matS * matR;
	pPyramid->Setup(D3DXCOLOR(0, 0, 0.7f, 1.0f), matWorld);
	m_vecPyramid.push_back(pPyramid);
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);

	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 
		m_vecVertex.size() / 2,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	for each (auto p in m_vecPyramid)
	{
		p->Render();
	}
}
