#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager()
{

}


cKeyManager::~cKeyManager()
{
}



void cKeyManager::Initialize()
{
	for (int i = 0; i < KEYMAX; ++i)
		m_keyUp.set(i, false);
}

void cKeyManager::Destroy()
{
}

bool cKeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!m_keyDown[key])
		{
			m_keyDown.set(key, true);
			return true;
		}
	}
	else m_keyDown.set(key, false);

	return false;
}

bool cKeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		m_keyUp.set(key, true);
	else
	{
		if (m_keyUp[key])
		{
			m_keyUp.set(key, false);
			return true;
		}
	}
	return false;
}

bool cKeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
			return true;

	return false;
}
