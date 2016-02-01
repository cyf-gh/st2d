#include <core\Core.h>
using namespace stLibCore;

#include <base\Str.h>
#include <utils\Maths.h>
using namespace stLibUtils::Maths;

#include "..\include\DeviceImpl.h"
#include "..\include\ErrCode.h"

using namespace st2dImpl;
using namespace st2dEnum;
using namespace st2dErrCode;

un32 DeviceImpl::BackBufInfo::Count = 0;


DeviceImpl::DeviceImpl() : m_pinterface( NULL ), m_pdevice( NULL ), m_deviceName( NULL ), m_initalized( false ) { 
	st_zero_memory( &m_presParm, sizeof( D3DPRESENT_PARAMETERS ) );
}

DeviceImpl::~DeviceImpl() { 
	st_delete_class<stStrA>( &m_deviceName );
}

DeviceImpl &DeviceImpl::Initalize( HWND handle, bool isWindowed, const un32 bufWidth, const un32 bufHeight, 
	const st2dEnum::AA multiSampleLevel, const bool isFpsLimited ) {
	D3DADAPTER_IDENTIFIER9 identify;
	D3DDISPLAYMODE		   displayMode; 
	D3DCAPS9			   caps9;
	n32					   vp = 0;

	if ( m_initalized ) {
		st_core_return_with_var( ST_ERR_REINITALIZE, *this );		
	}

	st_zero_memory( &displayMode, sizeof( D3DDISPLAYMODE ) );
	st_zero_memory( &caps9, sizeof( D3DCAPS9 ) );
	st_zero_memory( &identify, sizeof( D3DADAPTER_IDENTIFIER9 ) );

	if ( NULL == ( m_pinterface = Direct3DCreate9( D3D_SDK_VERSION ) ) ) {
		st_core_return_with_var( ST_ERR_CREATE_D3D9INTERFACE, *this );
	}
	if ( FAILED( m_pinterface->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps9 ) ) ) {
		st_core_return_with_var( ST_ERR_GETDEVICECAPS, *this );
	} else {
		vp = ( caps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) ? 
			D3DCREATE_HARDWARE_VERTEXPROCESSING : 
			D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	if ( FAILED( m_pinterface->GetAdapterIdentifier( 0, 0, &identify ) ) ) {
		st_core_return_with_var( ST_ERR_GETADAPTERINDENTIFIER, *this );
	} else {	
		m_deviceName = st_new_class<stStrA>( identify.Description );
	}
	if ( FAILED( m_pinterface->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &displayMode ) ) ) {
		st_core_return_with_var( ST_ERR_GETDISPLAYMODE, *this );
	}

	m_info.Format							= m_presParm.BackBufferFormat = displayMode.Format;
    m_presParm.hDeviceWindow				= handle;
    m_presParm.Windowed						= isWindowed;
	m_presParm.PresentationInterval			= isFpsLimited ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_info.Width							= m_presParm.BackBufferWidth  = bufWidth;
    m_info.Height							= m_presParm.BackBufferHeight = bufHeight;
    BackBufInfo::Count						= m_presParm.BackBufferCount  = 1;
	m_presParm.Flags						= 0;
	m_presParm.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
    m_presParm.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_presParm.EnableAutoDepthStencil		= true;
    m_presParm.AutoDepthStencilFormat		= D3DFMT_D16;
	m_presParm.MultiSampleQuality			= 0;
	
	SetAALevel( multiSampleLevel );

	m_initalized = true;
	st_core_return_with_var( ST_NOERR, *this );	
}

DeviceImpl & st2dImpl::DeviceImpl::Reset( HWND handle, bool isWindowed, const un32 bufWidth, const un32 bufHeight, const st2dEnum::AA multiSampleLevel, const bool isFpsLimited ) {
	m_info.Format							= m_presParm.BackBufferFormat;
    m_presParm.hDeviceWindow				= handle;
    m_presParm.Windowed						= isWindowed;
	m_presParm.PresentationInterval			= isFpsLimited ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_info.Width							= m_presParm.BackBufferWidth  = bufWidth;
    m_info.Height							= m_presParm.BackBufferHeight = bufHeight;
    BackBufInfo::Count						= m_presParm.BackBufferCount  = 1;
	m_presParm.Flags						= 0;
	m_presParm.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
    m_presParm.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_presParm.EnableAutoDepthStencil		= true;
    m_presParm.AutoDepthStencilFormat		= D3DFMT_D16;
	m_presParm.MultiSampleQuality			= 0;
	
	SetAALevel( multiSampleLevel );

	if ( FAILED( m_pdevice->Reset( &m_presParm ) ) ) {
		st_core_return_with_var( ST_ERR_RESET, *this );	
	}
	st_core_return_with_var( ST_NOERR, *this );	
}

DeviceImpl &DeviceImpl::SetAALevel(const st2dEnum::AA newLevel) {
	if ( ST_AA_NONE != newLevel ) {
		switch ( newLevel ) {
		case ST_AA_2X :
			m_presParm.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
			break;
		case ST_AA_4X :
			m_presParm.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
			break;
		case ST_AA_8X :
			m_presParm.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
			break;
		case ST_AA_16X :
			m_presParm.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
			break;
		}
	}
	return *this;
}

void st2dImpl::Fix2DStates( DeviceImpl &dev ) {
    dev.m_pdevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE ); 
	
	dev.m_pdevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER );
	dev.m_pdevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER );
	dev.m_pdevice->SetSamplerState( 0, D3DSAMP_BORDERCOLOR, 0x00000000 );
	
	dev.m_pdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true ); 
    dev.m_pdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ); 
    dev.m_pdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ); 
	dev.m_pdevice->SetRenderState( D3DRS_LIGHTING, false );
	dev.m_pdevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
    dev.m_pdevice->SetRenderState( D3DRS_ZENABLE, true );
    dev.m_pdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    dev.m_pdevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
}

stRect DeviceImpl::EnumResolutions( const un32 index ) {
	D3DDISPLAYMODE mode;

	st_zero_memory( &mode, sizeof( D3DDISPLAYMODE ) );

	if ( FAILED( m_pinterface->EnumAdapterModes( 0, D3DFMT_X8R8G8B8, index, &mode ) ) ) {
		st_core_return_with_var( ST_ERR_ENUMADAPTERMODES, stRect() );
	} else {
		return stRect( 0.0f, 0.0f, static_cast<f64>( mode.Width ), static_cast<f64>( mode.Height ) );
	}
}

un32 DeviceImpl::ResolutionsCount() {
	return m_pinterface->GetAdapterModeCount( 0, D3DFMT_X8R8G8B8 );	
}

DeviceImpl &DeviceImpl::CreateTextureFromFile( const stStrW &path, stTexture **ppTexture ) {

}

