#pragma once

/* 해골책 참고*/


#define FRUSTUM_SPACE 15.0f
class cFrustumCulling
{
	D3DXVECTOR3		m_vtxProjection[8];
	D3DXPLANE		m_plane[6];
	D3DXVECTOR3		m_vPos;
	std::vector<D3DXVECTOR3> m_vecWorldVertex;
public:

	void Initialize();
	void Update();
	void Render();

	bool IsIn(D3DXVECTOR3* pv);
	bool IsInSphere(D3DXVECTOR3* pv, float radius);

	D3DXVECTOR3*  GetPos() { return &m_vPos; }
	void SetFrustumPos(D3DXVECTOR3& v) { m_vPos = v; }
	cFrustumCulling();
	~cFrustumCulling();
};

