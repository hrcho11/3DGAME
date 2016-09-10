#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cSkinnedMesh.h"
#include "cTerrain.h"
cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pSkinnedMesh(NULL)
	, m_pTerrain(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pTerrain);
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Initialize();

	m_pTerrain = new cTerrain;
	m_pTerrain->Create();

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if(m_pCamera)
		m_pCamera->Update(&D3DXVECTOR3(0,0,0));
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	m_pTerrain->Draw(m_pCamera->GetFrustum());

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
		m_pCamera->WindowInputProc(message, wParam, lParam);

	switch(message)
	{
	case WM_KEYDOWN:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
			
		}
		break;
	case WM_RBUTTONDOWN:
		{
			
		}
		break;
	}
}


