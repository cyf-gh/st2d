/// ----------------------------------------------------------------------------
/// \file RenderQueueManagerImpl.h
/// \breif implement render queue manager
/// ----------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <vector>
#include "..\RenderDev.h"

class stRenderQueueManagerImpl;

/// ----------------------------------------------------------------------------
/// \brief render queue manager implements
/// ----------------------------------------------------------------------------
class stRenderQueueManagerImpl : public IRenderQueueManager {
protected:
	std::vector< IRenderQueue * > m_vecRenderQueue;
	IGraphics2D * m_pGrap2D;			///< \brief render device
	IFontRenderer * m_pFontRdr;		///< \brief font render device

	un32 m_unCurrentRenderQueue;		///< \brief current position of vertex device in render device

protected:
	stResult pushVertexToDev( IRenderQueue * pRdQueue ); ///< \brief push the vertices of quads of texture of render queue to graphics device

public:
	void * FindWithTag( const wchar_t *strTagW );

	stResult Attach( IRenderQueue * pRdQueue );
	stResult Detach( IRenderQueue * pRdQueue );
	void RemoveAll();
	
	stResult Refresh();
	stResult Render( void );
	
	void * GetRenderHandle( const n32 nGenre );
	
	stRenderQueueManagerImpl::stRenderQueueManagerImpl( IGraphics2D * pRenderer, IFontRenderer * pFontRdr ) 
		: m_pGrap2D( pRenderer ), m_unCurrentRenderQueue( 0 ), m_pFontRdr( pFontRdr ) { } 
	virtual stRenderQueueManagerImpl::~stRenderQueueManagerImpl() { } 
};
