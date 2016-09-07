#pragma once

class cCamera
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	D3DXMATRIXA16	m_matView;
	D3DXMATRIXA16	m_matProj;
	float			m_fDistance;
	float			m_fRotY;
	float			m_fRotX;
	bool			m_isLButtonDown;
	POINT			m_ptPrevMouse;
	//D3DXVECTOR3* m_pTarget;
public:
	cCamera(void);
	~cCamera(void);

	void Setup();
	void Update(D3DXVECTOR3* pTarget);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

