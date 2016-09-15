#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
	:m_fZdist(-60.0f)
	,m_fRotX(1.1f)
	,m_fRotY(0.0f)
	, m_isDrag(false)
{
}


cCamera::~cCamera()
{
}

void cCamera::Initialize()
{
	
	D3DXVECTOR3 vEye, vLookAt, vUp;
	D3DXMATRIXA16 matView, matProj;
	vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	vEye = D3DXVECTOR3(0.f, 0.f, m_fZdist);
	vLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	RECT rect;

	GetClientRect(g_hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI*0.25f , (float)width / (float)height, 1.0f, 1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIXA16 matViewProj = matView * matProj;

	m_cFrustum.Initialize();
}

void cCamera::Update(D3DXVECTOR3* vLookAt)
{
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, m_fZdist);

	D3DXMATRIXA16 matRotationX,matRotationY,matR;

	D3DXMatrixRotationX(&matRotationX, m_fRotX);
	D3DXMatrixRotationY(&matRotationY, m_fRotY);
	matR = matRotationX *  matRotationY;
	D3DXVec3TransformCoord(&vPos, &vPos, &matR);

	D3DXVECTOR3 vLook = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.0f, 0.f);
	if (vLookAt)
	{
		vLook = *vLookAt;
		vPos = *vLookAt + vPos;
	}

	D3DXMatrixLookAtLH(&matView, &vPos, &vLook, &vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	m_cFrustum.Update();
}

void cCamera::Destroy()
{
}

void cCamera::Render()
{
	m_cFrustum.Render();
}

bool cCamera::CheckFrustum(D3DXVECTOR3 * pv)
{
	if (m_cFrustum.IsIn(pv))
		return true;
	else 
		return false;
}

bool cCamera::CheckFrustum(D3DXVECTOR3 * pv, float radius)
{
	if(m_cFrustum.IsInSphere(pv, radius))
		return true;
	return false;
}

void cCamera::WindowInputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_isDrag = true;
		m_ptPrevMousePos.x = GET_X_LPARAM(lParam);
		m_ptPrevMousePos.y = GET_Y_LPARAM(lParam);

		break;
	case WM_LBUTTONUP:
		m_isDrag = false;
		break;
	case WM_MOUSEMOVE:
		/*if (m_isDrag)
		{
			POINT ptMouse;
			ptMouse.x = GET_X_LPARAM(lParam);
			ptMouse.y = GET_Y_LPARAM(lParam);

			float fDeltaX = (float)(ptMouse.x - m_ptPrevMousePos.x);
			float fDeltaY = (float)(ptMouse.y - m_ptPrevMousePos.y);

			m_fRotX += (fDeltaY / 100.f);
			m_fRotY += (fDeltaX / 100.0f);

			if (m_fRotX <= -D3DX_PI *0.5f + 0.000125f)
				m_fRotX = -D3DX_PI *0.5f + 0.000125f;
			else if (m_fRotX >= D3DX_PI* 0.5f - 0.000125f)
				m_fRotX = D3DX_PI* 0.5f - 0.000125f;

			m_ptPrevMousePos = ptMouse;
		}
	
		*/
		break;
	case WM_MOUSEWHEEL:
		//m_fZdist +=  GET_WHEEL_DELTA_WPARAM(wParam) /100.f;
		//if (m_fZdist > -1.0f)
		//	m_fZdist = -1.0f;
		break;
	}
}
