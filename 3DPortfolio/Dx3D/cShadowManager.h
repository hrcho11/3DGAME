#pragma once

#define g_pShadowManager cShadowManager::GetInstance()

class cShadowManager
{
	SINGLETONE(cShadowManager);
private:
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	LPDIRECT3DSURFACE9		m_pHWBackBuffer;
	LPDIRECT3DSURFACE9		m_pHWDepthStencilBuffer;
	D3DXMATRIXA16			m_matLightView;
	D3DXMATRIXA16			m_matLightProj;
	D3DXVECTOR4				m_vLightColor;
	D3DXVECTOR4				m_vLightPosition;
	D3DXVECTOR3				m_vLightDirection;
public:
	void Begin();
	void End();

	void Destroy();

	D3DXMATRIXA16*	GetLightViewMatrix();
	D3DXMATRIXA16*	GetLightProjMatrix();
	D3DXVECTOR4*	GetLightColor();
	D3DXVECTOR3*	GetLightDirection();
};

