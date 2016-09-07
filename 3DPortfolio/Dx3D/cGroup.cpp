#include "StdAfx.h"
#include "cGroup.h"
#include "cMtlTex.h"

cGroup::cGroup(void)
	: m_pMtlTex(NULL)
	, m_pVB(NULL)
	, m_nNumTri(0)
{
}


cGroup::~cGroup(void)
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
}

void cGroup::Render()
{	
	DWORD dwFillMode;
	g_pD3DDevice->GetRenderState(D3DRS_FILLMODE, &dwFillMode);
	if (dwFillMode == D3DFILL_WIREFRAME)
	{
		g_pD3DDevice->SetTexture(0, NULL);
	}
	else
	{
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
	}
	
	g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);
}

void cGroup::SetVertex( std::vector<ST_PNT_VERTEX>& vecVertex )
{
	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX),
		0,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	m_nNumTri = vecVertex.size() / 3;

	ST_PNT_VERTEX* pV;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVB->Unlock();

}
