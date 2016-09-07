#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
	SINGLETONE(cFontManager);
public:
	enum eFontType
	{
		E_NORMAL,
		E_CHAT,
		E_QUEST,
	};
	
private:
	std::map<eFontType, LPD3DXFONT> m_mapFont;

public:
	LPD3DXFONT GetFont(eFontType e); 
	void Destroy();
};

