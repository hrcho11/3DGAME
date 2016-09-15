#include "stdafx.h"
#include "cLava.h"

const D3DXVECTOR3	WaterCenter(0.0f, -1.0f, 0.0f);

inline bool ToleranceLE(float a, float b, float tolenrance = 0.001f)
{
	return ((a - tolenrance) <= b);
}

cLava::cLava()
{
	memset(m_fWaveTable, 0, (sizeof(m_fWaveTable)));

	m_pVertex = nullptr;
	m_wIndex = nullptr;

	m_nVertexCount = 0;
	m_nIndexCount = 0;
	m_nTesselation = 0;

	m_pTexture = g_pTextureManager->GetTexture("Data/Map/lava.jpg");
}


cLava::~cLava()
{
	if (m_pVertex)
		SAFE_DELETE_ARRAY(m_pVertex);

	if (m_wIndex)
		SAFE_DELETE_ARRAY(m_wIndex);
}

bool cLava::Init()
{
	float angInc = 360.0f / (float)nWaveTableSize;
	float angle = 0.0f;

	for (int i = 0; i < nWaveTableSize; ++i)
	{
		m_fWaveTable[i] = sin(DEG2RAD(angle));
		angle += angInc;
	}

	if (!CreateGeometry(fWaterWidth, fWaterHeight, nWaterTesslation))
		return false;

	return true;

}

void cLava::Process()
{
	static int startIndex = 0;

	int waveIndex = startIndex;
	int vertIndex = 0;

	for (int i = 0; i <= m_nTesselation; ++i)
	{
		for (int j = 0; j <= m_nTesselation; ++j, ++vertIndex)
		{
			m_pVertex[vertIndex].p.y += m_fWaveTable[waveIndex] * fWaterForce;

			if (++waveIndex == nWaveTableSize)
				waveIndex = 0;
		}
	}

	if (++startIndex == nWaveTableSize)
		startIndex = 0;
}

void cLava::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PCT_VERTEX::FVF);

	g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_nVertexCount, m_nIndexCount / 3, m_wIndex, D3DFMT_INDEX16, m_pVertex, sizeof(ST_PCT_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool cLava::CreateGeometry(float width, float height, int tesselation)
{
	if (width < 0.0f || height < 0.0f || tesselation <= 0)
		return false;

	m_nTesselation = tesselation;

	m_nVertexCount = (m_nTesselation + 1) * (m_nTesselation + 1);

	m_nIndexCount = m_nTesselation * m_nTesselation * 6;

	m_pVertex = new ST_PCT_VERTEX[m_nVertexCount];
	m_wIndex = new WORD[m_nIndexCount];

	float subDivideWid = width * (1.0f / (float)m_nTesselation);
	float subDivideHeight = height * (1.0f / (float)m_nTesselation);

	float startX = WaterCenter.x - (width / 2.0f);
	float startY = WaterCenter.z - (height / 2.0f);
	float endX = WaterCenter.x + (width / 2.0f);
	float endY = WaterCenter.z + (height / 2.0f);

	float u = 0.0f;
	float v = 1.0f;
	float uvInc = 1.0f / (float)m_nTesselation;

	int index = 0;

	for (float y = startY; ToleranceLE(y, endY); y += subDivideHeight)
	{
		for (float x = startX; ToleranceLE(x, endX); x += subDivideWid, ++index)
		{
			m_pVertex[index].p.x = x;
			m_pVertex[index].p.y = WaterCenter.y;
			m_pVertex[index].p.z = y;
			m_pVertex[index].t.x = u;
			m_pVertex[index].t.y = v;
			m_pVertex[index].c = D3DCOLOR_ARGB(128, 255, 255, 255);

			u += uvInc;
		}

		u = 0.0f;
		v -= uvInc;
	}

	assert(m_nVertexCount == index);

	int indexValue = 0;
	int nextRow = 0;

	for (index = 0; index < m_nIndexCount; index += 6)
	{
		m_wIndex[index] = indexValue;
		m_wIndex[index + 1] = indexValue + (m_nTesselation + 1);
		m_wIndex[index + 2] = m_wIndex[index] + 1;

		indexValue++;

		m_wIndex[index + 3] = indexValue;
		m_wIndex[index + 4] = indexValue + m_nTesselation;
		m_wIndex[index + 5]  = m_wIndex[index + 4] + 1;

		if (++nextRow == m_nTesselation)
		{
			indexValue++;
			nextRow = 0;
		}
	}

	return true;
}
