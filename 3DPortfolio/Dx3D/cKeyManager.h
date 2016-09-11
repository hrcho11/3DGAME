#pragma once
#include <bitset>
#define g_KeyManager		cKeyManager::GetInstance()
#define KEYMAX 256


class cKeyManager
{
public:
	SINGLETONE(cKeyManager)
private:
	std::bitset<KEYMAX> m_keyUp;
	std::bitset<KEYMAX> m_keyDown;
public:

	void Initialize();
	void Destroy();
	
	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);
};

