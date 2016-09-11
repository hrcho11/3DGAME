#pragma once
#include "cBaseEntity.h"

class cSkinnedMesh;
class CharacterController;

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

};

