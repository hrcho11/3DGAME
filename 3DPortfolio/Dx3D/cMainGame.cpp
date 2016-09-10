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
	
	//충돌 테스트
	/*
	float fTestX = 5.0f;
	if (GetAsyncKeyState(' ') & 0x8000)
		fTestX = 10.0f;
	
	cSphereCollider* sphereCollider = new cSphereCollider(5.0f, &D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	cSphereCollider* sphereCollider1 = new cSphereCollider(5.0f, &D3DXVECTOR3(fTestX, 5.0f, 0.0f));
	
	D3DMATERIAL9 mtl;
	if (IntersectColliders(sphereCollider, sphereCollider1))
	{
		mtl.Ambient = mtl.Diffuse = mtl.Emissive = mtl.Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		mtl.Ambient = mtl.Diffuse = mtl.Emissive = mtl.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&mtl);
	sphereCollider->Render();
	sphereCollider1->Render();
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	SAFE_DELETE(sphereCollider);
	SAFE_DELETE(sphereCollider1);
	*/

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
		m_pCamera->WindowInputProc(message, wParam, lParam);

	
}


