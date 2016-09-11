#pragma once
class cBaseEntity
{
protected:
	int		m_nID;

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

