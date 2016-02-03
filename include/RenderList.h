#ifndef __ST2D_RENDERLIST_H__
#define __ST2D_RENDERLIST_H__

#include <core\Def.h>

namespace st2d {

struct stElementInfo {
	const un32 CountV;
	const un32 CountI;
	const un32 CountEX;
	const un32 PrimtiveGenre;
	const un32 PrimCount;
};

st_interface( stRenderListElement ) {
public:
	virtual const stElementInfo &Info() = 0;

	/* vertex */
	virtual const void *DataV() = 0;

	/* index */
	virtual const un16 *DataI() = 0;

	/* ex interface */
	virtual const un32 DataEX( const un32 index, void **pphandle ) = 0;

	virtual stRenderListElement &SetEX( const un32 index, void *phandle ) = 0;
};

}

#endif /* !__ST2D_RENDERLIST_H__ */