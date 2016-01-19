/// ----------------------------------------------------------------------------
/// \file Texture.h
/// \brief 2D texture implements
/// ----------------------------------------------------------------------------
#pragma once

#include "..\RenderDev.h"
#include "RectangleImpl.h"

class stRect;

/// ----------------------------------------------------------------------------
/// \brief texture base implements
/// ----------------------------------------------------------------------------
template< typename T >
class TextureImpl : public T {
protected:
	IDirect3DTexture9 * m_pTexture;
	const un32 m_unTextureWidth;
	const un32 m_unTextureHeigth;

public:
	TextureImpl::TextureImpl( IDirect3DTexture9 * pTexture, in_un32 unWidth, in_un32 unHeight ) 
	: m_pTexture( pTexture ), m_unTextureWidth( unWidth ), m_unTextureHeigth( unHeight ) { }
	virtual ~TextureImpl() { }
};

/// ----------------------------------------------------------------------------
/// \brief common texture implements
/// ----------------------------------------------------------------------------
class stTextureImpl : public TextureImpl< ITexture > {
public:
	void * GetHandle() { return m_pTexture; }
	un32 GetHeight() const { return m_unTextureHeigth; }
	un32 GetWidth() const { return m_unTextureWidth; }

	stTextureImpl::stTextureImpl( IDirect3DTexture9 * pTexture, KREF( stRect ) tRect, in_un32 unWidth, in_un32 unHeight ) 
	: TextureImpl( pTexture, unWidth, unHeight ) { }
	virtual ~stTextureImpl() { }
};
