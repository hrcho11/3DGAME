#pragma once

#include "cAction.h"

class cActionMove : public cAction
{
protected:
	SYNTHESIZE(D3DXVECTOR3, m_vFrom, From);
	SYNTHESIZE(D3DXVECTOR3, m_vTo, To);

public:
	cActionMove(void);
	virtual ~cActionMove(void);

	// cAction override
	virtual void Start() override;
	virtual void Update() override;
};

