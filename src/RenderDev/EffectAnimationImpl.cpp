#include <stlib\Maths.h>
#include "EffectAnimationImpl.h"

using namespace st_maths;
using namespace stMatrixHelper;

////////////////////////////////////////////////////////////////////////////////

void stAnimationInfoImpl::Restore() {
	m_tInfo = m_tInfoStore;
}

void stAnimationInfoImpl::Store() {
	m_tInfoStore = m_tInfo;
	m_unRepeatedCount = 0;
	m_unLoopCount = 0;
}

void stAnimationInfoImpl::restoreRepeat() {
	m_tInfo.fCurrentData = m_tInfoStore.fDuration;
	m_tInfo.fDelta = ( - m_tInfoStore.fDelta );
	m_tInfo.fDuration = m_tInfoStore.fCurrentData;
	m_tInfo.fPreviousData = m_tInfo.fCurrentData;
	Store();
}

void stAnimationInfoImpl::ResetAll() {
	SetDuration(); 
	SetData(); 
	SetDelta();
	m_unRepeatedCount = 0;
	m_unLoopCount = 0;
}

n32 stAnimationInfoImpl::Do() {
	if ( isOutOfDuration() ) {
		m_tInfo.fCurrentData = m_tInfo.fDuration;
		return ST_ANIMATIONFINISHED;
	} else {
		m_tInfo.fPreviousData = m_tInfo.fCurrentData;
		m_tInfo.fCurrentData += m_tInfo.fDelta;
		return ST_ANIMATIONNOTFINISHED;
	}
}

un32 stAnimationInfoImpl::DoLoop() {
	if ( Do() == ST_ANIMATIONFINISHED ) {
		Restore();
		++m_unLoopCount;
	}
	return m_unLoopCount;
}

un32 stAnimationInfoImpl::DoRepeat() {
	if ( Do() == ST_ANIMATIONFINISHED ) {
		restoreRepeat();
		++m_unRepeatedCount;
	}
	return m_unRepeatedCount;
}

////////////////////////////////////////////////////////////////////////////////

n32 stEffectAnimation3DImpl::DoTransform() {
	static D3DXMATRIX aRotation[3] = { ktIdentityMatrix, ktIdentityMatrix, ktIdentityMatrix };

	if ( m_IsPaused ) 
		return ST_ANIMATIONPAUSED;
	else 
		m_nFinishedAnimationCount = 0;
	/// --- axis value processions ---
	for ( un32 i = 0; i < ST_MAXEFFECTS; ++i ) {
		for ( un32 j = 0; j < ST_MAXAXIS; ++j ) { 
			if ( ST_ANIMATIONFINISHED == m_aaAnimationInfos[ i ][ j ].Do() )
				++m_nFinishedAnimationCount;
		}
	}
	/// --- matrix processions --- 
	// translation
	D3DXMatrixTranslation( 
		& m_aMatrixs[ ST_EFFECTTRANSLATION ], 
		m_aaAnimationInfos[ ST_EFFECTTRANSLATION ][ ST_X ].GetData(),
		m_aaAnimationInfos[ ST_EFFECTTRANSLATION ][ ST_Y ].GetData(),
		m_aaAnimationInfos[ ST_EFFECTTRANSLATION ][ ST_Z ].GetData() );
	// rotation
	D3DXMatrixRotationX( & aRotation[ ST_X ], m_aaAnimationInfos[ ST_EFFECTROTATION ][ ST_X ].GetData() );
	D3DXMatrixRotationY( & aRotation[ ST_Y ], m_aaAnimationInfos[ ST_EFFECTROTATION ][ ST_Y ].GetData() );
	D3DXMatrixRotationZ( & aRotation[ ST_Z ], m_aaAnimationInfos[ ST_EFFECTROTATION ][ ST_Z ].GetData() );
	m_aMatrixs[ ST_EFFECTROTATION ] = GetMatrixMultiply( aRotation, ST_MAXAXIS );
	// scaling
	D3DXMatrixScaling( 
		& m_aMatrixs[ ST_EFFECTSCALE ], 
		m_aaAnimationInfos[ ST_EFFECTSCALE ][ ST_X ].GetData(),
		m_aaAnimationInfos[ ST_EFFECTSCALE ][ ST_Y ].GetData(),
		m_aaAnimationInfos[ ST_EFFECTSCALE ][ ST_Z ].GetData() );
	return m_nFinishedAnimationCount;
}