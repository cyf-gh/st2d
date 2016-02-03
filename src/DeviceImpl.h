#ifndef __ST2D_DEVICEIMPL_H__
#define __ST2D_DEVICEIMPL_H__

#include <d3d9.h>
#include <d3dx9.h>

#include <core\Def.h>
#include <utils\Maths.h>

#include "..\include\Enum.h"
#include "..\include\Device.h"

class stStrA;
class stStrW;
class stTexture;

namespace st2dImpl {

class DeviceImpl : public st2d::stDevice {
public:
	class BackBufInfo {
	public:
		static un32				Count;
		D3DFORMAT				Format;
		un32					Width;
		un32					Height;
		f32						Ratio() const { return ( f32 )Width / ( f32 )Height; }
	};

protected:
	bool						m_initalized;
	IDirect3D9 *				m_pinterface;
	IDirect3DDevice9 *			m_pdevice;
	D3DPRESENT_PARAMETERS		m_presParm;

	stStrA *					m_deviceName;	
	BackBufInfo					m_info;

public:
	DeviceImpl &				Initalize( HWND handle, bool isWindowed, const un32 bufWidth, const un32 bufHeight, const st2dEnum::AA multiSampleLevel, const bool IsFpsLimited );
	DeviceImpl &				Reset( HWND handle, bool isWindowed, const un32 bufWidth, const un32 bufHeight, const st2dEnum::AA multiSampleLevel, const bool IsFpsLimited );
	DeviceImpl &				SetAALevel( const st2dEnum::AA newLevel );
	stLibUtils::Maths::stRect	EnumResolutions( const un32 index );
	un32						ResolutionsCount();
	const BackBufInfo &			GetBackBufInfo() const { return m_info; }
	const stStrA &				GetDeviceName() const { return *m_deviceName; }

	DeviceImpl &				CreateTextureFromFile( const stStrW &path, stTexture **ppTexture );

	friend void					Fix2DStates( DeviceImpl &dev );

								DeviceImpl(); 
	virtual						~DeviceImpl();
};

} /* st2dImpl */

#endif /* !__ST2D_DEVICEIMPL_H__ */