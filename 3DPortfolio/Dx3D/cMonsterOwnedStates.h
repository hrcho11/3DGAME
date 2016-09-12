#pragma once
#include "cState.h"

class cMonster;

class cState_Idle : public cState<cMonster>
{
	cState_Idle() {};
	~cState_Idle() {}
public:
	static cState_Idle*	Get()
	{
		static cState_Idle instance;
		return &instance;
	}
	virtual void Enter(cMonster* pMonster);
	virtual void Execute(cMonster* pMonster);
	virtual void Exit(cMonster* pMonster);
};

class cState_Run : public cState<cMonster>
{
private:
	cState_Run();
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
	cState_Attack();
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