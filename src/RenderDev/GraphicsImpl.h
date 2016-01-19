/// ----------------------------------------------------------------------------
/// \file GraphicsImpl.h
/// \brief Graphics implements
/// ----------------------------------------------------------------------------
#pragma once

#include "..\RenderDev.h"

/// ----------------------------------------------------------------------------
/// \brief Graphics implements
/// ----------------------------------------------------------------------------
template< typename T >
class stGraphicsImpl : public T {
protected:
	IDirect3DDevice9 * m_pDevice;
	bool m_IsOnRender;

protected:
	void setRenderState( bool IsOnRender ) { m_IsOnRender = IsOnRender; }

public:
	stResult Begin() {
		if ( FAILED( m_pDevice->BeginScene() ) ) 
			return 0;
		m_pDevice->SetFVF( stVertex::FVF );
		setRenderState( true );
		return 0;
	}
	stResult End() {
		if ( FAILED( m_pDevice->EndScene() ) ) 
			return 0;
		setRenderState( false );
		return 0;
	}
	stResult Flush() {
		setRenderState( false );
		return 0;
	}
	stResult Present( stRect * pSrcRect, stRect * pDestRect ) {
		static RECT * rectSrc = NULL;
		static RECT * rectDest = NULL;

		rectSrc = ( pSrcRect != NULL ) ? ( & pSrcRect->ToRECT() ) : NULL;
		rectDest = ( pDestRect != NULL ) ? ( & pDestRect->ToRECT() ) : NULL;
		if( FAILED( m_pDevice->Present( rectSrc, rectDest, NULL, NULL ) ) ) 
			return ST_ERR_PRESENT;
		return ST_ERR_OK;	
	}
	stResult Clear( KREF( D3DCOLOR ) tColor = D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) {
		return m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, tColor, 1.0f, 0 );
	}

	stResult SetViewTransform( D3DXVECTOR3 * pPos, D3DXVECTOR3 * pViewAt, D3DXVECTOR3 * pUp ) {
		D3DXMATRIX matView;

		D3DXMatrixLookAtLH( &matView, pPos, pViewAt, pUp );    
		if ( FAILED( m_pDevice->SetTransform( D3DTS_VIEW, & matView ) ) ) 
			return ST_ERR_SETVIEWTRANSFORM;
		return ST_ERR_OK;
	}
	stResult SetProjTransform( in_f32 fRatio, in_f32 fNear, in_f32 fFar ) {
		D3DXMATRIX matProjection;  

		D3DXMatrixPerspectiveFovLH( &matProjection, D3DXToRadian( 90 ), fRatio, fNear, fFar );  
		if ( FAILED( m_pDevice->SetTransform( D3DTS_PROJECTION, & matProjection ) ) )
			return ST_ERR_SETPROJTRANSFORM;
		return ST_ERR_OK;	
	}
	stResult SetWorldTransform( KREF( D3DXMATRIX ) tMatrix ) {
		if ( FAILED( m_pDevice->SetTransform( D3DTS_WORLD, & tMatrix ) ) )
			return ST_ERR_SETWORLDTRANSFORM;
		return ST_ERR_OK;
	}

	stGraphicsImpl( IRenderDevice * pDev ) 
		: m_pDevice( ( IDirect3DDevice9 * )pDev->GetHandle() ), m_IsOnRender( false ) { }
	virtual ~stGraphicsImpl() { }
};

