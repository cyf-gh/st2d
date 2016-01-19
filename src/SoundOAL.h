#pragma once 

#include <stlib\Misc\Def.h>
#include <al.h>
#include <alc.h>
#include <alut.h>
#pragma comment( lib, "openal32.lib" )
#pragma comment( lib, "alut.lib" )

#include "Unit.h"

enum ST_SOUNDSTATUS {
	ST_PLAYING = AL_PLAYING,
	ST_PAUSED = AL_PAUSED,
	ST_STOPPED = AL_STOPPED
};

enum ST_SOUNDALRESULT {
	ST_ERR_UNKNOWN = -3,
	ST_ERR_OK = 0
};

struct ISoundOAL : public IUintParent {
public:
	virtual void * GetDevice() = 0;
	virtual void SetListener( f32 aPosition[ 3 ], f32 aVelocity[ 3 ], f32 aOrientation[ 6 ] ) = 0;
};

struct ISoundInfo : public IUnit {
public:
	virtual n32	 LoadWAVFile( const char *strPath, bool IsLoop = false ) = 0;
	
	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual void Rewind( bool IsPlayNow = false ) = 0;
	virtual void SetLoop( bool IsLoop = false ) = 0;
	virtual void SetVolume( in_f32 fVolumeValue ) = 0;
	virtual void SetPosition( f32 aPosition[ 3 ], f32 aVelocity[ 3 ] ) = 0; 
};