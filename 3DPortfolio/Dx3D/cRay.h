#pragma once


class cRay
{
private:
	D3DXVECTOR3 m_vOrigin;
	D3DXVECTOR3 m_vDir;
public:
	cRay();
	~cRay();

	const cRay& GetRayVector(int x, int y);

};

