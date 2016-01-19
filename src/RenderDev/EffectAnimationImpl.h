/// ----------------------------------------------------------------------------
/// \file EffectAnimationImpl.h
/// \brief animation effect implements
/// ----------------------------------------------------------------------------
#pragma once

#include "..\EffectAnimation.h"
#include "..\MathsExtend\Matrix.h"
#include "..\MathsExtend\Rect.h"

/// ----------------------------------------------------------------------------
/// \brief animation effect infos implements
/// ----------------------------------------------------------------------------
class stAnimationInfoImpl : public IAnimationInfo {
protected:
	un32 m_unLoopCount;
	un32 m_unRepeatedCount;

	struct Info {
		f32 fDuration;
		f32 fDelta;
		f32 fCurrentData;
		f32 fPreviousData;
		Info() {
			fDuration = 0.f; fDelta = 0.f; fCurrentData = 0.f; fPreviousData = 0.f;
		}
	} m_tInfo, m_tInfoStore;

	bool isOutOfDuration() { return m_tInfo.fDuration >= m_tInfo.fPreviousData && m_tInfo.fDuration <= m_tInfo.fCurrentData; }
	void restoreRepeat();

public:
	void ResetAll();

	void SetDuration( in_f32 fDuration = 0.f ) { m_tInfo.fDuration = fDuration; }
	void SetDelta( in_f32 fDelta = 0.f ) { m_tInfo.fDelta = fDelta; } 
	void SetData( in_f32 fData = 0.f ) { m_tInfo.fCurrentData = fData; }
	void Store();
	void Restore();

	f32 GetDuration() const { return m_tInfo.fDuration; }
	f32 GetDelta() const { return m_tInfo.fDelta; }
	f32 GetData() const { return m_tInfo.fCurrentData; }
	
	bool IsFinishedAnimation() const { return m_tInfo.fCurrentData == m_tInfo.fDuration; }

	n32 Do();
	un32 DoLoop();
	un32 DoRepeat();

	stAnimationInfoImpl() : m_unLoopCount( 0 ), m_unRepeatedCount( 0 ) { }
	virtual ~stAnimationInfoImpl() { 
	}
};

/// ----------------------------------------------------------------------------
/// \brief animation effect implements
/// ----------------------------------------------------------------------------
class stEffectAnimation3DImpl : public IEffectAnimation {
protected:
	stAnimationInfoImpl m_aaAnimationInfos[ ST_MAXEFFECTS ][ ST_MAXAXIS ];
	D3DXMATRIX m_aMatrixs[ ST_MAXEFFECTS ];

	n32 m_nFinishedAnimationCount;
	bool m_IsPaused;

public:
	n32 DoTransform();

	void Pause() { m_IsPaused = true; }
	void Resume() { m_IsPaused = false; }

	n32 GetFinishedAnimationCount() const { return m_nFinishedAnimationCount; }
	IAnimationInfo * GetAnimationInfo( ST_AXIS nXYZ, ST_EFFECTS nEffects ) { return & m_aaAnimationInfos[ nEffects ][ nXYZ ]; }
	D3DXMATRIX GetMatrix() { return stMatrixHelper::GetMatrixMultiply( m_aMatrixs, ST_MAXEFFECTS ); }

	stEffectAnimation3DImpl() 
		: m_IsPaused( false ), m_nFinishedAnimationCount( 0 ) { 
		for ( un32 i = 0; i < ST_MAXEFFECTS; ++i )
			m_aMatrixs[ i ] = stMatrixHelper::ktIdentityMatrix;
		// set for do not scale
		for ( un32 i = 0; i < ST_MAXAXIS; ++i ) {
			m_aaAnimationInfos[ ST_EFFECTSCALE ][ i ].SetData( 1.f );
			m_aaAnimationInfos[ ST_EFFECTSCALE ][ i ].SetDuration( 1.f );
		}
	}
	virtual ~stEffectAnimation3DImpl() { }
};

