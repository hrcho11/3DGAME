#pragma once

class cFrame;

class cAseCharacter : public cObject
{
protected:
	//typedef std::map<int, std::string> MY_MAP;
	//SYNTHESIZE_PASS_BY_REF(MY_MAP, m_map, MAp);
	ST_ASE_SCENE	m_stScene;
	cFrame*			m_pRoot;

public:
	cAseCharacter(void);
	~cAseCharacter(void);

	void Load(char* szFullPath);
	void Update(D3DXMATRIXA16* pmatWorld);
	void Render();
};

