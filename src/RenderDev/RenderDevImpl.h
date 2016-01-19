/// ----------------------------------------------------------------------------
/// \file RenderDevImpl.h
/// \brief implement render device
/// ----------------------------------------------------------------------------
#pragma once

#include <stlib\Misc\Def.h> 
#include <stlib\String\StringImpl.h>
#include "..\RenderDev.h"
#include "..\RenderObj\TextureImpl.h"

class stGraphics2DImpl;

/// ----------------------------------------------------------------------------
/// \brief render device implements
/// ----------------------------------------------------------------------------
class stRenderDevImpl : public IRenderDevice {
protected:
	IDirect3D9 * m_pInterface;
	IDirect3DDevice9 * m_pDevice;
	D3DPRESENT_PARAMETERS m_tPresentParam;
	D3DFORMAT m_tBackBufferFormat;
	un32 m_unBackBufferWidth;
	un32 m_unBackBufferHeight;
	un32 m_unBackBufferCount;
	str m_strDeviceName;

protected:
	void fixStates( void );
	stResult resetDevice( void );

public:
	stResult Init( 
		HWND hWnd, 
		bool IsWindowed, 
		in_un32 unBufWidth, 
		in_un32 unBufHeight, 
		const n32 nMultiSampleQuality, 
		bool IsFpsLimited );
	f32 GetBufferRatio() const { return ( float ) m_unBackBufferWidth / ( float ) m_unBackBufferHeight; }
	void * GetHandle() { return ( IDirect3DDevice9 * )m_pDevice; }
	un32 GetBufWidth() const { return m_unBackBufferWidth; }
	un32 GetBufHeight() const { return m_unBackBufferHeight; }
	str GetDeviceName() const { return m_strDeviceName; }
	n32 IsWindowed() { return m_tPresentParam.Windowed; }
	
	void SetBufferSize( in_un32 unBufWidth, in_un32 unBufHeight );
	void SetWindowed( bool IsWindowed );
	void SetAALevel( const n32 nMultiSampleQuality );
	void SetTextureFilter( const n32 nFlag, const n32 nLevel );
	stResult ResetDevice( void );

	stResult CreateTextureFromFile( wchar_t *  strPathW, ITexture ** ppTexture );
	stResult CreateGraphics2D( stGraphics2DImpl ** ppG2D, in_un32 unMaxVertexBuf, in_un32 unMaxIndicesBuf );

	RECT EnumResolutions( in_un32 nIndex );
	n32 GetMaxResolutionsCount();

	stRenderDevImpl();
	virtual ~stRenderDevImpl();
};
