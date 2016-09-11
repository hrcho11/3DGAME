#pragma once
class cCapsuleCollider : public cCollider
{
private:
	float			m_fRadius;
	LPD3DXVECTOR3	m_pVec3RadRot;
	LPD3DXVECTOR3	m_pVec3Pivot;
	D3DXVECTOR3		m_vec3DistA;
	D3DXVECTOR3		m_vec3DistB;
	LPD3DXMESH		m_pMesh;
private:
	     //보류
	void CreateMesh(UINT unSlices, UINT unStacks);
	void RemodelMesh(float p_fRadius);
public:
	cCapsuleCollider();
	//DistA와 DistB는 Pivot으로부터 떨어진 거리차를 나타낸다
	cCapsuleCollider
		(float			p_fRadius,
		LPD3DXVECTOR3	p_pVec3RadRot,
		LPD3DXVECTOR3	p_pVec3Pivot,
		D3DXVECTOR3&	p_vec3DistA,
		D3DXVECTOR3&	p_vec3DistB);
	virtual ~cCapsuleCollider();

	bool			IntersectSphere(cSphereCollider* p_pSphereCol);
	bool			IntersectCapsule(cCapsuleCollider* p_pCapsuleCol);


	//GetterSetter

	float			GetRadius()
					{ return m_fRadius; };
					//반지름이 변경되면 크기가 변하기에
					//렌더할 메쉬를 삭제한 후 다시 만듬 (Need a Solution)
	void			SetRadiusAndMesh(float p_fRadius)
					{ m_fRadius = p_fRadius; this->RemodelMesh(p_fRadius); };

	LPD3DXVECTOR3	GetRadRotation()
					{ return m_pVec3RadRot; };
	void			SetRadRotation(LPD3DXVECTOR3 p_pVec3RadRot)	
					{ m_pVec3RadRot = p_pVec3RadRot; };

	LPD3DXVECTOR3	GetPivot()
					{ return m_pVec3Pivot; };
	void			SetPivot(LPD3DXVECTOR3 p_pVec3Rot)
					{ m_pVec3Pivot = p_pVec3Rot; };

	D3DXVECTOR3&	GetDistanceA()
					{ return m_vec3DistA; };
	void			SetDistanceA(D3DXVECTOR3& p_vec3DistA)
					{ m_vec3DistA = p_vec3DistA; };
	
	D3DXVECTOR3&	GetDistanceB()
					{ return m_vec3DistA; };
	void			SetDistanceB(D3DXVECTOR3& p_vec3DistB)
					{ m_vec3DistB = p_vec3DistB; };

	
	//override cCollider

	virtual cCollider::E_TYPE	GetType()								override;
	virtual bool				IntersectCollider(cCollider* p_pCol)	override;
								//보류
	virtual void				Render()								override;
};

