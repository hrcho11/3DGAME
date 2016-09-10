#pragma once
class cSphereCollider : public cCollider
{
private:
	float			m_fRadius;
	LPD3DXVECTOR3	m_pVec3Pivot;
	LPD3DXMESH		m_pMesh;

private:
	void RemodelMesh(float p_fRadius);

public:
	cSphereCollider();
	cSphereCollider
		(float			p_fRadius,
		LPD3DXVECTOR3	p_pVec3Pivot);
	virtual ~cSphereCollider();

	bool			IntersectSphere(cSphereCollider* p_pSphereCol);
	bool			IntersectCapsule(cCapsuleCollider* p_pCapsuleCol);

	
	//GetterSetter

	float			GetRadius()
					{ return m_fRadius; };
					//반지름이 변경되면 크기가 변하기에
					//렌더할 메쉬를 삭제한 후 다시 만듬 (Need a Solution)
	void			SetRadiusAndMesh(float p_fRadius)
					{ m_fRadius = p_fRadius; this->RemodelMesh(p_fRadius); };
	
	LPD3DXVECTOR3	GetPivot()
					{ return m_pVec3Pivot; };
	void			SetPivot(LPD3DXVECTOR3 p_pVec3Pivot)
					{ m_pVec3Pivot = p_pVec3Pivot; };


	//override cCollider

	virtual cCollider::E_TYPE	GetType()								override;
	virtual bool				IntersectCollider(cCollider* p_pCol)	override;
	virtual void				Render()								override;
};
