#ifndef __ST2D_VERTEX_H__
#define __ST2D_VERTEX_H__

#include <d3d9.h>
#include <core\Def.h>

namespace st2d {

namespace stVertex {

struct Vertex {
	f32		x, y, z;
	f32		nx, ny, nz;
	f32		u, v;
	un32	color;
};

static const un32 s_textureFVF = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 ); 

static ST_INLINE void normal_vertex( Vertex *pvertex ) {
	pvertex->x = 1.0f;
	pvertex->y = 1.0f;
	pvertex->z = 1.0f;
	
	pvertex->nx = 0.0f;
	pvertex->ny = 0.0f;
	pvertex->nz = -1.0f;

	pvertex->u = 0.0f;
	pvertex->v = 0.0f;

	pvertex->color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

}

} /* st2d */

#endif // !__ST2D_VERTEX_H__

