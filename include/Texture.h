#ifndef __ST2D_TEXTURE_H__
#define __ST2D_TEXTURE_H__

#include <d3d9.h>
#include <core\Def.h>

namespace st2d {

st_interface( stTexture ) {
public:

virtual IDirect3DTexture9 *Handle() = 0;

};

} /* st2d */

#endif /* !__ST2D_TEXTURE_H__ */