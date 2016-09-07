#pragma once

class cCollider
{
public:
	enum E_TYPE
	{
		eSPHERE=0,
		eCOUNT,
	};
public:
	cCollider();
	virtual ~cCollider();

	virtual cCollider::E_TYPE	GetType()								PURE;
	virtual bool				IntersectCollider(cCollider* p_pCol)	PURE;
	virtual void				Render()								PURE;
};

static bool IntersectColliders(cCollider* p_pCol1, cCollider* p_pCol2)
{
	return p_pCol1->IntersectCollider(p_pCol2);
}