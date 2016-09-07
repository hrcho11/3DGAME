#include "StdAfx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager(void)
{
}

cTextureManager::~cTextureManager(void)
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( char* szFullPath,OUT D3DXIMAGE_INFO* pImageInfo /*= NULL*/ )
{
	return GetTexture(std::string(szFullPath), pImageInfo);
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( std::string sFullPath,OUT D3DXIMAGE_INFO* pImageInfo /*= NULL*/ )
{	
	if (pImageInfo)
	{
		if (m_mapTexture.find(sFullPath) == m_mapTexture.end() || 
			m_mapImageInfo.find(sFullPath) == m_mapImageInfo.end())
		{
			SAFE_RELEASE(m_mapTexture[sFullPath]);

			D3DXCreateTextureFromFileEx(
				g_pD3DDevice,
				sFullPath.c_str(), 
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT,
				0,
				D3DFMT_UNKNOWN, 
				D3DPOOL_MANAGED, 
				D3DX_FILTER_NONE, 
				D3DX_DEFAULT, 
				0,//D3DCOLOR_XRGB(255, 255, 255),
				&m_mapImageInfo[sFullPath],
				NULL,
				&m_mapTexture[sFullPath]);
		}
		*pImageInfo = m_mapImageInfo[sFullPath];
	}
	else
	{
		if (m_mapTexture.find(sFullPath) == m_mapTexture.end())
		{
			D3DXCreateTextureFromFile(g_pD3DDevice, sFullPath.c_str(), &m_mapTexture[sFullPath]);
		}
	}

	return m_mapTexture[sFullPath];
}

void cTextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapTexture.clear();
	m_mapImageInfo.clear();
}
