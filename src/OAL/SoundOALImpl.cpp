#include <stlib\Misc\Def.h>
#include <stlib\String\StringImpl.h>
#pragma comment( lib, "StomLIB.lib" )

#include "SoundOALImpl.h"

////////////////////////////////////////////////////////////////////////////////

n32 stSoundInfo::getALError() {
	return m_nError = alGetError();
}

void stSoundInfo::Pause() {
	alSourcePause( m_unSource );
	m_nStatus = ST_PAUSED;
}

void stSoundInfo::Stop() {
	alSourceStop( m_unSource );
	m_nStatus = ST_STOPPED;
}

void stSoundInfo::Rewind( bool IsPlayNow ) {
	alSourceRewind( m_unSource );
	if ( IsPlayNow )
		Play();
	else 
		m_nStatus = ST_STOPPED;
}

void stSoundInfo::Play() {
	alSourcePlay( m_unSource );
	m_nStatus = ST_PLAYING;
}

void stSoundInfo::SetVolume( in_f32 fVolumeValue ) {
	alSourcef( m_unSource , AL_GAIN, fVolumeValue );
}

void stSoundInfo::SetLoop( bool IsLoop ) {
	alSourcei( m_unSource, AL_LOOPING, IsLoop );
}

void stSoundInfo::SetPosition( f32 aPosition[ 3 ], f32 aVelocity[ 3 ] ) {
	memcpy( m_aPosition, aPosition, sizeof( aPosition ) );
	memcpy( m_aVelocity, aVelocity, sizeof( aVelocity ) );
	alSourcefv( m_unSource, AL_POSITION, aPosition );
	alSourcefv( m_unSource, AL_VELOCITY, aVelocity );
}

n32 stSoundInfo::LoadWAVFile( const char *strPath, bool IsLoop ) {
	m_unBuffer = alutCreateBufferFromFile( strPath );
	if ( NULL == m_unBuffer )
		return getALError();
	alGenSources( 1, & m_unSource );
	alSourcei( m_unSource, AL_BUFFER, m_unSource );
	SetLoop( IsLoop );
	if ( getALError() != ALUT_ERROR_NO_ERROR )
		return m_nError;
	return ST_ERR_OK;
}

////////////////////////////////////////////////////////////////////////////////

stOALImpl::stOALImpl() 
	: m_pDev( new stOALCDevImpl ) {
	n32 argc = 1;
	str argv = "OPENAL_X86";
	if ( ! alutInit( & argc, & argv ) ) 
		return;
}

void stOALImpl::fixListenerPosition() {
	for ( un32 i = 0; i < 3; ++i ) {
		m_aPosition[ i ] = 0.f;
		m_aVelocity[ i ] = 0.f;
	}
	m_aOrientation[ 0 ] = 0.f;
	m_aOrientation[ 1 ] = 0.f;
	m_aOrientation[ 2 ] = -1.f;
	m_aOrientation[ 3 ] = 0.f;
	m_aOrientation[ 4 ] = 1.f;
	m_aOrientation[ 5 ] = 0.f;
	SetListener( m_aPosition, m_aVelocity, m_aOrientation );
}

void stOALImpl::SetListener( f32 aPosition[ 3 ], f32 aVelocity[ 3 ], f32 aOrientation[ 6 ] ) {
	memcpy( m_aPosition, aPosition, sizeof( aPosition ) );
	memcpy( m_aVelocity, aVelocity, sizeof( aVelocity ) );
	memcpy( m_aOrientation, aOrientation, sizeof( aOrientation ) );
	alListenerfv( AL_POSITION, m_aPosition );
	alListenerfv( AL_VELOCITY, m_aVelocity );
	alListenerfv( AL_ORIENTATION, m_aOrientation );
}

void * stOALImpl::FindWithTag( const wchar_t *strTagW ) {
	for ( std::vector< ISoundInfo * >::iterator it = m_vecSoundInfo.begin(); 
		  it != m_vecSoundInfo.end(); ++it ) {
		if ( st_wcs_base::wcsIsEqual( ( * it )->GetTag(), strTagW ) )
			return * it;	
	}
	return NULL;
}

