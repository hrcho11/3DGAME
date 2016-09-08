#pragma once

class cCamera;
class cSkinnedMesh;
class cTerrain;
class cRay;
class cPlayer;
class cMainGame
{
private:
	cCamera*					m_pCamera;
	cSkinnedMesh*			m_pSkinnedMesh;
	cTerrain*					m_pTerrain;
	cRay*						m_pRay;
	cPlayer*					m_pPlayer;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

