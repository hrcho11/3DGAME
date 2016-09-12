#include "stdafx.h"
#include "cShadowManager.h"

#define SHADOWMAP_SIZE 2048

cShadowManager::cShadowManager()
	: m_pHWBackBuffer(NULL)
	, m_pHWDepthStencilBuffer(NULL)
	, m_pShadowDepthStencil(NULL)
	, m_pShadowRenderTarget(NULL)
	, m_vLightColor(0.f, 0.f, 0.f, 1.f)
	, m_vLightPosition(500.0f, 500.0f, 500.0f, 1.0f)
	, m_vLightDirection(1.0f, -1.0f, 1.0f)
{
	D3DXCreateTexture(g_pD3DDevice,
		SHADOWMAP_SIZE,
		SHADOWMAP_SIZE,
		1, 
		D3DUSAGE_RENDERTARGET, 
		D3DFMT_R32F,
		D3DPOOL_DEFAULT, 
		&m_pShadowRenderTarget);

	g_pD3DDevice->CreateDepthStencilSurface(SHADOWMAP_SIZE,
		SHADOWMAP_SIZE,
		D3DFMT_D24X8, 
		D3DMULTISAMPLE_NONE, 
		0, 
		TRUE, 
		&m_pShadowDepthStencil, 
		NULL);

	D3DXVECTOR3 vLightPos, vLightAt, vUp;
	vLightPos = D3DXVECTOR3(m_vLightDirection.x * m_vLightPosition.x, 
		m_vLightDirection.y * m_vLightPosition.y, 
		m_vLightDirection.z * m_vLightPosition.z);
	vLightAt = D3DXVECTOR3(0, 0, 0);
	vUp = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&m_matLightView, &vLightPos, &vLightAt, &vUp);

	D3DXMatrixPerspectiveFovLH(&m_matLightProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f);

	g_pD3DDevice->GetRenderTarget(0, &m_pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&m_pHWDepthStencilBuffer);
}


cShadowManager::~cShadowManager()
{
}

void cShadowManager::Begin()
{
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		SAFE_RELEASE(pShadowSurface);
	}
	g_pD3DDevice->SetDepthStencilSurface(m_pShadowDepthStencil);

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
}

void cShadowManager::End()
{
	g_pD3DDevice->SetRenderTarget(0, m_pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(m_pHWDepthStencilBuffer);
}

void cShadowManager::Destroy()
{
	SAFE_RELEASE(m_pHWBackBuffer);
	SAFE_RELEASE(m_pHWDepthStencilBuffer);
	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pShadowDepthStencil);
}

D3DXMATRIXA16 * cShadowManager::GetLightViewMatrix()
{
	return &m_matLightView;
}

D3DXMATRIXA16 * cShadowManager::GetLightProjMatrix()
{
	return &m_matLightProj;
}

D3DXVECTOR4 * cShadowManager::GetLightColor()
{
	return &m_vLightColor;
}

D3DXVECTOR3 * cShadowManager::GetLightDirection()
{
	return &m_vLightDirection;
}
