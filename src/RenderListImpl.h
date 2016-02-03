#ifndef __ST2D_RENDERLISTIMPL_H__
#define __ST2D_RENDERLISTIMPL_H__

#include <vector>

#include <utils\Maths.h>

#include "..\include\Texture.h"
#include "..\include\Vertex.h"
#include "..\include\Animation.h"

#include "..\include\RenderList.h"

namespace st2dImpl {

using namespace stLibUtils::Maths;
using namespace st2d::stVertex;

class QuadImpl : public st2d::stRenderListElement {

private:
	Vertex							m_quad[4];
	st2d::stTexture *				m_ptexture;
	st2d::stAnimation *				m_peffect;

public:
	const st2d::stElementInfo &		Info();

	const void *					DataV() { return m_quad; }
	const un16 *					DataI();
	const un32						CountV() { return 4; }
	const un32						CountI() { return 6; }
	
	const un32						DataEX( const un32 index, void ** pphandle );
	const un32						CountEX() { return 2; }
	st2d::stRenderListElement &		SetEX( const un32 index, void *phandle );


	const Vertex &					Data( const un8 index )	const 
										{ return m_quad[index]; }
	stRect							Rect() const			
										{ return stRect( m_quad[0].x, m_quad[0].y, m_quad[3].x, m_quad[3].x ); }
	stVec2							Center() const			
										{ return stVec2( m_quad[1].x + Width(), m_quad[1].y + Height() ); }
	
	f32								Width() const			
										{ return m_quad[2].x - m_quad[0].x; }
	f32								Height() const
										{ return m_quad[1].y - m_quad[0].y; }
	
	QuadImpl &						SetColor( const un32 *pcolor );
	QuadImpl &						Scale( const f32 scale );
	QuadImpl &						ScaleCenter( const f32 scale );

									QuadImpl() { }
									QuadImpl( const stRect &rect ); 
									QuadImpl( const f32 x, const f32 y, const f32 width, const f32 height ); 
									~QuadImpl() { }
};

} /* st2dImpl */

#endif /* !__ST2D_RENDERLISTIMPL_H__ */
