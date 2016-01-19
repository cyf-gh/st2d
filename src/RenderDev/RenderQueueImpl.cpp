#include "RenderQueueImpl.h"

////////////////////////////////////////////////////////////////////////////////

stResult stRenderQueueTextureImpl::RenderSingle( IRenderQueueManager * pRdrQueueManager ) {
	if ( st_do_err( setWorldTransform( pRdrQueueManager ) ) )
		return ST_ERR_SETWORLDTRANSFORM;
	if ( st_do_err( ( ( IGraphics2D * ) pRdrQueueManager->GetRenderHandle( GetGenre() ) )->DrawQuad( m_pTexture, m_unCurrentSerial ) )  )
		return ST_ERR_RENDERSINGLE;
	return ST_ERR_OK;
}

////////////////////////////////////////////////////////////////////////////////

stResult stRenderQueueRectangleImpl::RenderSingle( IRenderQueueManager * pRdrQueueManager ) {
	if ( st_do_err( setWorldTransform( pRdrQueueManager ) ) )
		return ST_ERR_SETWORLDTRANSFORM;
	if ( st_do_err( 
		getRenderDevice( pRdrQueueManager )->DrawQuad( NULL, m_unCurrentSerial ) )  )
		return ST_ERR_RENDERSINGLE;
	return ST_ERR_OK;
}

////////////////////////////////////////////////////////////////////////////////

stResult stRenderQueueFontImpl::RenderSingle( IRenderQueueManager * pRdrQueueManager ) {
	( ( IFontRenderer * ) pRdrQueueManager->GetRenderHandle( GetGenre() ) )->PreLoad( m_strW, m_unCharCount );
	if ( st_do_err( 
		( ( IFontRenderer * ) pRdrQueueManager->GetRenderHandle( GetGenre() ) )->Draw( 
		m_strW, 
		GetVisiableCharCount(),
		& m_tRect, 
		m_tColor,
		m_dFormat ) ) ) return ST_ERR_RENDERSINGLE;
	return ST_ERR_OK;
}

