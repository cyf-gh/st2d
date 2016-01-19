#include "FontRendererImpl.h"

////////////////////////////////////////////////////////////////////////////////

stFontRenderer::stFontRenderer( LPDIRECT3DDEVICE9 pDevice, const wchar_t *strFaceNameW ) {
	D3DXFONT_DESC desc;	

	ZeroMemory( &desc, sizeof( D3DXFONT_DESC ) );
	desc.Height         = 25;    // in logical units
	desc.Width          = 12;    // in logical units
	desc.Weight         = 500;   // boldness, range 0(light) - 1000(bold)
	desc.Italic         = false;   
	desc.CharSet        = DEFAULT_CHARSET;
	desc.OutputPrecision= 0;
	desc.Quality        = 0;           
	desc.PitchAndFamily = 0;           
	wcscpy_s( desc.FaceName, strFaceNameW ); // font style
	D3DXCreateFontIndirect( pDevice, & desc, & m_pFont );
}

stFontRenderer::~stFontRenderer() { 
	st_safe_rel( m_pFont );
}

stResult stFontRenderer::Draw( const wchar_t *strDraw, in_un32 unTextCount, stRect * pRect, KREF( D3DCOLOR ) tColor, in_ul32 dFormat ) {
	return m_pFont->DrawText( NULL, strDraw, unTextCount, & pRect->ToRECT(), dFormat, tColor );
}

stResult stFontRenderer::PreLoad( const wchar_t *strDraw, in_un32 unTextCount ) {
	return m_pFont->PreloadText( strDraw, unTextCount );
}
