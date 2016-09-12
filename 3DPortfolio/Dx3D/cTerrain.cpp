#include "stdafx.h"
#include "cTerrain.h"


cTerrain::cTerrain()
	:m_cxDIB(0)
	, m_cyDIB(0)
	, m_czDIB(0)
	, m_vfScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	, m_pvHeightMap(nullptr)
	, m_pIB(nullptr)
	, m_pVB(nullptr)
	, m_nTriangles(NULL)
	, m_pQuadTree(nullptr)
	,m_pTexture(nullptr)
	, m_fLODRatio(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cTerrain::~cTerrain()
{
	SAFE_DESTROY(m_pQuadTree);
	SAFE_DELETE_ARRAY(m_pvHeightMap);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
}

void cTerrain::Draw(cFrustumCulling* pFrustum)
{
	LPDWORD pI;

	m_pIB->Lock(0, (m_cxDIB - 1) * (m_czDIB-1)* 2 * sizeof(TRIINDEX), (LPVOID*)&pI, 0);
	m_nTriangles = m_pQuadTree->GenerateIndex(pI,m_pvHeightMap, pFrustum,0.1f);
	m_pIB->Unlock();
	Render();
}

void cTerrain::Create()
{
	BuildTerrain();
	LoadTexture("./Data/Map/texture.jpg");
	CreateVIB();
	BuildQuadTree();
	CreatePickingGround();
}

void cTerrain::Destroy()
{

}

void cTerrain::LoadTexture(char * lpTexFileName)
{
	m_pTexture = g_pTextureManager->GetTexture(lpTexFileName);
}

void cTerrain::BuildTerrain()
{
	m_cxDIB = TERRAIN_SIZE;
	m_czDIB = TERRAIN_SIZE;

	m_pvHeightMap = new ST_PNT_VERTEX[m_cxDIB *m_czDIB];

	ST_PNT_VERTEX v;
	for (int z = 0; z < m_czDIB; z++)
	{
		for (int x = 0; x < m_cxDIB; x++)
		{
			v.p.x = (float)((x - m_cxDIB / 2) *m_vfScale.x);
			v.p.z = -(float)((z - m_czDIB / 2) * m_vfScale.z);
			v.p.y = 0;
			D3DXVec3Normalize(&v.n, &v.p);
			v.t.x = (float)x / (float)(m_cxDIB - 1);
			v.t.y = (float)z / (float)(m_czDIB - 1);
			m_pvHeightMap[x + z*m_czDIB] = v;
		}
	}
}

void cTerrain::BuildQuadTree()
{
	m_pQuadTree = new cQuadTree(TERRAIN_SIZE, TERRAIN_SIZE);
	m_pQuadTree->Build(m_pvHeightMap);
}

void cTerrain::CreateVIB()
{
	g_pD3DDevice->CreateVertexBuffer(
		m_cxDIB*m_czDIB *sizeof(ST_PNT_VERTEX),
		0, ST_PNT_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL);

	LPVOID pVertices;
	m_pVB->Lock(0, m_cxDIB*m_czDIB*sizeof(ST_PNT_VERTEX),
		(LPVOID*)&pVertices, 0);
	memcpy(pVertices, m_pvHeightMap, m_cxDIB*m_czDIB*sizeof(ST_PNT_VERTEX));
	m_pVB->Unlock();

	g_pD3DDevice->CreateIndexBuffer(
		(m_cxDIB - 1) * (m_czDIB - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX32,
		D3DPOOL_DEFAULT, &m_pIB, NULL);
}

void cTerrain::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetIndices(m_pIB);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_cxDIB*m_czDIB, 0, m_nTriangles);




#ifdef SHOW_ALGORITHM_HYUNJAE
	//쿼드트리 잘라진 사각형들 보여주기용
	m_pQuadTree->Render();

	//픽킹그라운드 보여주기용
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 2, m_vPickingGround, sizeof(ST_PC_VERTEX));
#endif
}

void cTerrain::CreatePickingGround()
{
	
	m_vPickingGround[0].p = D3DXVECTOR3(-TERRAIN_SIZE, 0, TERRAIN_SIZE);
	m_vPickingGround[1].p = D3DXVECTOR3(TERRAIN_SIZE, 0, -TERRAIN_SIZE);
	m_vPickingGround[2].p = D3DXVECTOR3(-TERRAIN_SIZE, 0, -TERRAIN_SIZE);

	m_vPickingGround[3].p = D3DXVECTOR3(-TERRAIN_SIZE, 0, TERRAIN_SIZE);
	m_vPickingGround[4].p = D3DXVECTOR3(TERRAIN_SIZE, 0, TERRAIN_SIZE);
	m_vPickingGround[5].p = D3DXVECTOR3(TERRAIN_SIZE, 0, -TERRAIN_SIZE);

	for (int i = 0; i < 6; i++)
		m_vPickingGround[i].c = D3DXCOLOR(0.4f, 0.6f, 0.4f, 1.0f);
}
