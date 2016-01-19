#pragma once 

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype\freetype.h>
#include <freetype\ftglyph.h>
#include <stlib\Misc\Def.h>

class stStringW;

class FontInfoImpl {
private:
	FT_Library m_tFTLib;
	FT_Face m_tFace;

public:
	FontInfoImpl( stStringW * strFontFilePathW, in_un32 unFontPixelSize );
	virtual ~FontInfoImpl();
};