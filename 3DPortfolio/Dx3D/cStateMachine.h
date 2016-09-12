#pragma once
#include "cState.h"

template<class entity_type>
class cStateMachine
{
private:
	entity_type*				m_pOwner;
	cState<entity_type>*	m_pCurrentState;
	cState<entity_type>*	m_pPreviousState;
	cState<entity_type>*	m_pGlobalState;
public:
	cStateMachine(entity_type* owner)
		:m_pOwner(nullptr)
		,m_pCurrentState(nullptr)
		,m_pPreviousState(nullptr)
		,m_pGlobalState(nullptr)
	{

	}
	~cStateMachine() {};

	void SetCurrentState(cState<entity_type>* s)
	{
		m_pCurrentState = s;
	}
	void SetPreviousState(cState<entity_type>* s)
	{
		m_pPreviousState = s;
	}
	void SetGlobalState(cState<entity_type>* s)
	{
		m_pGlobalState = s;
	}

	void Update() const
	{
		if (m_pGlobalState)
		{
			m_pGlobalState->Execute(m_pOwner);
		}

		if (m_pCurrentState)
			m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(cState<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine::ChangeState: trying to change to a null state");

		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->Exit(m_pOwner);
		m_pCurrentState = pNewState;
		m_pCurrentState->Enter(m_pOwner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	cState<entity_type>* GetCurrentState() const { return m_pCurrentState; }
	cState<entity_type>* GetGlobalState() const { return m_pGlobalState; }
	cState<entity_type>* GetPreviousState() const { return m_pPreviousState; }

	bool isInState(const cState<entity_type>& st)const
	{
		if (st == m_pCurrentState)return true;
		else false;
	}
};

