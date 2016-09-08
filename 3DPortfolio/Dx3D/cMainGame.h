#pragma once

class cCamera;
class cSkinnedMesh;
class cTerrain;
class cRay;

class cMainGame
{
private:
	cCamera*					m_pCamera;
	cSkinnedMesh*			m_pSkinnedMesh;
	cTerrain*					m_pTerrain;
	cRay*						m_pRay;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

