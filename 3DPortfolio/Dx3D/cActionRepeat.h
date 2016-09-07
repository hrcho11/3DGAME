#pragma once

#include "cAction.h"

class cActionRepeat
	: public cAction
	, public iActionDelegate
{
protected:
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);

public:
	cActionRepeat(void);
	virtual ~cActionRepeat(void);
	
	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(cAction* pSender) override;
};

