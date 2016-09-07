#pragma once

#include "cUIObject.h"

class cUIButton;

class iUIButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};

class cUIButton : public cUIObject
{
private:
	enum eButtonState
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELECTED,
		E_COUNT
	};
	eButtonState		m_eButtonState;
	LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];

	SYNTHESIZE(iUIButtonDelegate*, m_pDelegate, Delegate);
public:
	cUIButton(void);
	virtual ~cUIButton(void);

	virtual void SetTexture(std::string sNormal,
		std::string sMouseOver,
		std::string sSelected);
	
	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override; 
};

