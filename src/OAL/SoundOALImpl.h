/// ----------------------------------------------------------------------------
/// \file SoundOALImpl.h
/// \brief OpenAL interfaces implements
/// ----------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>

// --- OpenAL implicit load ---
#include "..\SoundOAL.h"

class stStringW;

/// ----------------------------------------------------------------------------
/// \brief OpenALC device implements
/// ----------------------------------------------------------------------------
class stOALCDevImpl {
protected:
	ALCdevice * m_pDevice;
	ALCcontext * m_pContext;
	stResult m_nError;
	const ALCchar * m_pDeviceSpecifier;

public:
	const ALCchar * GetDeviceName() const { return m_pDeviceSpecifier; }
	stOALCDevImpl() {
		m_pDevice = alcOpenDevice( "DirectSound3D" );
		m_pDeviceSpecifier = alcGetString( m_pDevice, ALC_DEVICE_SPECIFIER );
		m_pContext = alcCreateContext( m_pDevice, NULL );
		alcMakeContextCurrent( m_pContext );
		if ( m_nError = alcGetError( m_pDevice ) != ALC_NO_ERROR )
			throw;
	}
	virtual ~stOALCDevImpl() {
		alcMakeContextCurrent( NULL );
		alcDestroyContext( m_pContext );
		alcCloseDevice( m_pDevice );
	}
};

/// ----------------------------------------------------------------------------
/// \brief OpenAL sound source implements
/// \note should initialize after SoundOAL class
/// ----------------------------------------------------------------------------
class stSoundInfo : public ISoundInfo {
private:
	stStringW * m_strTagW;

protected:
	f32 m_aPosition[ 3 ];
	f32 m_aVelocity[ 3 ];

	un32 m_unBuffer;
	un32 m_unSource;
	stResult m_nError;
	ST_SOUNDSTATUS	m_nStatus;

	n32 getALError();

public:
	wchar_t *  GetTag() const { return m_strTagW->GetString(); }	
	void SetTag( const wchar_t *strTagW ) { m_strTagW->SetString( strTagW ); }

	n32	 LoadWAVFile( const char *strPath, bool IsLoop = false );
	
	void Play();
	void Pause();
	void Stop();
	void Rewind( bool IsPlayNow = false );
	void SetLoop( bool IsLoop = false );
	void SetVolume( in_f32 fVolumeValue );
	void SetPosition( f32 aPosition[ 3 ], f32 aVelocity[ 3 ] ); 

	stSoundInfo( const char *strPath, const wchar_t *strTagW, bool IsLoop = false ) 
		: m_strTagW( new stStringW( strTagW ) ) {
		LoadWAVFile( strPath, IsLoop );
	}
	virtual ~stSoundInfo() { 
		alDeleteBuffers( 1, & m_unBuffer );
		alDeleteSources( 1, & m_unSource );
		st_safe_del( m_strTagW );
	}
};

/// ----------------------------------------------------------------------------
/// \brief OpenAL implements
/// ----------------------------------------------------------------------------
class stOALImpl : public ISoundOAL {
protected:	
	f32 m_aPosition[ 3 ];
	f32 m_aVelocity[ 3 ];
	f32 m_aOrientation[ 6 ];

	std::vector< ISoundInfo * > m_vecSoundInfo;
	stOALCDevImpl * m_pDev;

	void fixListenerPosition();

public:
	void * FindWithTag( const wchar_t *strTagW );

	void * GetDevice() const { return m_pDev; }
	void SetListener( f32 aPosition[ 3 ], f32 aVelocity[ 3 ], f32 aOrientation[ 6 ] );
	
	stOALImpl();
	virtual ~stOALImpl() { }
};