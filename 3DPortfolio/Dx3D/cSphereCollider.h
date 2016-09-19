#pragma once
class cSphereCollider : public cCollider
{
private:
	float			m_fRadius;
	LPD3DXVECTOR3	m_pVec3Pivot;
	float*			m_pFDistPerSec;
	LPD3DXVECTOR3	m_pVec3Direction;
	LPD3DXMESH		m_pMesh;

private:
	cSphereCollider(){};
	void RemodelMesh(float p_fRadius);

public:
	cSphereCollider
		(float			p_fRadius,
		LPD3DXVECTOR3	p_pVec3Pivot,
		float*			p_pFDistPerSec,
		LPD3DXVECTOR3	p_pVec3Direction);
	virtual ~cSphereCollider();

	bool			IntersectSphere(cSphereCollider* p_pSphereCol);
	bool			IntersectCapsule(cCapsuleCollider* p_pCapsuleCol);
	bool			CollideSpherePhysically(cSphereCollider* p_pSphereCol);
	
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

	float*			GetDistancePerSecond()
					{ return m_pFDistPerSec; };
	void			SetDistancePerSecond(float* p_pFDistPerSec)
					{ m_pFDistPerSec = p_pFDistPerSec; };

	LPD3DXVECTOR3	GetDirection()
					{ return m_pVec3Direction; };
					//넣은 값은 노말라이즈된다.
	void			SetDirection(LPD3DXVECTOR3 p_pVec3Direction)
					{ m_pVec3Direction = p_pVec3Direction; D3DXVec3Normalize(m_pVec3Direction, m_pVec3Direction); };


	//override cCollider

	virtual cCollider::E_TYPE	GetType()								override;
	virtual bool				IntersectCollider(cCollider* p_pCol)	override;
	virtual bool				CollidePhysically(cCollider* p_pCol)	override;
	virtual void				Update()								override;
	virtual void				Render()								override;
};
