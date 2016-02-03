#ifndef __ST2D_ANIMATION_H__
#define __ST2D_ANIMATION_H__

#include <d3d9.h>
#include <core\Def.h>

namespace st2d {

st_interface( stAnimation ) {
public:
	virtual D3DMATRIX *Handle() = 0;
};

} /* st2d */

#endif /* !__ST2D_ANIMATION_H__ */