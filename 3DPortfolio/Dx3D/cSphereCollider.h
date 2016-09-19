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
					//�������� ����Ǹ� ũ�Ⱑ ���ϱ⿡
					//������ �޽��� ������ �� �ٽ� ���� (Need a Solution)
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
					//���� ���� �븻������ȴ�.
	void			SetDirection(LPD3DXVECTOR3 p_pVec3Direction)
					{ m_pVec3Direction = p_pVec3Direction; D3DXVec3Normalize(m_pVec3Direction, m_pVec3Direction); };


	//override cCollider

	virtual cCollider::E_TYPE	GetType()								override;
	virtual bool				IntersectCollider(cCollider* p_pCol)	override;
	virtual bool				CollidePhysically(cCollider* p_pCol)	override;
	virtual void				Update()								override;
	virtual void				Render()								override;
};
