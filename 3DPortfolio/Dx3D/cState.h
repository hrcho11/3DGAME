#pragma once

template<class entity_type>
class cState
{
public:
	virtual void Enter(entity_type*) = 0;
	virtual void Execute(entity_type*) = 0;
	virtual void Exit(entity_type*) = 0;

	virtual ~cState() {}
};