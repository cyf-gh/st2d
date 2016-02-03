#ifndef __ST2D_GRAPHICS_H__ 
#define __ST2D_GRAPHICS_H__ 

#include <d3d9.h>
#include <d3dx9.h>

#include <core\core.h>
#include <utils\Maths.h>

#include "Vertex.h"
#include "Texture.h"
#include "Animation.h"

namespace st2d {

st_interface( stGraphics ) {
	virtual stResult Begin( const un32 FVF ) = 0;
	virtual stResult End() = 0;
	virtual void Flush() = 0;
	virtual stResult Present( stLibUtils::Maths::stRect *psrc, stLibUtils::Maths::stRect *pdest ) = 0;
	virtual stResult Clear(
		const stLibUtils::Maths::stRect *rect	   = NULL, 
		const un32						clearFlags = D3DCLEAR_TARGET, 
		const D3DCOLOR &				color	   = D3DCOLOR_ARGB( 255, 0, 0, 0 ), 
		const f32						z		   = 1.0f ) = 0;
	virtual stResult SetViewTransform( D3DXVECTOR3 * ppos, D3DXVECTOR3 * pviewAt, D3DXVECTOR3 * pup ) = 0;
	virtual stResult SetProjTransform( f32 ratio, f32 near, f32 far ) = 0;
	virtual stResult SetWorldTransform( const D3DXMATRIX &world ) = 0;	
};

st_interface( stGraphics2D ) : st_interface_public_inherit( stGraphics ) {

};

} /* st2d */

#endif /* !__ST2D_GRAPHICS_H__ */