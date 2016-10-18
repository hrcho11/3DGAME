#pragma once

#define EPSILON 0.05f

class CharacterController
{
public:
	enum eAnimationState
	{
		IDLE,
		RUN,
		ATK
	};
private:
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vDir;
	D3DXVECTOR3				m_vDestination;
	float						m_fSpeed;
	eAnimationState m_eState;
	cSphereCollider*		m_pCollider;
	cSkinnedMesh*		m_pSkinnedMesh;
public:

	CharacterController();
	~CharacterController();
	void Init();
	void Update();
	void Destroy();
	void Render();
	D3DXVECTOR3& Running();
	void StartRunning(D3DXVECTOR3& vDestination);
	void SetDestination(D3DXVECTOR3& vDestination) { m_vDestination = vDestination; }
	float CalculateRotY();
	void Attack();

	void ChangeState(eAnimationState state);


	D3DXVECTOR3& GetPos() { return m_vPos; }
};

