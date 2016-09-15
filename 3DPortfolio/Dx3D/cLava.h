#pragma once

const int nWaveTableSize = 36;
const int nWaterTesslation = 32;
const float fWaterWidth = 128.0f;
const float fWaterHeight = 128.0f;
const float fWaterForce = 1.0f / 60.0f;
#define DEG2RAD(x) (x * (D3DX_PI / 180.0f))
class cLava
{
public:
	cLava();
	~cLava();

	bool Init();
	void Process();
	void Render();
private:
	bool CreateGeometry(float width, float height, int tesselation);

	LPDIRECT3DTEXTURE9		m_pTexture;
	ST_PCT_VERTEX*			m_pVertex;
	WORD*							m_wIndex;

	int								m_nVertexCount;
	int								m_nIndexCount;
	int								m_nTesselation;

	float							m_fWaveTable[nWaveTableSize];
};

