#pragma once

class cMtlTex;
class cGroup;

class cObjLoader : public cObject
{
protected:
	std::map<std::string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFullPath,
		IN D3DXMATRIXA16* pmat,
		OUT std::vector<cGroup*>& vecGroup);

	LPD3DXMESH LoadMesh(IN char* szFullPath,
		IN D3DXMATRIXA16* pmat,
		OUT std::vector<cMtlTex*>& vecMtlTex);

protected:
	void LoadMtlLib(char* szFullPath);
};

