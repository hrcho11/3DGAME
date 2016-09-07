#include "StdAfx.h"
#include "cMainGame.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cCube.h"
#include "cCharactorController.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "iMap.h"
#include "cObjMap.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cAseCharacter.h"
#include "cMtlTex.h"
#include "cHeightMap.h"
#include "cRay.h"
#include "cSkinnedMesh.h"
#include "cFrustum.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pCharactorContoller(NULL)
	, m_pSkinnedMesh(NULL)
	, m_pFrustum(NULL)
{
	//D3DXPLANE
	//D3DXPlaneFromPoints(아웃풋, 점1, 점2, 점3);
	//거리 = D3DXPlaneDotCoord(평면, 점)
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharactorContoller);
	SAFE_DELETE(m_pFrustum);

	for each(auto p in m_vecSkinnedMesh)
	{
		SAFE_DELETE(p);
	}

	g_pSkinnedMeshManager->Destroy();
	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pObjectManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	for (int z = -20; z <= 20; ++z)
	{
		for (int x = -20; x <= 20; ++x)
		{
			cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh("./Zealot/", "zealot.X");
			pSkinnedMesh->SetRandomTrackPosition();
			pSkinnedMesh->SetPosition(D3DXVECTOR3(x, 0, z));
			m_vecSkinnedMesh.push_back(pSkinnedMesh);
		}
	}

	m_pFrustum = new cFrustum();

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup();

	SetLight();

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if(m_pFrustum)
		m_pFrustum->Update();

	if(m_pCharactorContoller)
		m_pCharactorContoller->Update(NULL);

	if(m_pCamera)
		m_pCamera->Update(m_pCharactorContoller->GetPosition());
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
	if (m_pGrid)
		m_pGrid->Render();

	for(size_t i = 0; i < m_vecSkinnedMesh.size(); ++i)
	{
		if(GetKeyState(VK_SPACE) & 0x8000)
		{
			m_pFrustum->IsInFrustum(m_vecSkinnedMesh[i]->GetBoundingSphere());
		}

		if (m_vecSkinnedMesh[i]->GetBoundingSphere()->isInFrustum)
		{
			m_vecSkinnedMesh[i]->UpdateAndRender();
		}
	}

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);

	switch(message)
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SPACE:
				{
					static int n;
					//m_pSkinnedMesh->SetAnimationIndex(++n, true);
				}
				break;
			}
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

void cMainGame::SetLight()
{
	D3DXVECTOR3 vDirection( rand() % 100,-rand() % 100, rand() % 100);
	D3DXVec3Normalize(&vDirection, &vDirection);

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));

	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Direction = vDirection;

	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}
