#pragma once
#include "cFrustumCulling.h"
class cMagicSkill
{
protected:

	int					m_nAtkDamage;
	float				m_fMoveSpeed;
	float				m_fDistance;
	LPD3DXMESH		m_pMesh;
	D3DXMATRIXA16	m_matWorld;
	D3DXVECTOR3		m_vPos;
	bool				m_isFire;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vStartPos;
	cSphereCollider*	m_pCollider;
public:
	cMagicSkill();
	~cMagicSkill();

	virtual void Init(D3DXVECTOR3& vPos);
	virtual void Update();
	virtual void Destroy();
	virtual void Render();
	void SetToOriginal();

	inline bool GetIsFire() { return m_isFire; }

	inline void SetIsFire(bool isFire) { m_isFire = isFire; }
	void SetDiretion(D3DXVECTOR3& vDir);
};

