#include "StdAfx.h"
#include "cMtlTex.h"


cMtlTex::cMtlTex(void)
	: m_pTexture(NULL)
	, m_nMtlRef(-1)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}

cMtlTex::~cMtlTex(void)
{
	SAFE_RELEASE(m_pTexture);
}
