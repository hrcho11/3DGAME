#include "StdAfx.h"
#include "cUIObject.h"


cUIObject::cUIObject(void)
	: m_vLocalPos(0, 0, 0)
	, m_pParent(NULL)
	, m_stSize(0, 0)
	, m_isDebugRender(false)
	, m_nTag(0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cUIObject::~cUIObject(void)
{
	//SAFE_RELEASE(m_pObject);
}

void cUIObject::Update()
{
	UpdateWorldTM();
	UpdateChildren();
}

void cUIObject::Render( LPD3DXSPRITE pSprite )
{
	if (m_isDebugRender)
	{
		float l = m_matWorld._41;
		float t = m_matWorld._42;
		float r = m_matWorld._41 + m_stSize.fWidth;
		float b = m_matWorld._42 + m_stSize.fHeight;

		D3DCOLOR c = D3DCOLOR_XRGB(255, 0, 0);
		std::vector<ST_RHWC_VERTEX> vecVertex;
		vecVertex.reserve(8);
		
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, t, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, t, 0, 1), c));

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, t, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, b, 0, 1), c));

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, b, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, b, 0, 1), c));

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, b, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, t, 0, 1), c));

		g_pD3DDevice->SetFVF(ST_RHWC_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecVertex[0], sizeof(ST_RHWC_VERTEX));
	}

	for each(auto p in m_vecChild)
	{
		p->Render(pSprite);
	}
}

void cUIObject::AddChild( cUIObject* pChild )
{
	pChild->m_pParent = this;
	m_vecChild.push_back(pChild);
}

void cUIObject::UpdateWorldTM()
{
	m_matWorld._41 = m_vLocalPos.x;
	m_matWorld._42 = m_vLocalPos.y;
	m_matWorld._43 = m_vLocalPos.z;

	if(m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}
}

void cUIObject::UpdateChildren()
{
	for each(auto p in m_vecChild)
	{
		p->Update();
	}
}

void cUIObject::Destroy()
{
	for each(auto p in m_vecChild)
	{
		p->Destroy();
	}
	Release();
}

cUIObject* cUIObject::GetChildByTag( int nTag )
{
	if (m_nTag == nTag)
		return this;

	for each(auto p in m_vecChild)
	{
		cUIObject* pChild = p->GetChildByTag(nTag);
		if(pChild)
			return pChild;
	}
	return NULL;
}
