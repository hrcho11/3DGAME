#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
	: m_vEye(0, 0, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fDistance(5)
	, m_fRotX(0.0f)
	, m_fRotY(0.0f)
	, m_isLButtonDown(false)
{
}


cCamera::~cCamera(void)
{
}

void cCamera::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, 
		D3DX_PI / 4.0f, 
		rc.right / (float)rc.bottom,
		1,
		5000);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void cCamera::Update(D3DXVECTOR3* pTarget)
{
	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);
	
	D3DXMATRIXA16 matRotX, matRotY, matRot;
	D3DXMatrixRotationX(&matRotX, m_fRotX);
	D3DXMatrixRotationY(&matRotY, m_fRotY);
	matRot = matRotX * matRotY;
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRot);
	
	if (pTarget)
	{
		m_vLookAt = *pTarget;
		m_vEye = *pTarget + m_vEye;
	}

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void cCamera::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			m_ptPrevMouse.x = LOWORD(lParam);
			m_ptPrevMouse.y = HIWORD(lParam);
			m_isLButtonDown = true;
		}
		break;
	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		{
			if (m_isLButtonDown)
			{
				POINT ptCurrMouse;
				ptCurrMouse.x = LOWORD(lParam);
				ptCurrMouse.y = HIWORD(lParam);

				m_fRotY += (ptCurrMouse.x - m_ptPrevMouse.x) / 100.f;
				m_fRotX += (ptCurrMouse.y - m_ptPrevMouse.y) / 100.f;

				if (m_fRotX <= -3.141592f * 0.5f + D3DX_16F_EPSILON)
				{
					m_fRotX = -3.141592f * 0.5f + D3DX_16F_EPSILON;
				}
				if (m_fRotX >=  3.141592f * 0.5f - D3DX_16F_EPSILON)
				{
					m_fRotX =  3.141592f * 0.5f - D3DX_16F_EPSILON;
				}

				m_ptPrevMouse = ptCurrMouse;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		m_fDistance -= GET_WHEEL_DELTA_WPARAM(wParam) / 100.f;
		break;
	}
}
