/// ----------------------------------------------------------------------------
/// \file EffectAnimation.h
/// \brief effect animation interfaces defines
/// ----------------------------------------------------------------------------
#pragma once

#include <d3dx9.h>

#include <stlib\Misc\Def.h>
#pragma comment( lib, "StomLIB.lib" )

/// --- symbols define ---

/// ----------------------------------------------------------------------------
/// \brief animation status
/// ----------------------------------------------------------------------------
enum ST_ANIMATIONEFFECTS {
	ST_ANIMATIONPAUSED = -1,
	ST_ANIMATIONING = 1
};

/// ----------------------------------------------------------------------------
/// \brief XYZ axis
/// ----------------------------------------------------------------------------
enum ST_AXIS {
	ST_X = 0,
	ST_Y = 1,
	ST_Z = 2,
	ST_MAXAXIS = 3,
};

/// ----------------------------------------------------------------------------
/// \brief effect genres
/// ----------------------------------------------------------------------------
enum ST_EFFECTS {
	ST_EFFECTTRANSLATION = 0,
	ST_EFFECTROTATION = 1,
	ST_EFFECTSCALE = 2,
	ST_MAXEFFECTS = 3
};

/// ----------------------------------------------------------------------------
/// \brief animation info status
/// ----------------------------------------------------------------------------
enum ST_ANIMATIONINFO {
	ST_ANIMATIONFINISHED = 0,
	ST_ANIMATIONNOTFINISHED = -1,
};

/// --- interfaces define ---

/// ----------------------------------------------------------------------------
/// \brief effect animation info interface
/// ----------------------------------------------------------------------------
struct IAnimationInfo {
public:
	virtual void ResetAll() = 0;

	virtual void SetDuration( in_f32 fDuration = 0.f ) = 0;
	virtual void SetDelta( in_f32 fDelta = 0.f ) = 0;
	virtual void SetData( in_f32 fData = 0.f ) = 0;
	virtual void Store() = 0;
	virtual void Restore() = 0;

	virtual f32 GetDuration() const = 0;
	virtual f32 GetDelta() const = 0;
	virtual f32 GetData() const = 0;
	
	virtual bool IsFinishedAnimation() const = 0;

	virtual n32 Do() = 0;
	virtual un32 DoLoop() = 0;
	virtual un32 DoRepeat() = 0;
};

/// ----------------------------------------------------------------------------
/// \brief effect animation interface
/// ----------------------------------------------------------------------------
struct IEffectAnimation {
public:
	/// \brief do animation transform
	/// \return animation finished count
	virtual n32 DoTransform() = 0;

	/// \brief get transformed matrix
	virtual D3DXMATRIX GetMatrix() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual n32 GetFinishedAnimationCount() const = 0;
	virtual IAnimationInfo * GetAnimationInfo( ST_AXIS nXYZ, ST_EFFECTS nEffects ) = 0;
};