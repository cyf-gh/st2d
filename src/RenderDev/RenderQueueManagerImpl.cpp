#include <stlib\String\base.h>
#include "..\RenderObject.h"	
#include "RenderQueueManagerImpl.h"

////////////////////////////////////////////////////////////////////////////////

stResult stRenderQueueManagerImpl::pushVertexToDev( IRenderQueue * pRdQueue ) {
	IRectangle * pCurrentRdQueueRectangle = ( IRectangle * )( ( IRenderQueueRectangle * )pRdQueue )->GetRectangle();
	un32 unIncrease = 0;

	( ( IRenderQueueRectangle * )pRdQueue )->SetCurrentSerial( m_unCurrentRenderQueue );
	if ( pCurrentRdQueueRectangle->IsRepeated() ) {
		unIncrease = pCurrentRdQueueRectangle->GetRectCount();
		for ( un32 i = 0; i < unIncrease; ++i ) {
			if( ST_ERR_NOMOREVERTEXALLOC == m_pGrap2D->ColRects( pCurrentRdQueueRectangle->GetRect( i ), m_unCurrentRenderQueue, & ( pCurrentRdQueueRectangle->GetQuadColor() ) ) )
				return ST_ERR_NOMOREVERTEXALLOC;
		}
	} else {
		unIncrease = 1;
		if( ST_ERR_NOMOREVERTEXALLOC == m_pGrap2D->ColRects( pCurrentRdQueueRectangle->GetRect( 0 ), m_unCurrentRenderQueue, & ( pCurrentRdQueueRectangle->GetQuadColor() ) ) )
			return ST_ERR_NOMOREVERTEXALLOC;
	}	
	m_unCurrentRenderQueue += unIncrease;
	return ST_ERR_OK_INTERNAL;
}

stResult stRenderQueueManagerImpl::Attach( IRenderQueue * pRdQueue ) {
	if ( NULL == pRdQueue )
		return ST_ERR_NULLRENDERQUEUE;
	switch ( pRdQueue->GetGenre() ) {
	case ST_RENDERQUEUE_TEXTURE : {
		if ( NULL == ( ( IRenderQueueTexture * )pRdQueue )->GetTexture() )
			return ST_ERR_NULLRENDERQUEUETEXTURE;
		if ( st_do_err( pushVertexToDev( pRdQueue ) ) )
			return ST_ERR_INTERNAL_PUSHVERTEXTODEV;
	} 
	break;
	case ST_RENDERQUEUE_RECTANGLE : {
		if ( NULL == ( ( IRenderQueueRectangle * )pRdQueue )->GetRectangle() )
			return ST_ERR_NULLREDNERQUEUERECTANGLE;
		if ( st_do_err( pushVertexToDev( pRdQueue ) ) )
			return ST_ERR_INTERNAL_PUSHVERTEXTODEV;			
	} 
	break;
	case ST_RENDERQUEUE_FONT : {
		/* do nothing so far */
	}
	break;
	default :
		return ST_ERR_UNKNOWNRENDERQUEUEGENRE; // no such genre render queue
	}
	m_vecRenderQueue.push_back( pRdQueue );
	return ST_ERR_OK;
}

stResult stRenderQueueManagerImpl::Detach( IRenderQueue * pRdQueue ) {
	std::vector< IRenderQueue * >::iterator itQueue = m_vecRenderQueue.begin();
	
	while( itQueue != m_vecRenderQueue.end() ) {
		if ( pRdQueue == ( * itQueue ) ) {
			m_vecRenderQueue.erase( itQueue );
			break;
			return ST_ERR_OK;
		}
	}
	return ST_ERR_DETACHNOTFOUND;
}

stResult stRenderQueueManagerImpl::Refresh() {
	std::vector<IRenderQueue *>::iterator itQueue = m_vecRenderQueue.begin();
	
	/// --- reload render queue index ---
	m_unCurrentRenderQueue = 0; 
	while( itQueue != m_vecRenderQueue.end() ) {
		if ( ( * itQueue )->GetGenre() == ST_RENDERQUEUE_FONT ) {
			++itQueue;
			break;
		}
		if ( st_do_err( pushVertexToDev( * itQueue ) ) )
			return ST_ERR_INTERNAL_PUSHVERTEXTODEV;
		++itQueue;
	}
	m_pGrap2D->PushVertexBuffer();
	m_pGrap2D->PushIndexBuffer();
	return ST_ERR_OK;
}

void stRenderQueueManagerImpl::RemoveAll() {
	m_vecRenderQueue.clear();
}

stResult stRenderQueueManagerImpl::Render() {
	std::vector<IRenderQueue *>::iterator itQueue = m_vecRenderQueue.begin();
	m_pGrap2D->Clear();
	m_pGrap2D->Begin();
	while( itQueue != m_vecRenderQueue.end() ) {
		if ( ( * itQueue )->IsVisiable() )
			( * itQueue )->RenderSingle( this );
		++itQueue;
	}
	m_pGrap2D->End();
	m_pGrap2D->Present( NULL, NULL );
	return ST_ERR_OK;
}

void * stRenderQueueManagerImpl::GetRenderHandle( const n32 nGenre ) {
	switch ( nGenre ) {
		case ST_RENDERQUEUE_TEXTURE : return m_pGrap2D;
		case ST_RENDERQUEUE_RECTANGLE : return m_pGrap2D;
		case ST_RENDERQUEUE_FONT : return m_pFontRdr;
		default: return NULL; // unknown render queue genre
	}
}

void * stRenderQueueManagerImpl::FindWithTag( const wchar_t *strTagW ) {
	for ( std::vector< IRenderQueue * >::iterator it = m_vecRenderQueue.begin(); 
		  it != m_vecRenderQueue.end(); ++it ) {
		if ( st_wcs_base::wcsIsEqual( ( * it )->GetTag(), strTagW ) )
			return * it;	
	}
	return NULL;
}