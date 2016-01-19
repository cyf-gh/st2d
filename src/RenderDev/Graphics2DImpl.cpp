#include <malloc.h>
#include <stlib\Misc\Def.h>
#include "..\MathsExtend\Vertex.h"
#include "Graphics2DImpl.h"

////////////////////////////////////////////////////////////////////////////////

void stGraphics2DImpl::coord::fixRect3D() {
	m_tRect3D = stRect( -( m_fZDeep * m_fScreenRatio ), m_fZDeep * m_fScreenRatio, m_fZDeep, -m_fZDeep );
}

stGraphics2DImpl::stQuad stGraphics2DImpl::coord::To3D( KREF( stRect ) tRectScreen ) {
	fixRect3D();
	f32 fXRatio = m_tRect3D.GetWidth() / m_tRectScreen.GetWidth();
	f32 fYRatio = ( m_tRect3D.GetCat1().fY - m_tRect3D.GetCat2().fY ) / m_tRectScreen.GetHeight();

	return stGraphics2DImpl::stQuad(
		( tRectScreen.GetCat1().fX - m_tRectScreen.GetCenter().fX ) * fXRatio,
		-( tRectScreen.GetCat1().fY - m_tRectScreen.GetCenter().fY ) * fYRatio,
		  tRectScreen.GetWidth() * fXRatio,
		  tRectScreen.GetHeight() * fYRatio );
}

stVertex2D stGraphics2DImpl::coord::To3DPoint( KREF( stPoint ) tPoint ) {
	fixRect3D();
	f32 fXRatio = m_tRect3D.GetWidth() / m_tRectScreen.GetWidth();
	f32 fYRatio = ( m_tRect3D.GetCat1().fY - m_tRect3D.GetCat2().fY ) / m_tRectScreen.GetHeight();

	return stVertex2D( 
		( tPoint.GetX() - m_tRectScreen.GetCenter().fX ) * fXRatio,
		-(tPoint.GetY() - m_tRectScreen.GetCenter().fY ) * fYRatio,
		0.f, 0.f );
}

stRect stGraphics2DImpl::coord::ToScreen( KREF( stGraphics2DImpl::stQuad ) tQuad3D ) {
	fixRect3D();
	f32 fXRatio = m_tRectScreen.GetWidth() / m_tRect3D.GetWidth();
	f32 fYRatio = m_tRectScreen.GetHeight() / ( m_tRect3D.GetCat1().fY - m_tRect3D.GetCat2().fY );

	return stRect( 
		( tQuad3D.GetCat1().fX + tQuad3D.GetWidth() ) * fXRatio,
		( tQuad3D.GetCat1().fY + tQuad3D.GetHeight() ) * fXRatio,
		  tQuad3D.GetWidth() * fXRatio,
		  tQuad3D. GetHeight() * fYRatio );
}

////////////////////////////////////////////////////////////////////////////////

stGraphics2DImpl::stGraphics2DImpl( stRenderDevImpl * pDev, in_un32 nMaxVertexBuf, in_un32 nMaxIndexBuf ) 
	: stGraphicsImpl( pDev ), 
	m_unMaxVerticesCount( nMaxVertexBuf ), m_unMaxIndicesCount( nMaxIndexBuf ) {
	m_pVertices = new stVertex::Vertex[ nMaxVertexBuf ];
	m_pIndices = new WORD[ nMaxIndexBuf ];
	m_pCoord = new coord( pDev->GetBufferRatio(), 1.0f, stRect( 0.0f, ( f32 )pDev->GetBufWidth(), 0.0f, ( f32 )pDev->GetBufHeight() ) );
	m_pDevice->CreateVertexBuffer( 
		sizeof( stVertex::Vertex ) * m_unMaxVerticesCount, 
		D3DUSAGE_WRITEONLY,
		stVertex::FVF,
		D3DPOOL_MANAGED,
		& m_pVertexBuf,
		NULL 
	);
	m_pDevice->CreateIndexBuffer( 
		sizeof( un16 ) * m_unMaxIndicesCount,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		& m_pIndexBuf,
		NULL 
	);
	fixProjTransform();
	fixViewTransform();
}

stGraphics2DImpl::~stGraphics2DImpl() { 
	st_safe_del( m_pCoord );
	st_safe_del( m_pVertices );
	st_safe_del( m_pIndices );
	st_safe_rel( m_pIndexBuf );
	st_safe_rel( m_pVertexBuf );
}

void stGraphics2DImpl::fixProjTransform( void ) {
	SetProjTransform( m_pCoord->GetScreenRatio(), 0.0f, 1.0f );
}

void stGraphics2DImpl::fixViewTransform( void ) {
	SetViewTransform( 
		& D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    
		& D3DXVECTOR3 (0.0f, 0.0f, 1.0f),  
		& D3DXVECTOR3 (0.0f, 1.0f, 0.0f) 
	);
	m_pCoord->SetZDeep( 1.0f );
}

stResult stGraphics2DImpl::ColRects( KREF( stRect ) tAppendRect, in_un32 nCurrentTarget, QuadColor * pQuadColor ) {	
	static const un16 indice[] = { 0, 1, 2, 0, 2, 3 };
	const un32 unCurrentVerticesIndex = nCurrentTarget * 4;

	if ( nCurrentTarget * 4 > m_unMaxVerticesCount )
		return ST_ERR_NOMOREVERTEXALLOC;
	stQuad tQuad = m_pCoord->To3D( tAppendRect );
	for ( un32 i = 0; i < 4; ++i ) {
		if ( pQuadColor != NULL ) {
			tQuad.Data( i ).color = pQuadColor->GetColor( i );
		}
		* ( m_pVertices + unCurrentVerticesIndex + i ) = tQuad.Data( i );
	}
	for ( un32 i = 0; i < 6; ++i ) {
		* ( m_pIndices + nCurrentTarget * 6 + i ) = indice[ i ] + unCurrentVerticesIndex;
	}
	return ST_ERR_OK;
}

stResult stGraphics2DImpl::PushIndexBuffer() {
	void * pInd = NULL;

	if ( FAILED( m_pIndexBuf->Lock( 0, 0, ( void ** )& pInd, 0 ) ) )
		return ST_ERR_INDEXBUFFERLOCK;
	{
		memcpy( pInd, m_pIndices, _msize( m_pIndices ) );
	}
	if ( FAILED( m_pIndexBuf->Unlock() ) )
		return ST_ERR_INDEXBUFFERUNLOCK;
	return ST_ERR_OK;
}

stResult stGraphics2DImpl::PushVertexBuffer() {
	void * pVerticesData = NULL;

	if ( FAILED( m_pVertexBuf->Lock( 0, 0, ( void ** )& pVerticesData, 0 ) ) ) 
		return ST_ERR_VERTEXBUFFERLOCK;
	{
		memcpy( pVerticesData, m_pVertices, _msize( m_pVertices ) );
	}
	if ( FAILED( m_pVertexBuf->Unlock() ) )
		return ST_ERR_VERTEXBUFFERUNLOCK;
	return ST_ERR_OK;
}

stResult stGraphics2DImpl::DrawQuad( ITexture * pTexture, in_un32 nRenderQueue ) {
	static ITexture * pTmpTex = NULL;
	
	pTmpTex = pTexture;
	if ( FAILED( m_pDevice->SetStreamSource( 0, m_pVertexBuf, 0, sizeof( stVertex::Vertex ) ) ) ) 
		return ST_ERR_SETSTREAMSOURCE;
	if ( FAILED( m_pDevice->SetIndices( m_pIndexBuf ) ) )
		return ST_ERR_SETINDIECES;
	if ( pTexture != NULL ) {
		if ( FAILED( m_pDevice->SetTexture( 0, ( IDirect3DTexture9 * )pTexture->GetHandle() ) ) )
			return ST_ERR_SETTEXTURE;
	} else {
		if ( FAILED( m_pDevice->SetTexture( 0, NULL ) ) )
			return ST_ERR_SETTEXTURE;
	}
	if ( FAILED( m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, nRenderQueue * 6, 2 ) ) )
		return ST_ERR_DRAWINDEXEDPRIMITIVE;
	return ST_ERR_OK;
}

stResult stGraphics2DImpl::DrawNormal( in_un32 nVerticesCount, in_un32 nTriangleCount, in_un32 nRenderQueue ) {
	if ( FAILED( m_pDevice->SetStreamSource( 0, m_pVertexBuf, 0, sizeof( stVertex::Vertex ) ) ) ) 
		return ST_ERR_SETSTREAMSOURCE;
	if ( FAILED( m_pDevice->SetIndices( m_pIndexBuf ) ) )
		return ST_ERR_SETINDIECES;
	if ( FAILED( m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, nVerticesCount, nRenderQueue * 6, nTriangleCount ) ) )
		return ST_ERR_DRAWINDEXEDPRIMITIVE;
	return ST_ERR_OK;
}
