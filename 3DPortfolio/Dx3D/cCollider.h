#pragma once

class cCollider
{
public:
	enum E_TYPE
	{
		eSPHERE = 0,
		eCAPSULE = 1,
		eCOUNT,
	};
public:
	cCollider();
	virtual ~cCollider();

	virtual cCollider::E_TYPE	GetType()								PURE;
	virtual bool				IntersectCollider(cCollider* p_pCol)	PURE;
	virtual bool				CollidePhysically(cCollider* p_pCol)	PURE;
	virtual void				Update()								PURE;
	virtual void				Render()								PURE;
};

static bool IntersectColliders(cCollider* p_pCol1, cCollider* p_pCol2)
{
	if (p_pCol1 == nullptr) return false;
	return p_pCol1->IntersectCollider(p_pCol2);
}

//구-구 충돌만 구현되있음
static bool CollideCollidersPhysically(cCollider* p_pCol1, cCollider* p_pCol2)
{
	if (p_pCol1 == nullptr) return false;
	return p_pCol1->CollidePhysically(p_pCol2);
}