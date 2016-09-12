#pragma once
#include "cState.h"

class cMonster;

class cState_Run : public cState<cMonster>
{
private:
	cState_Run() {};
	~cState_Run() {}

public:
	static cState_Run* Get()
	{
		static cState_Run instance;
		return &instance;
	}
	void Enter(cMonster* pMonster);
	void Execute(cMonster* pMonster);
	void Exit(cMonster* pMonster);
};

class cState_Attack : public cState<cMonster>
{
private:
	cState_Attack() {};
	~cState_Attack() {}

public:
	static cState_Attack* Get()
	{
		static cState_Attack instance;
		return &instance;
	}
	void Enter(cMonster* pMonster);
	void Execute(cMonster* pMonster);
	void Exit(cMonster* pMonster);
};