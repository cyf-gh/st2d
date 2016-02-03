#ifndef __ST2D_GRAPHICSIMPL_H__
#define __ST2D_GRAPHICSIMPL_H__

#include "..\include\ErrCode.h"
#include "..\include\Device.h"
#include "..\include\Graphics.h"

namespace st2dImpl {

template<typename T>
class GraphicsImpl : public T {

protected:
	IDirect3DDevice9 *		m_pdevice;
	bool					m_isRendering;

public:
	
	stResult Begin( const un32 FVF ) {
		if ( FAILED( m_pdevice->BeginScene() ) ) {
			stCore::st_core_return_with_var( st2dErrCode::ST_ERR_RENDERBEGIN, st2dErrCode::ST_ERR_RENDERBEGIN );
		}
		m_pdevice->SetFVF( FVF );
		m_isRendering = true;
		stCore::st_core_return_with_var( ST_NOERR, ST_NOERR );
	}
	
	stResult End() {
		if ( FAILED( m_pdevice->EndScene() ) ) {
			return 0;
		}
		m_isRendering = false;
		stCore::st_core_return_with_var( ST_NOERR, ST_NOERR );
	}
	
	void Flush() {
		m_isRendering = false;
	}

	stResult Present( stLibUtils::Maths::stRect *psrc, stLibUtils::Maths::stRect *pdest ) {
		static RECT * src = NULL;
		static RECT * dest = NULL;

		src  = ( psrc  != NULL ) ? ( & psrc->ToRECT() )  : NULL;
		dest = ( pdest != NULL ) ? ( & pdest->ToRECT() ) : NULL;
		if( FAILED( m_pdevice->Present( src, dest, NULL, NULL ) ) ) {
			stCore::st_core_return_with_var( st2dErrCode::ST_ERR_PRESENT, st2dErrCode::ST_ERR_PRESENT );
		}
		stCore::st_core_return_with_var( ST_NOERR, ST_NOERR );
	}

	stResult Clear( 
		const stLibUtils::Maths::stRect *rect	   = NULL, 
		const un32						clearFlags = D3DCLEAR_TARGET, 
		const D3DCOLOR &				color	   = D3DCOLOR_ARGB( 255, 0, 0, 0 ), 
		const f32						z		   = 1.0f ) {
		
		return ( NULL == rect ) ? 
			m_pdevice->Clear( 0, NULL, clearFlags, color, z, 0 ) : 
			m_pdevice->Clear( 0, &( ( D3DRECT * )rect->ToRECT() ), clearFlags, color, z, 0 );
	}

	stResult SetViewTransform( D3DXVECTOR3 * ppos, D3DXVECTOR3 * pviewAt, D3DXVECTOR3 * pup ) {
		D3DXMATRIX view;

		D3DXMatrixLookAtLH( &view, ppos, pviewUp, pup );    
		if ( FAILED( m_pdevice->SetTransform( D3DTS_VIEW, &view ) ) ) {
			stCore::st_core_return_with_var( st2dErrCode::ST_ERR_TRVIEW, st2dErrCode::ST_ERR_TRVIEW );
		}
		stCore::st_core_return_with_var( ST_NOERR, ST_NOERR );
	}
	
	stResult SetProjTransform( f32 ratio, f32 near, f32 far ) {
		D3DXMATRIX matProjection;  

		D3DXMatrixPerspectiveFovLH( &matProjection, D3DXToRadian( 90 ), ratio, near, far );  
		if ( FAILED( m_pdevice->SetTransform( D3DTS_PROJECTION, & matProjection ) ) ) {
			stCore::st_core_return_with_var( st2dErrCode::ST_ERR_TRPROJ, st2dErrCode::ST_ERR_TRPROJ );
		}
		stCore::st_core_return_with_var( ST_NOERR, ST_NOERR );
	}

	stResult SetWorldTransform( const D3DXMATRIX &world ) {
		if ( FAILED( m_pdevice->SetTransform( D3DTS_WORLD, &world ) ) ) {
			stCore::st_core_return_with_var( st2dErrCode::ST_ERR_TRWORLD, st2dErrCode::ST_ERR_TRWORLD );
		}
		stCore::st_core_return_with_var( ST_NOERR, ST_NOERR );
	}

	GraphicsImpl( st2d::stDevice *pdev ) : m_pdevice( ( IDirect3DDevice9 * )pdev->GetHandle() ), m_isRendering( false ) { }
	virtual ~GraphicsImpl() { }
};

} /* st2dImpl */

#endif /* !__ST2D_GRAPHICSIMPL_H__ */