/// ----------------------------------------------------------------------------
/// \file FPSController.h
/// \brief FPS controller implements
/// ----------------------------------------------------------------------------
#pragma once

#include <stlib\Misc\StopWatch.h>

/// ----------------------------------------------------------------------------
/// \brief FPS controller implements
/// ----------------------------------------------------------------------------
class stFPSController{
protected:
	stStopWatch * m_pstStopWatch; 

	ul32 m_ulFrameCnt; /// \brief frame summation
	f64 m_dFPS;		   /// \brief current fps

public:
	f64 GetCurrentFPS( stStopWatch * pstStopWatch ) {	
		++m_ulFrameCnt;
		if ( pstStopWatch->GetElapsed() >= 1 ) {
			m_dFPS = ( f64 )( m_ulFrameCnt ) / pstStopWatch->GetElapsed();
			m_ulFrameCnt = 0;
			pstStopWatch->Reset();
		}
		return m_dFPS;
	}

	stFPSController() : m_pstStopWatch( NULL ), m_ulFrameCnt( 0UL ), m_dFPS( 0 ) { }
	virtual ~stFPSController() { }
};

