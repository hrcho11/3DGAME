#pragma once


class cRay
{
private:
	D3DXVECTOR3 m_vOrigin;
	D3DXVECTOR3 m_vDir;
public:
	cRay();
	~cRay();

	static const cRay& GetRayVector(int x, int y);

	D3DXVECTOR3*	GetOrigin() { return &m_vOrigin; }
	D3DXVECTOR3*	GetDirection() { return &m_vDir; }

};

