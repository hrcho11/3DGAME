#include "StdAfx.h"
#include "cLinearInterpolation.h"
#include "cCube.h"
#include "cAction.h"
#include "cActionMove.h"
#include "cActionSeq.h"
#include "cActionRepeat.h"

cLinearInterpolation::cLinearInterpolation(void)
	: m_pCube(NULL)
{
}

cLinearInterpolation::~cLinearInterpolation(void)
{
	SAFE_RELEASE(m_pCube);
}

void cLinearInterpolation::Setup()
{
	m_pCube = new cCube;
	m_pCube->Setup(NULL);
	m_pCube->SetMtlColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	
	cActionSeq* pActionSeq = new cActionSeq;

	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, D3DX_PI / 3.0f);

	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 0);
	D3DXVECTOR3 p = D3DXVECTOR3(10, 0, 0);
	
	for (int i = 0; i < 6; ++i)
	{
		cActionMove* pActionMove = new cActionMove;

		m_vecLineVertex.push_back(ST_PC_VERTEX(p, c));
		pActionMove->SetFrom(p);

		D3DXVec3TransformCoord(&p, &p, &matR);

		m_vecLineVertex.push_back(ST_PC_VERTEX(p, c));
		pActionMove->SetTo(p);
		
		pActionMove->SetActionTime(1);
		pActionMove->AutoRelease();
		pActionMove->SetTargetObject(m_pCube);

		pActionSeq->AddAction(pActionMove);
	}

	cActionRepeat* pActionRepeat = new cActionRepeat;
	pActionSeq->SetDelegate(pActionRepeat);
	pActionRepeat->SetAction(pActionSeq);
	pActionRepeat->AutoRelease();

	m_pCube->SetAction(pActionRepeat);
	pActionRepeat->Start();
	pActionSeq->AutoRelease();
}

void cLinearInterpolation::Update()
{
	if (m_pCube)
	{
		m_pCube->Update();
	}
}

void cLinearInterpolation::Render()
{
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);

	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 
		m_vecLineVertex.size() / 2,
		&m_vecLineVertex[0],
		sizeof(ST_PC_VERTEX));

	if(m_pCube)
		m_pCube->Render(NULL);
}
