#pragma once
class cCapsuleCollider : public cCollider
{
private:
	float			m_fRadius;
	LPD3DXVECTOR3	m_pVec3_A_Pos;
	LPD3DXVECTOR3	m_pVec3_B_Pos;
	LPD3DXMESH		m_pMesh_A_;
	LPD3DXMESH		m_pMesh_B_;
private:
	void RemodelMesh(float p_fRadius);
public:
	cCapsuleCollider();
	cCapsuleCollider(float p_fRadius, LPD3DXVECTOR3 p_pVec3_A_Pos, LPD3DXVECTOR3 p_pVec3_B_Pos);
	virtual ~cCapsuleCollider();

	bool			IntersectSphere(cSphereCollider* p_pSphereCol);
	bool			IntersectCapsule(cCapsuleCollider* p_pCapsuleCol);


	//GetterSetter

	float			GetRadius()							{ return m_fRadius; };
	//반지름이 변경되면 크기가 변하기에
	//렌더할 메쉬를 삭제한 후 다시 만듬 (Need a Solution)
	void			SetRadiusAndMesh(float p_fRadius)	{ m_fRadius = p_fRadius; this->RemodelMesh(p_fRadius); };

	LPD3DXVECTOR3	Get_A_Pos()								{ return m_pVec3_A_Pos; };
	void			Set_A_Pos(LPD3DXVECTOR3 p_pVec3_A_Pos)	{ m_pVec3_A_Pos = p_pVec3_A_Pos; };
	
	LPD3DXVECTOR3	Get_B_Pos()								{ return m_pVec3_B_Pos; };
	void			Set_B_Pos(LPD3DXVECTOR3 p_pVec3_B_Pos)	{ m_pVec3_B_Pos = p_pVec3_B_Pos; };

	
	//override cCollider

	virtual cCollider::E_TYPE	GetType()								override;
	virtual bool				IntersectCollider(cCollider* p_pCol)	override;
	virtual void				Render()								override;
};

