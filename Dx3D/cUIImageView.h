#pragma once

#include "cUIObject.h"

class cUIImageView : public cUIObject
{
private:
	SYNTHESIZE(D3DCOLOR, m_dwColor, Color);
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

public:
	cUIImageView(void);
	virtual ~cUIImageView(void);

	// cUIObject override
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

