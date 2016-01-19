#include <stlib\String\StringImpl.h>
#include "FontInfoImpl.h"

FontInfoImpl::FontInfoImpl( stStringW * strFontFilePathW, in_un32 unFontPixelSize ) {
	FT_Error tErr = FT_Init_FreeType( & m_tFTLib );

	if ( st_do_err( tErr ) )
		throw "FontInfoImpl::FontInfoImpl FT_Init_FreeType Abort";
	FT_New_Face( m_tFTLib, strFontFilePathW->GetStringASCI(), 0, & m_tFace );

}