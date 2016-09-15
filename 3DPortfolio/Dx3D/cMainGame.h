#pragma once

class cCamera;
class cSkinnedMesh;
class cTerrain;
class cRay;
class cPlayer;
class cMonsterManager;

class cMainGame
{
private:
	cCamera*					m_pCamera;
	cSkinnedMesh*			m_pSkinnedMesh;
	cTerrain*					m_pTerrain;
	cPlayer*					m_pPlayer;
	cMonsterManager*		m_pMonsterManager;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

