#pragma once

class iMap;

class cCharactorController
{
private:
	D3DXVECTOR3		m_vPosition;
	float			m_fRotY;
	D3DXMATRIXA16	m_matWorld;
	float			m_fSpeed;

public:
	cCharactorController(void);
	~cCharactorController(void);

	void Update(iMap* pMap);
	D3DXVECTOR3* GetPosition();
	D3DXMATRIXA16* GetWorldTM();
};

