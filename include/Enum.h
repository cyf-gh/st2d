#ifndef __ST2D_ENUM_H__
#define __ST2D_ENUM_H__

namespace st2dEnum {

enum AA {
	ST_AA_NONE					= 0,
	ST_AA_2X					= 2,
	ST_AA_4X					= 4,
	ST_AA_8X					= 8,
	ST_AA_16X					= 16
};

enum WINSTYLE {
	ST_CLIPCHILDREN	          = 0x00000000L,
	ST_MINIMIZE				  = 0x20000000L,
	ST_DISABLED				  = 0x08000000L,
	ST_MAXIMIZE               = 0x01000000L,
	ST_CAPTION				  = 0x00C00000L,     /* ST_BORDER | ST_DLGFRAME  */
	ST_BORDER				  = 0x00800000L,
	ST_DLGFRAME				  = 0x00400000L,
	ST_VSCROLL				  = 0x00200000L,
	ST_HSCROLL				  = 0x00100000L,
	ST_SYSMENU				  = 0x00080000L,
	ST_THICKFRAME			  = 0x00040000L,
	ST_GROUP				  = 0x00020000L,
	ST_TABSTOP				  = 0x00010000L,

	ST_MINIMIZEBOX			  = 0x00020000L,
	ST_MAXIMIZEBOX			  = 0x00010000L
};

}

#endif /* !__ST2D_ENUM_H__ */
