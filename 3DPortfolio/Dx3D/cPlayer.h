#pragma once
#include "cBaseEntity.h"

class cSkinnedMesh;


class cPlayer : public cBaseEntity
{
private:
	cSkinnedMesh*		m_pSkinnedMesh;
public:
	cPlayer();
	~cPlayer();

	void Init();
	void Update();
	void Destroy();
	void Render();
};

