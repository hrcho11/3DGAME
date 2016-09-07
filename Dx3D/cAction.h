#pragma once

class cAction;
class cGameObject;

class iActionDelegate
{
public:
	virtual void OnActionFinish(cAction* pSender) = 0;
};

class cAction : public cObject
{
protected:
	float m_fPassedActionTime;
	
	SYNTHESIZE(cGameObject*, m_pTarget, TargetObject);
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);

public:
	cAction(void);
	virtual ~cAction(void);

	virtual void Start();
	virtual void Update();
};

