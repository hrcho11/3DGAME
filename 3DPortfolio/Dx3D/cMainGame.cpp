#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cSkinnedMesh.h"
#include "cTerrain.h"
#include "cRay.h"
#include "cPlayer.h"
#include "cMonsterManager.h"
cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pSkinnedMesh(NULL)
	, m_pTerrain(NULL)
	, m_pPlayer(NULL)
	,m_pMonsterManager(NULL)
{
	
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pTerrain);
	m_pPlayer->Destroy();
	SAFE_RELEASE(m_pPlayer);
	SAFE_DESTROY(m_pMonsterManager);

	g_pSkillManager->Destroy();
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

	m_pPlayer = new cPlayer;
	m_pPlayer->Init();

	g_pSkillManager->Init();

	m_pMonsterManager = new cMonsterManager;
	m_pMonsterManager->Init();


	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	
	if(m_pCamera)
		m_pCamera->Update(&D3DXVECTOR3(0,0,0));

	if (m_pPlayer)
		m_pPlayer->Update();

	if (m_pMonsterManager)
		m_pMonsterManager->Update(m_pPlayer->GetPos());

	g_pSkillManager->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(230, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	m_pTerrain->Draw(m_pCamera->GetFrustum());

	m_pPlayer->Render();

	m_pMonsterManager->Render();

	g_pSkillManager->Render();

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
		m_pCamera->WindowInputProc(message, wParam, lParam);
	
	switch (message)
	{
		case WM_RBUTTONDOWN:
				cRay ray = cRay::GetRayVector(LOWORD(lParam), HIWORD(lParam));
				float u, v, dist;
				for (int i = 0; i < 6; i+=3)
				{
					if (D3DXIntersectTri(
						&m_pTerrain->GetPickingGround()[i].p,
						&m_pTerrain->GetPickingGround()[i + 1].p,
						&m_pTerrain->GetPickingGround()[i + 2].p,
						ray.GetOrigin(),
						ray.GetDirection(),
						&u, &v, &dist))
					{
						D3DXVECTOR3 origin = *ray.GetOrigin();
						D3DXVECTOR3 dir = *ray.GetDirection();
						D3DXVECTOR3 pickedPosition = *ray.GetOrigin() + ((*ray.GetDirection()) * dist);
						pickedPosition.y = 0.0f;
						m_pPlayer->SetDestination(pickedPosition);
						break;
					}
				}
			break;
	}

	
}


