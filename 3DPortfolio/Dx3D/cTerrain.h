#pragma once
#include "cQuadTree.h"

#define TERRAIN_SIZE 65
class cTerrain
{
private:
	int						m_cxDIB;
	int						m_czDIB;
	int						m_cyDIB;

	D3DXVECTOR3				m_vfScale;
	ST_PNT_VERTEX*			m_pvHeightMap;
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	int						m_nTriangles;
	cQuadTree*				m_pQuadTree;
	float					m_fLODRatio;
	D3DXMATRIXA16			m_matWorld;

	ST_PC_VERTEX		m_vPickingGround[6];
public:

	void Create();
	void Destroy();
private:
	
	void LoadTexture(char* lpTexFileName);
	void BuildTerrain();
	void BuildQuadTree();
	void CreateVIB();
	void Render();

	void CreatePickingGround();
public:
	cTerrain();
	~cTerrain();
	
	ST_PNT_VERTEX*	GetVertex(int x, int z) { return (m_pvHeightMap + x + z *m_cxDIB); }
	float			GetHeight(int x, int z) { return (GetVertex(x, z))->p.y; }
	int				GetCXDIB() { return m_cxDIB; }
	int				GetCZDIB() { return m_czDIB; }
	float			GetCXTerrain() { return m_cxDIB* m_vfScale.x; }
	float			GetCYTerrain() { return m_cyDIB* m_vfScale.y; }
	float			GetCZTerrain() { return m_czDIB * m_vfScale.z; }
	
	ST_PC_VERTEX*		GetPickingGround() { return m_vPickingGround; }

	void Draw(cFrustumCulling* pFrustum);
};

