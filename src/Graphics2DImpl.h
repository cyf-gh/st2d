#ifndef __ST2D_GRAPHICS2DIMPL_H__
#define __ST2D_GRAPHICS2DIMPL_H__

#include "GraphicsImpl.h"
#include "..\include\Vertex.h"

namespace st2dImpl {

class Graphics2DImpl : public GraphicsImpl<st2d::stGraphics> {

protected:
	IDirect3DVertexBuffer9 *			m_pvertexBuf;
	IDirect3DIndexBuffer9 *				m_pindexBuf;

	un16 *								m_pindices;
	st2d::stVertex::Vertex *			m_pvertices;
	
	const un32							m_maxIndicesCount;
	const un32							m_maxVerticesCount;

private:
	void								fixAll();

public:
										Graphics2DImpl( st2d::stDevice *pdev, const un32 maxVerticesBuf, const un32 maxIndicesBuf );
	virtual								~Graphics2DImpl();
};

} /* st2dImpl */

#endif /* !__ST2D_GRAPHICS2DIMPL_H__ */
