#pragma once
#include "cState.h"

class cMonster;

class cGlobalState_Monster : public cState<cMonster>
{
private:
	cGlobalState_Monster() {};
	~cGlobalState_Monster() {};
public:
	static cGlobalState_Monster* Get()
	{
		static cGlobalState_Monster instance;
		return &instance;
	}
	void Enter(cMonster* pMonster) {};
	void Execute(cMonster* pMonster) ;
	void Exit(cMonster* pMonster) {};
};

class cState_Track : public cState<cMonster>
{
private:
	cState_Track() {};
	~cState_Track() {}

public:
	static cState_Track* Get()
	{
		static cState_Track instance;
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

class cState_Avoid : public cState<cMonster>
{
private:
		cState_Avoid() {};
		~cState_Avoid() {}
public:
	static cState_Avoid* Get()
	{
		static cState_Avoid instance;
		return &instance;
	}
	void Enter(cMonster* pMonster);
	void Execute(cMonster* pMonster);
	void Exit(cMonster* pMonster);
};