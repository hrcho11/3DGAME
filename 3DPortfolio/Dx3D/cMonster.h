#pragma once
#include "cBaseEntity.h"
#include "cSkinnedMesh.h"
#include "cStateMachine.h"

class cMonsterOwnedStates;

class cMonster : public cBaseEntity
{
protected:
	cStateMachine<cMonster>*		m_pStateMachine;
	cSkinnedMesh*					m_pSkinnedMesh;

	void LoadModel(char* path, char* fileName);
public:
	cMonster();
	~cMonster();

	void Init(char* path, char* fileName);
	void Update();
	void Destroy();
	void Render();
};

