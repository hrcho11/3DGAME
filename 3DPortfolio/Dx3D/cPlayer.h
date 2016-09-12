#pragma once
#include "cBaseEntity.h"
#include "CharacterController.h"
class cSkinnedMesh;

class cMonsterManager;
class cPlayer : public cBaseEntity
{
private:

	CharacterController*	m_pCharacterController;
public:
	cPlayer();
	~cPlayer();

	void Init();
	void Update();
	void Destroy();
	void Render();

	void SetDestination(D3DXVECTOR3& pos);

	D3DXVECTOR3& GetPos() { return m_pCharacterController->GetPos(); }
};

