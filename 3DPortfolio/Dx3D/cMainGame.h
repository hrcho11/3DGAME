#pragma once

class cCamera;
class cPlayer;
class cMonsterManager;
class cMapManager;
class cMainGame
{
private:
	cCamera*					m_pCamera;
	cSkinnedMesh*			m_pSkinnedMesh;
	cPlayer*					m_pPlayer;
	cMonsterManager*		m_pMonsterManager;
	cMapManager*			m_pMapManager;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

