#include "StdAfx.h"
#include "cHeightMap.h"

#define TILE_N 256
#define VERTEX_DIM 257

cHeightMap::cHeightMap(void)
	: m_pTexture(NULL)
	, m_pMesh(NULL)
{
}

cHeightMap::~cHeightMap(void)
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
}

void cHeightMap::Load( char* szFullPath, D3DXMATRIXA16* pmat )
{
	std::vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.resize( VERTEX_DIM * VERTEX_DIM );
	m_vecVertex.resize( VERTEX_DIM * VERTEX_DIM );

	FILE* fp = NULL;

	fopen_s(&fp, szFullPath, "rb");

	for (int z = 0; z < VERTEX_DIM; ++z)
	{
		for (int x = 0; x < VERTEX_DIM; ++x)
		{
			int nIndex = z * (VERTEX_DIM) + x;
			float y = ((unsigned char)fgetc(fp)) / 10.0f;
			vecVertex[nIndex].p = D3DXVECTOR3(x, y, z);
			vecVertex[nIndex].n = D3DXVECTOR3(0, 1, 0);
			vecVertex[nIndex].t = D3DXVECTOR2(x / (float)TILE_N, z / (float)TILE_N);

			m_vecVertex[nIndex] = vecVertex[nIndex].p;
		}
	}

	fclose(fp);

	std::vector<DWORD> vecIndex;
	vecIndex.reserve(TILE_N * TILE_N * 2 * 3);

	for (int z = 0; z < TILE_N; ++z)
	{
		for (int x = 0; x < TILE_N; ++x)
		{
			// 1--3
			// |\ |
			// | \|
			// 0--2
			int _0 = (z + 0) * VERTEX_DIM + x + 0;
			int _1 = (z + 1) * VERTEX_DIM + x + 0;
			int _2 = (z + 0) * VERTEX_DIM + x + 1;
			int _3 = (z + 1) * VERTEX_DIM + x + 1;

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);
			vecIndex.push_back(_3);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
		}
	}

	for (int z = 1; z < TILE_N; ++z)
	{
		for (int x = 1; x < TILE_N; ++x)
		{
			// ---u---
			// |\ |\ |
			// | \| \|
			// l--n--r
			// |\ |\ |
			// | \| \|
			// ---d---
			int nIndex = z * VERTEX_DIM + x;
			
			int l = nIndex - 1;
			int r = nIndex + 1;
			int u = nIndex + VERTEX_DIM;
			int d = nIndex - VERTEX_DIM;

			D3DXVECTOR3 du = m_vecVertex[u] - m_vecVertex[d];
			D3DXVECTOR3 lr = m_vecVertex[r] - m_vecVertex[l];
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);
			
			vecVertex[nIndex].n = n;
		}
	}

	D3DXCreateMeshFVF(vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuf(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuf[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuf[0], 0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
}

bool cHeightMap::GetHeight( IN const float& x, OUT float& y, IN const float& z )
{
	if (x < 0 || z < 0 || x > TILE_N || z > TILE_N)
	{
		y = 0.0f;
		return false;
	}

	// 1--3
	// |\ |
	// | \|
	// 0--2
	int nX = (int)x;
	int nZ = (int)z;
	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;

	int _0 = (nZ + 0) * VERTEX_DIM + nX + 0;
	int _1 = (nZ + 1) * VERTEX_DIM + nX + 0;
	int _2 = (nZ + 0) * VERTEX_DIM + nX + 1;
	int _3 = (nZ + 1) * VERTEX_DIM + nX + 1;

	if(fDeltaX + fDeltaZ < 1.0f)
	{
		D3DXVECTOR3 v01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 v02 = m_vecVertex[_2] - m_vecVertex[_0];
		y = m_vecVertex[_0].y + (v01 * fDeltaZ + v02 * fDeltaX).y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;

		D3DXVECTOR3 v31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 v32 = m_vecVertex[_2] - m_vecVertex[_3];
		y = m_vecVertex[_3].y + (v31 * fDeltaX + v32 * fDeltaZ).y;
	}
	return true;
}

void cHeightMap::Render()
{
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);
}
