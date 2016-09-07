#pragma once

class cUIObject : public cObject
{
protected:
	std::vector<cUIObject*> m_vecChild;
	D3DXMATRIXA16			m_matWorld;

	SYNTHESIZE(D3DXVECTOR3, m_vLocalPos, LocalPos);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(bool, m_isDebugRender, DebugRender);
	SYNTHESIZE(int, m_nTag, Tag);

public:
	cUIObject(void);
	virtual ~cUIObject(void);
	
	virtual void Update();
	virtual void UpdateWorldTM();
	virtual void UpdateChildren();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void AddChild(cUIObject* pChild);
	virtual void Destroy();
	virtual cUIObject* GetChildByTag(int nTag);
	/*
	cObject* m_pObject;
	
	void SetObject(cObject* pObject)
	{
		if(m_pObject != pObject)
		{
			SAFE_RELEASE(m_pObject);
			SAFE_ADDREF(pObject);
			m_pObject = pObject;
		}
	}
	
	cObject* GetObject()
	{
		return m_pObject;
	}
	*/
};

