#ifndef __ST2D_GRAPHICS2DIMPL_H__
#define __ST2D_GRAPHICS2DIMPL_H__

#include <vector>

#include "..\include\Vertex.h"
#include "..\include\RenderList.h"

#include "GraphicsImpl.h"

namespace st2dImpl {

class CoordImpl;

class Graphics2DImpl : public GraphicsImpl<st2d::stGraphics> {

protected:
	std::vector<st2d::stRenderListElement*>m_list;

	IDirect3DVertexBuffer9 *			m_pvertexBuf;
	IDirect3DIndexBuffer9 *				m_pindexBuf;

	un16 *								m_pindices;

	st2d::stVertex::Vertex *			m_pvertices;

	const un32							m_maxIndicesCount;
	const un32							m_maxVerticesCount;
	
	CoordImpl *							m_ptrans;

private:
	void								fixAll();

public:
	Graphics2DImpl &					PushBuf();
	
	void								Render( n32 startEleIndex = 0, n32 endEleIndex = -1 );

	Graphics2DImpl &					AddElement( st2d::stRenderListElement *pelement );
	Graphics2DImpl &					RemoveElement( st2d::stRenderListElement *pelement );

	st2d::stRenderListElement *			CreateQuad( const stLibUtils::Maths::stRect &screen, st2d::stTexture *ptexture, st2d::stAnimation *panimation ); 
	st2d::stGraphics2D *				ReleaseQuad( st2d::stRenderListElement **ppquad );

										Graphics2DImpl( st2d::stDevice *pdev, const un32 maxVerticesBuf, const un32 maxIndicesBuf, const f32 bufWidth, const f32 bufHeight, const f32 bufRatio  );
	virtual								~Graphics2DImpl();
};

} /* st2dImpl */

#endif /* !__ST2D_GRAPHICS2DIMPL_H__ */
