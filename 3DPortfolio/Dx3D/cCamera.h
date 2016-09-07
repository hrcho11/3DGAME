#pragma once

#include "cFrustumCulling.h"


class cCamera
{
private:
	float						m_fZdist;
	float						m_fRotY;
	float						m_fRotX;
	bool						m_isDrag;
	POINT						m_ptPrevMousePos;
	cFrustumCulling			m_cFrustum;
public:
	cCamera();
	~cCamera();

	void Initialize();
	void Update(D3DXVECTOR3* vLookAt);
	void Destroy();
	void Render();

	bool CheckFrustum(D3DXVECTOR3* pv);
	bool CheckFrustum(D3DXVECTOR3* pv, float radius);
	void WindowInputProc(UINT message, WPARAM wParam, LPARAM lParam);

	cFrustumCulling* GetFrustum(void) { return &m_cFrustum; }
	
};

