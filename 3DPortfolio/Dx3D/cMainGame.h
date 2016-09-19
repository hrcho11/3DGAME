#pragma once

class cCamera;
class cPlayer;
class cMonsterManager;
class cMapManager;
class cMainGame
{
private:
	cCamera*				m_pCamera;
	cSkinnedMesh*			m_pSkinnedMesh;
	cPlayer*				m_pPlayer;
	cMonsterManager*		m_pMonsterManager;
	cMapManager*			m_pMapManager;

	//抗力内靛
	cSphereCollider*		m_pSphereCollider1;
	D3DXVECTOR3				m_vec3Pivot1;
	float					m_fDistPerSec1;
	D3DXVECTOR3				m_vec3Direction1;
	cSphereCollider*		m_pSphereCollider2;
	D3DXVECTOR3				m_vec3Pivot2;
	float					m_fDistPerSec2;
	D3DXVECTOR3				m_vec3Direction2;
	//抗力内靛
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

