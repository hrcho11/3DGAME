#pragma once

class cGrid;
class cCamera;
class cCube;
class cCharactorController;
class cGroup;
class iMap;
class cFrame;
class cAseCharacter;
class cMtlTex;
class cSkinnedMesh;
class cFrustum;

class cMainGame
{
private:
	cGrid*						m_pGrid;
	cCamera*					m_pCamera;
	cCharactorController*		m_pCharactorContoller;
	cSkinnedMesh*				m_pSkinnedMesh;
	std::vector<cSkinnedMesh*>	m_vecSkinnedMesh;
	cFrustum*					m_pFrustum;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

