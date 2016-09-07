#pragma once
#include "cFrustumCulling.h"

struct TRIINDEX
{
	DWORD _0, _1, _2;
};

class cQuadTree
{
	enum eCornorType
	{
		CORNER_TL,
		CORNER_TR,
		CORNER_BL,
		CORNER_BR
	};

	enum
	{
		EDGE_UP,
		EDGE_DN,
		EDGE_LT,
		EDGE_RT
	};

	enum QuadLocation
	{
		FRUSTUM_OUT = 0,
		FRUSTUM_PARTIALLY_IN = 1,
		FRUSTUM_COMPLETELY_IN = 2,
		FRUSTUM_UNKNOWN = -1
	};
private:
	cQuadTree*		m_pChild[4];

	int					m_nCenter;
	int					m_nCorner[4];

	bool				m_bCulled;
	float				m_fRadius;

	cQuadTree*		m_pParent;
	cQuadTree*		m_pNeighbor[4];


	LPD3DXMESH			m_pMesh; //쿼드트리 사각형 나눈거 보여주기용
	bool IsInRange(int value, int r0, int r1)
	{
		return ((((r0) <= (value)) && ((value) <= (r1))) ? 1 : 0);
	}
private:
	cQuadTree*		AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);
	bool				SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);
	bool				SubDivide();

	bool				IsVisible(ST_PNT_VERTEX* pHeightMap,D3DXVECTOR3* pCamera,float fLODRatio)
	{ return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= GetLODLevel(pHeightMap,pCamera,fLODRatio)); }

	int					GenTriIndex(int nTriangles, LPVOID pIndex, ST_PNT_VERTEX* pHeightMap,cFrustumCulling* pFrustum, float fLODRatio);

	int					IsInFrustum(ST_PNT_VERTEX* pHeightMap, cFrustumCulling* pFrusum);
	void				FrustumCull(ST_PNT_VERTEX* pHeightMap, cFrustumCulling* pFrstum);

	float				GetDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2) { return D3DXVec3Length(&(*pv2 - *pv1)); }
	int					GetLODLevel(ST_PNT_VERTEX* pHeightMap, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		float d = GetDistance((D3DXVECTOR3*)(pHeightMap + m_nCenter), pCamera);
		return max((int)(d * fLODRatio), 1);
	}

	void				AllInFrustum();
	void				BuildNeighborNode(cQuadTree* pRoot, ST_PNT_VERTEX* pHeightMap, int cx);
	bool				BuildQuadTree(ST_PNT_VERTEX* pHeightMap);
	cQuadTree*		FindNode(ST_PNT_VERTEX* pHeightMap, int _0, int _1, int _2, int _3);
	int					GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
	
public:
	cQuadTree();
	cQuadTree(int cx, int cy);
	cQuadTree(cQuadTree* pParent);
	~cQuadTree();
	void				Destroy();
	bool				Build(ST_PNT_VERTEX* pHeightMap);
	int					GenerateIndex(LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, cFrustumCulling* pFrustum, float fLODRatio);
	void				Render();
	bool				IsCulled() {return m_bCulled;}
	void				GetCorner(int& _0, int& _1, int& _2, int& _3)
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];
	}
};

