#include "Graphics2DImpl.h"
#include "..\MathsExtend\Rect.h"
#include "RenderDevImpl.h"

////////////////////////////////////////////////////////////////////////////////

stRenderDevImpl::stRenderDevImpl() 
	: m_pDevice( NULL ), m_pInterface( NULL ), m_strDeviceName( NULL ) {
	ZeroMemory( &m_tPresentParam, sizeof( m_tPresentParam ) );
}

stRenderDevImpl::~stRenderDevImpl() {
	st_safe_del( m_strDeviceName );
	st_safe_rel( m_pInterface );
	st_safe_rel( m_pDevice );
}

stResult stRenderDevImpl::Init( HWND hWnd, bool IsWindowed, in_un32 unBufWidth, in_un32 unBufHeight, const n32 nMultiSampleQuality, bool IsFpsLimited ) {
	D3DCAPS9 Caps9;
	D3DDISPLAYMODE DisplayMode = { 0 }; 
	n32 nVp = 0;

	if ( NULL == ( m_pInterface = Direct3DCreate9(D3D_SDK_VERSION) ) )
		return ST_ERR_INTERFACE;
	if ( FAILED( m_pInterface->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, & Caps9 ) ) ) 
		return ST_ERR_DEVICECAPS;
	if ( FAILED( m_pInterface->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, & DisplayMode ) ) )
		return ST_ERR_ADAPTERDISPLAYMODE;
	nVp = ( Caps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    m_tBackBufferFormat = m_tPresentParam.BackBufferFormat = DisplayMode.Format;
    m_tPresentParam.hDeviceWindow = hWnd;
    m_tPresentParam.Windowed = IsWindowed;
	m_tPresentParam.PresentationInterval = IsFpsLimited ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_unBackBufferWidth = m_tPresentParam.BackBufferWidth = unBufWidth;
    m_unBackBufferHeight = m_tPresentParam.BackBufferHeight = unBufHeight;
    m_unBackBufferCount = m_tPresentParam.BackBufferCount = 1;
	m_tPresentParam.Flags = 0;
	m_tPresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    m_tPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_tPresentParam.EnableAutoDepthStencil = true;
    m_tPresentParam.AutoDepthStencilFormat = D3DFMT_D16;
	m_tPresentParam.MultiSampleQuality = 0;
	SetAALevel( nMultiSampleQuality );
	if ( FAILED( m_pInterface->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, nVp, &m_tPresentParam, &m_pDevice ) ) ) 
		return ST_ERR_DEVICE;

	/// --- get device name ---
	D3DADAPTER_IDENTIFIER9 tIdentify;
	m_pInterface->GetAdapterIdentifier( 0, 0, & tIdentify );
	m_strDeviceName = new ch8[ strlen( tIdentify.Description ) + 1 ];
	strcpy( m_strDeviceName, tIdentify.Description );

	fixStates();
	return ST_ERR_OK;
}

stResult stRenderDevImpl::CreateTextureFromFile( wchar_t *  strPathW, ITexture ** ppTexture ) {
	IDirect3DTexture9 * pTexture = NULL;
	D3DXIMAGE_INFO ImageInfo = { 0 };

	if ( NULL != * ppTexture )
		return ST_ERR_OK_TEXTUREEXSIT;
	D3DXCreateTextureFromFileEx(
		m_pDevice,
		strPathW,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_FROM_FILE,
		D3DPOOL_DEFAULT,
		D3DFMT_A8B8G8R8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_COLORVALUE( 0.0f, 0.0f, 0.0f, 1.0f ),
		& ImageInfo,
		NULL,
		& pTexture
	);
	if ( NULL == pTexture )
		return ST_ERR_TEXTURECREATEFROMFILE;
	* ppTexture = new stTextureImpl( pTexture, stRect( 0, ImageInfo.Width, 0, ImageInfo.Height ), ImageInfo.Width, ImageInfo.Height );
	return ST_ERR_OK;
}

RECT stRenderDevImpl::EnumResolutions( in_un32 unIndex ) {
	D3DDISPLAYMODE tMode;

	if ( SUCCEEDED( m_pInterface->EnumAdapterModes( 0, D3DFMT_X8R8G8B8, unIndex, & tMode ) ) )
		return stRect( 0.0f, 0.0f, ( f32 )tMode.Width, ( f32 )tMode.Height ).ToRECT();
	else 
		return { 0 };
}

n32 stRenderDevImpl::GetMaxResolutionsCount() {
	return m_pInterface->GetAdapterModeCount( 0, D3DFMT_X8R8G8B8 );
}

stResult stRenderDevImpl::ResetDevice( void ) {
	return resetDevice();
}

void stRenderDevImpl::SetTextureFilter( const n32 nFlag, const n32 nLevel ) {
	n32 nType = 0;

	switch ( nFlag ) {
	case ST_NEARESTPOINTSAMLING :
		nType = D3DTEXF_POINT;
		break;
	case ST_LINEARFILTERING :
		nType = D3DTEXF_LINEAR;
		break;
	case ST_ANISOTROPICFILTERING :
		nType = D3DTEXF_ANISOTROPIC;
		break;
	}
	m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, nType );
	m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, nType );
	m_pDevice->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, nLevel );
}

void stRenderDevImpl::SetWindowed( bool IsWindowed ) {
	m_tPresentParam.Windowed = IsWindowed;
}

void stRenderDevImpl::SetAALevel( const n32 nMultiSampleQuality ) {
	if ( ST_AA_NONE != nMultiSampleQuality ) {
		switch ( nMultiSampleQuality ) {
		case ST_AA_2X :
			m_tPresentParam.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
			break;
		case ST_AA_4X :
			m_tPresentParam.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
			break;
		case ST_AA_8X :
			m_tPresentParam.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
			break;
		case ST_AA_16X :
			m_tPresentParam.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
			break;
		}
	}
}
void stRenderDevImpl::SetBufferSize( in_un32 unBufWidth, in_un32 unBufHeight ) {
	m_tPresentParam.BackBufferWidth = unBufWidth;
	m_tPresentParam.BackBufferHeight = unBufHeight;
}

void stRenderDevImpl::fixStates() {
    m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE ); 
	
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER );
	m_pDevice->SetSamplerState( 0, D3DSAMP_BORDERCOLOR, 0x00000000 );
	
	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true ); 
    m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ); 
    m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ); 
	m_pDevice->SetRenderState( D3DRS_LIGHTING, false );
	m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
    m_pDevice->SetRenderState( D3DRS_ZENABLE, true );
    m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
}

stResult stRenderDevImpl::resetDevice( void ) {
	if ( FAILED( m_pDevice->Reset( & m_tPresentParam ) ) )
		return ST_ERR_RESET;
	// --- when Reset returns, params below are set to zero ---
	m_tPresentParam.BackBufferWidth = m_unBackBufferWidth;
    m_tPresentParam.BackBufferHeight = m_unBackBufferHeight;
    m_tPresentParam.BackBufferCount = m_unBackBufferCount;
	return ST_ERR_OK;
}

stResult stRenderDevImpl::CreateGraphics2D( stGraphics2DImpl ** ppG2D,  in_un32 unMaxVertexBuf, in_un32 unMaxIndicesBuf ) {
	* ppG2D = new stGraphics2DImpl( this, unMaxVertexBuf, unMaxIndicesBuf );
	return ST_ERR_OK;
}
