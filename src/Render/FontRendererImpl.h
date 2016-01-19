/// ----------------------------------------------------------------------------
/// \file FontRenderer.h
/// \brief simple font renderer implements
/// ----------------------------------------------------------------------------
#pragma once

#include "..\MathsExtend\Rect.h"
#include "..\RenderDev.h"

class stRect;
/// ----------------------------------------------------------------------------
/// \brief simple font renderer implements
/// ----------------------------------------------------------------------------
class stFontRenderer : public IFontRenderer {
protected:
	ID3DXFont * m_pFont;

public:
	virtual stResult Draw( const wchar_t *strDraw, in_un32 unTextCount, stRect * pRect, KREF( D3DCOLOR ) tColor, in_ul32 dFormat );
	virtual stResult PreLoad( const wchar_t *strDraw, in_un32 unTextCount );

	stFontRenderer( LPDIRECT3DDEVICE9 pDevice, const wchar_t *strFaceNameW );
	virtual ~stFontRenderer();
};

