#pragma once
#include "cBaseEntity.h"
#include "cSkinnedMesh.h"
#include "cStateMachine.h"

class cMonsterOwnedStates;


enum eMonsterAnim
{
	RUN = 1,
	ATTACK
};


class cMonster : public cBaseEntity
{
protected:
	cStateMachine<cMonster>*		m_pStateMachine;
	cSkinnedMesh*					m_pSkinnedMesh;
	D3DXVECTOR3						m_vPos;
	D3DXVECTOR3						m_vDir;
	D3DXVECTOR3						m_vPlayerPos;
	float								m_fSpeed;
	float								m_fAtkRadius;


	void LoadModel(char* path, char* fileName);

public:

	cMonster();
	~cMonster();
	virtual void Init() {} ;
	virtual void Update() {};
	void Init(char* path, char* fileName);
	void Update( D3DXVECTOR3& vPos);
	virtual void Destroy();
	virtual void Render();

	void SetPos(const D3DXVECTOR3& vPos);
	void SetAnim(eMonsterAnim anim);
	void SetPlayerPos(D3DXVECTOR3& playerPos);
	cSkinnedMesh* GetSkinnedMesh() { return m_pSkinnedMesh; }
	D3DXVECTOR3&	GetPos() { return m_vPos; }
	D3DXVECTOR3&	GetDirection() { return m_vDir; }
	void Tracking();
	float RotY(D3DXVECTOR3& vDir);

	void Attack();
	void Run();
	bool SenseAtkPlayer();

	void AIUpdate();
};

