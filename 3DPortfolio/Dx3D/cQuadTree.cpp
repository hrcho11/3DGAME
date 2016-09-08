#include "stdafx.h"
#include "cQuadTree.h"



cQuadTree::cQuadTree()
{
}

cQuadTree::cQuadTree(int cx, int cy)
{
	m_nCenter = 0;
	for (int i = 0; i < 4; i++)
	{
		m_pNeighbor[i] = nullptr;
		m_pChild[i] = nullptr;
	}
		

	m_nCorner[CORNER_TL] = 0;
	m_nCorner[CORNER_TR] = cx -1;
	m_nCorner[CORNER_BL] = cx * (cy-1);
	m_nCorner[CORNER_BR] = cx *cy -1;
	m_nCenter =
		(m_nCorner[CORNER_TL] +
			m_nCorner[CORNER_TR] +
			m_nCorner[CORNER_BL] +
			m_nCorner[CORNER_BR]) / 4;

	m_fRadius = 0.0f;
	m_bCulled = false;
	m_pMesh = nullptr;
	
}

cQuadTree::cQuadTree(cQuadTree * pParent)
{
	int i = 0;
	m_nCenter = 0;
	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = nullptr;
		m_nCorner[i] = 0;
		m_pNeighbor[i] = nullptr;
	}
	
	m_bCulled = false;
	m_fRadius = 0.0f;


	m_pMesh = nullptr;
}


cQuadTree::~cQuadTree()
{

}
void cQuadTree::Destroy()
{
	SAFE_RELEASE(m_pMesh);
	for (int i = 0; i < 4; i++)
		SAFE_DESTROY(m_pChild[i]);
}



bool cQuadTree::SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;

	m_nCenter =
		(m_nCorner[CORNER_TL] +
			m_nCorner[CORNER_TR] +
			m_nCorner[CORNER_BL] +
			m_nCorner[CORNER_BR]) / 4;
	return true;
}



cQuadTree * cQuadTree::AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	cQuadTree*		pChild;

	pChild = new cQuadTree(this);
	pChild->SetCorners(nCornerTL, nCornerTR, nCornerBL, nCornerBR);
	
	return pChild;
}

bool cQuadTree::SubDivide()
{
	int nTopEdgeCenter;
	int nBottomEdgeCenter;
	int nLeftEdgeCenter;
	int nRightEdgeCenter;
	int nCentralPoint;

	nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;

	nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;

	nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;

	nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;

	nCentralPoint = (m_nCorner[CORNER_TL] +
		m_nCorner[CORNER_TR] +
		m_nCorner[CORNER_BL] +
		m_nCorner[CORNER_BR]) / 4;

	D3DXCreateBox(
		g_pD3DDevice,
		m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL],
		2,
		m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL],
		&m_pMesh,
		NULL);

	if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
		return false;

	m_pChild[CORNER_TL] = AddChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);
	m_pChild[CORNER_TR] = AddChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);
	m_pChild[CORNER_BL] = AddChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);
	m_pChild[CORNER_BR] = AddChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

	return true;
}

int cQuadTree::GenTriIndex(int nTriangles, LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, cFrustumCulling* pFrustum, float fLODRatio)
{
	if (m_bCulled)
	{
		m_bCulled = false;
		return nTriangles;
	}

	if (IsVisible(pHeightMap,pFrustum->GetPos(),fLODRatio))
	{	
		LPDWORD p = ((LPDWORD)pIndex) + nTriangles * 3;
		if (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1)
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTriangles++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];

			nTriangles++;

			return nTriangles;
		}

		bool b[4] = { 0,0,0,0 };

		if (m_pNeighbor[EDGE_UP])
			b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
		if (m_pNeighbor[EDGE_DN])
			b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
		if (m_pNeighbor[EDGE_LT])
			b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);
		if (m_pNeighbor[EDGE_RT])
			b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible(pHeightMap, pFrustum->GetPos(), fLODRatio);

		if (b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT])
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTriangles++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];

			nTriangles++;

			return nTriangles;
		}

		int n;

		if (!b[EDGE_UP])
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_TL];
			*p++ = n;
			nTriangles++;
			*p++ = m_nCenter;
			*p++ = n;
			*p++ = m_nCorner[CORNER_TR];
			nTriangles++;
		}
		else
		{
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_TL];
			*p++ = m_nCorner[CORNER_TR]; 
			nTriangles++;
		}

		if (!b[EDGE_DN])
		{
			n = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR])/2;
			*p++ = m_nCenter; 
			*p++ = m_nCorner[CORNER_BR];
			*p++ = n;
			nTriangles++;

			*p++ = m_nCenter;
			*p++ = n;
			*p++ = m_nCorner[CORNER_BL];
			nTriangles++;
		}
		else
		{
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_BR];
			*p++ = m_nCorner[CORNER_BL];
			nTriangles++;
		}
		if (!b[EDGE_LT])
		{
			n = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_BL];
			*p++ = n;
			nTriangles++;
			*p++ = m_nCenter;
			*p++ = n;
			*p++ = m_nCorner[CORNER_TL];
			nTriangles++;
		}
		else
		{
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_BL];
			*p++ = m_nCorner[CORNER_TL];
			nTriangles++;
		}
		if (!b[EDGE_RT])
		{
			n = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_TR];
			*p++ = n;
			nTriangles++;
			*p++ = m_nCenter;
			*p++ = n;
			*p++ = m_nCorner[CORNER_BR];
			nTriangles++;
		}
		else
		{
			*p++ = m_nCenter;
			*p++ = m_nCorner[CORNER_TR];
			*p++ = m_nCorner[CORNER_BR];
			nTriangles++;
		}

		return nTriangles;
	}

	if (m_pChild[CORNER_TL])
		nTriangles = m_pChild[CORNER_TL]->GenTriIndex(nTriangles, pIndex,pHeightMap,pFrustum,fLODRatio);
	if (m_pChild[CORNER_TR])
		nTriangles = m_pChild[CORNER_TR]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BL])
		nTriangles = m_pChild[CORNER_BL]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);
	if (m_pChild[CORNER_BR])
		nTriangles = m_pChild[CORNER_BR]->GenTriIndex(nTriangles, pIndex, pHeightMap, pFrustum, fLODRatio);

	
	return nTriangles;
}

int cQuadTree::IsInFrustum(ST_PNT_VERTEX * pHeightMap, cFrustumCulling * pFrustum)
{
	bool		b[4];
	bool		bInSphere;

	bInSphere = pFrustum->IsInSphere(
		(D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);
	if (!bInSphere)return FRUSTUM_OUT;

	b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
	b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
	b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
	b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

	if ((b[0] + b[1] + b[2] + b[3]) == 4) return FRUSTUM_COMPLETELY_IN;

	return FRUSTUM_PARTIALLY_IN;
}


void cQuadTree::AllInFrustum()
{
	m_bCulled = false;
	if (!m_pChild[0])return;
	m_pChild[0]->AllInFrustum();
	m_pChild[1]->AllInFrustum();
	m_pChild[2]->AllInFrustum();
	m_pChild[3]->AllInFrustum();
}



void cQuadTree::FrustumCull(ST_PNT_VERTEX * pHeightMap, cFrustumCulling * pFrstum)
{
	int ret;

	m_bCulled = false;
	ret = IsInFrustum(pHeightMap, pFrstum);

	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN :
		m_bCulled = false;
		return;
	case FRUSTUM_PARTIALLY_IN:
		m_bCulled = false;
		break;
	case FRUSTUM_OUT:
		m_bCulled = true;
		return;
	}

	if (m_pChild[0])
		m_pChild[0]->FrustumCull(pHeightMap, pFrstum);
	if (m_pChild[1])
		m_pChild[1]->FrustumCull(pHeightMap, pFrstum);
	if (m_pChild[2])
		m_pChild[2]->FrustumCull(pHeightMap, pFrstum);
	if (m_pChild[3])
		m_pChild[3]->FrustumCull(pHeightMap, pFrstum);
}



int cQuadTree::GetNodeIndex(int ed, int cx, int & _0, int & _1, int & _2, int & _3)
{
	int n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _b - _a;

	switch (ed)
	{
	case EDGE_UP:
		_0 = _a - cx * gap;
		_1 = _b - cx *gap;
		_2 = _a;
		_3 = _b;
		break;
	case EDGE_DN:
		_0 = _c;
		_1 = _d;
		_2 = _c + cx* gap;
		_3 = _d + cx *gap;
		break;
	case EDGE_LT:
		_0 = _a - gap;
		_1 = _a;
		_2 = _c - gap;
		_3 = _c;
		break;
	case EDGE_RT:
		_0 = _b;
		_1 = _b + gap;
		_2 = _d;
		_3 = _d + gap;
		if ((_0 / cx) != (_a / cx)) return -1;
		if ((_1 / cx) != (_b / cx)) return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;
	if (!IsInRange(n, 0, cx*cx - 1)) return -1;
	return n;
}


cQuadTree * cQuadTree::FindNode(ST_PNT_VERTEX * pHeightMap, int _0, int _1, int _2, int _3)
{

	cQuadTree* p = nullptr;
	if ((m_nCorner[0] == _0) && m_nCorner[1] == _1 && (m_nCorner[2] == _2 &&
		m_nCorner[3] == _3))
		return this;

	if (m_pChild[0])
	{
		RECT rc;
		POINT pt;
		int n = (_0 + _1 + _2 + _3) / 4;

		pt.x = (int)pHeightMap[n].p.x;
		pt.y = (int)pHeightMap[n].p.z;

		SetRect(&rc,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.z);

		if (PtInRect(&rc, pt))
		{
			return m_pChild[0]->FindNode(pHeightMap, _0, _1, _2, _3);
		}

		SetRect(&rc,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.z);

		if (PtInRect(&rc, pt))
		{
			return m_pChild[1]->FindNode(pHeightMap, _0, _1, _2, _3);
		}

		SetRect(&rc,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.z);

		if (PtInRect(&rc, pt))
		{
			return m_pChild[2]->FindNode(pHeightMap, _0, _1, _2, _3);
		}

		SetRect(&rc,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.x,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.z,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.x,
			(int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.z);

		if (PtInRect(&rc, pt))
		{
			return m_pChild[3]->FindNode(pHeightMap, _0, _1, _2, _3);
		}
	}
	return nullptr;
}

void cQuadTree::BuildNeighborNode(cQuadTree * pRoot, ST_PNT_VERTEX * pHeightMap, int cx)
{
	int n;
	int _0, _1, _2, _3;

	for (int i = 0; i < 4; ++i)
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];

		n = GetNodeIndex(i, cx, _0, _1, _2, _3);
		
		if (n >= 0)
			m_pNeighbor[i] = pRoot->FindNode(pHeightMap, _0, _1, _2, _3);
	}

	if (m_pChild[0])
	{
		m_pChild[0]->BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[1]->BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[2]->BuildNeighborNode(pRoot, pHeightMap, cx);
		m_pChild[3]->BuildNeighborNode(pRoot, pHeightMap, cx);
	}
}

bool cQuadTree::BuildQuadTree(ST_PNT_VERTEX * pHeightMap)
{
	if (SubDivide())
	{
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_TL])) -
			*((D3DXVECTOR3*)(pHeightMap + m_nCorner[CORNER_BR]));

		m_fRadius = D3DXVec3Length(&v) / 2.0f;
		m_pChild[CORNER_TL]->BuildQuadTree(pHeightMap);
		m_pChild[CORNER_TR]->BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BL]->BuildQuadTree(pHeightMap);
		m_pChild[CORNER_BR]->BuildQuadTree(pHeightMap);
	}
	return true;
}

void cQuadTree::Render()
{
	if (m_pMesh)
	{
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DXMATRIXA16 mat;
		float x =  ((m_nCorner[CORNER_TL]%33) + (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] - 32.f) *0.5f);
		float z = (m_nCorner[CORNER_TL] + (m_nCorner[CORNER_BL] - m_nCorner[CORNER_TL] - 1024.f) *0.5f) / 32.f;
		D3DXMatrixTranslation(&mat,
			x,
			0,
			z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
		m_pMesh->DrawSubset(0);
	}
	if (m_pChild[0])
		m_pChild[0]->Render();
	if (m_pChild[1])
		m_pChild[1]->Render();
	if (m_pChild[2])
		m_pChild[2]->Render();
	if (m_pChild[3])
		m_pChild[3]->Render();
}

bool cQuadTree::Build(ST_PNT_VERTEX* pHeightMap)
{
	BuildQuadTree(pHeightMap);
	BuildNeighborNode(this, pHeightMap, m_nCorner[CORNER_TR] + 1);
	return true;
}

int cQuadTree::GenerateIndex(LPVOID pIndex, ST_PNT_VERTEX* pHeightMap, cFrustumCulling* pFrustum,float fLODRatio)
{
	FrustumCull(pHeightMap, pFrustum);
	return GenTriIndex(0, pIndex,pHeightMap,pFrustum,fLODRatio);
}
