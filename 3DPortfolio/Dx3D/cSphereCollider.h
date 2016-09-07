#pragma once
class cSphereCollider : public cCollider
{
private:
	float			m_fRadius;
	LPD3DXVECTOR3	m_pVec3Pos;
	LPD3DXMESH		m_pMesh;
public:
	cSphereCollider();
	cSphereCollider(float p_fRadius, LPD3DXVECTOR3 p_pVec3Pos);
	virtual ~cSphereCollider();

	//GetterSetter
	float	GetRadius()					{ return m_fRadius; };
	void	SetRadius(float p_fRadius)	{ m_fRadius = p_fRadius; };
	
	LPD3DXVECTOR3	GetPos()							{ return m_pVec3Pos; };
	void			SetPos(LPD3DXVECTOR3 p_pVec3Pos)	{ m_pVec3Pos = p_pVec3Pos; };

	//override cCollider
	virtual cCollider::E_TYPE	GetType()								override;
	virtual bool				IntersectCollider(cCollider* p_pCol)	override;
	virtual void				Render()								override;
};
