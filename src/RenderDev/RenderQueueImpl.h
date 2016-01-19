/// ----------------------------------------------------------------------------
/// \file stRenderQueueTextureImpl.h
/// \brief render queue implements
/// ----------------------------------------------------------------------------
#pragma once

#include <d3d9.h>
#include <stlib\String\StringImpl.h>
#include "..\RenderDev.h"
#include "..\EffectAnimation.h"
#include "..\MathsExtend\Matrix.h"
#include "..\MathsExtend\Rect.h"

class stRenderQueueManagerImpl;
class stRenderQueueTextureImpl;
class stRenderQueueRectangleImpl;
class stRenderQueueFontImpl;

/// ----------------------------------------------------------------------------
/// \brief base render queue interface implement
/// ----------------------------------------------------------------------------
template< typename T >
class RenderQueueImpl : public T {
private:
	stStringW * m_strTagW;	
	bool m_IsVisiable;

protected:
	IEffectAnimation * m_pAnimation;
	const bool m_kIsDynamic;

	IGraphics2D * getRenderDevice( IRenderQueueManager * pRdrQueueManager ) { return ( IGraphics2D * ) ( pRdrQueueManager->GetRenderHandle( GetGenre() ) ); }
	n32 setWorldTransform( IRenderQueueManager * pRdrQueueManager ) {
		if ( m_kIsDynamic ) {
			if ( st_do_err( getRenderDevice( pRdrQueueManager )->SetWorldTransform( m_pAnimation->GetMatrix() ) ) ) 
				return ST_ERR_INTERNAL_SETWORLDTRANSFORM;
		} else {
			if ( st_do_err( getRenderDevice( pRdrQueueManager )->SetWorldTransform( stMatrixHelper::ktIdentityMatrix ) ) ) 
				return ST_ERR_INTERNAL_SETWORLDTRANSFORM;			
		}
		return ST_ERR_OK;
	}

public:
	wchar_t *  GetTag() const { return m_strTagW->GetString(); }	
	void SetTag( const wchar_t *strTagW ) { m_strTagW->SetString( strTagW ); }

	bool IsVisiable() const { return m_IsVisiable; }
	bool IsDynamic() const { return m_kIsDynamic; }
	
	void SetVisiable( bool IsVisiable ) { m_IsVisiable = IsVisiable; }
	void SetAnimation( void * pAnimationEffect ) { if ( m_kIsDynamic ) m_pAnimation = ( IEffectAnimation * ) pAnimationEffect; }

	RenderQueueImpl( const wchar_t *strTagW, bool kIsDynamic, bool IsVisiable ) 
		: m_strTagW( new stStringW( strTagW ) ), m_kIsDynamic( kIsDynamic ), m_IsVisiable( IsVisiable ), m_pAnimation( NULL ) { }
	virtual ~RenderQueueImpl() { 
		st_safe_del( m_strTagW );
	}
};

/// ----------------------------------------------------------------------------
/// \brief render queue for texture implements
/// ----------------------------------------------------------------------------
class stRenderQueueTextureImpl : public RenderQueueImpl< IRenderQueueTexture > {
protected:
	ITexture * m_pTexture;
	IRectangle * m_pRectangle;
	un32 m_unCurrentSerial;

public:
	n32 GetGenre() const { return ST_RENDERQUEUE_TEXTURE; }
	stResult RenderSingle( IRenderQueueManager * pRdrQueueManager );

	void SetRectangle( IRectangle * pRectangle ) { m_pRectangle = pRectangle; }
	void * GetRectangle() { return m_pRectangle; }

	void SetTexture( ITexture * pTexture ) { m_pTexture = pTexture; }
	void * GetTexture() { return m_pTexture; }
	
	void SetCurrentSerial( in_un32 unCurrentSerial ) { m_unCurrentSerial = unCurrentSerial; }

	stRenderQueueTextureImpl( const wchar_t *strTagW, ITexture * pTexture, IRectangle * pRectangle, bool IsVisiable, bool IsDynamic = false ) 
		: RenderQueueImpl( strTagW, IsDynamic, IsVisiable ), m_pTexture( pTexture ), m_pRectangle( pRectangle ) { }
	virtual ~stRenderQueueTextureImpl() { } 
};

/// ----------------------------------------------------------------------------
/// \brief render queue for font rendering
/// ----------------------------------------------------------------------------
class stRenderQueueRectangleImpl : public RenderQueueImpl< IRenderQueueRectangle > {
protected:
	IRectangle * m_pRectangle;
	un32 m_unCurrentSerial;
	
public:
	n32 GetGenre() const { return ST_RENDERQUEUE_RECTANGLE; }
	stResult RenderSingle( IRenderQueueManager * pRdrQueueManager );

	void SetRectangle( IRectangle * pRectangle ) { m_pRectangle = pRectangle; }
	void * GetRectangle() { return m_pRectangle; } 
	
	void SetCurrentSerial( in_un32 unCurrentSerial ) { m_unCurrentSerial = unCurrentSerial; }

	stRenderQueueRectangleImpl( const wchar_t *strTagW, IRectangle * pRectangle, bool IsVisiable, bool IsDynamic = false ) 
		: RenderQueueImpl( strTagW, IsDynamic, IsVisiable ), m_pRectangle( pRectangle ) { }
	virtual ~stRenderQueueRectangleImpl() { }
};

/// ----------------------------------------------------------------------------
/// \brief render queue for font rendering
/// ----------------------------------------------------------------------------
class stRenderQueueFontImpl : public RenderQueueImpl< IRenderQueueFont > {
protected:
	wchar_t *  m_strW;
	un32 m_unCharCount;
	D3DCOLOR m_tColor;
	stRect m_tRect;
	DWORD m_dFormat;

public:
	n32 GetGenre() const { return ST_RENDERQUEUE_FONT; }
	stResult RenderSingle( IRenderQueueManager * pRdrQueueManager );
	
	D3DCOLOR * GetColor() { return & m_tColor; }

	wchar_t *  GetText() const { return m_strW; }
	un32 GetVisiableCharCount() const { return m_unCharCount; }
	
	void SetCharCount( in_un32 unCount ) { m_unCharCount = unCount; }
	void SetColor( KREF( D3DCOLOR ) tColor ) { m_tColor = tColor; }
	
	stRenderQueueFontImpl( const wchar_t *strTagW, wchar_t *  strW, KREF( D3DCOLOR ) tColor, KREF( stRect )tRect, in_ul32 dFormat, bool IsVisiable, bool IsDynamic = false ) 
		: RenderQueueImpl( strTagW, IsDynamic, IsVisiable ), m_strW( strW ), m_unCharCount( 0 ), m_tColor( tColor ), m_tRect( tRect ), m_dFormat( dFormat ) { }
	virtual ~stRenderQueueFontImpl() { }
};