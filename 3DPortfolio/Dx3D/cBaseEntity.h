#pragma once
class cBaseEntity :public cObject
{
protected:
	int		m_nID;
	int		m_nHealth;
	static int m_nNextValidID;
public:
	cBaseEntity();
	cBaseEntity(int id)
	{
		SetID(id);
	}
	virtual ~cBaseEntity();

	virtual void Update() = 0;

	void SetID(int value);

	int GetID() const { return m_nID; }
};

