#include "RectangleImpl.h"


void stRectangleImpl::SetPos( KREF( stPoint )tPoint, in_un32 unIndex ) { 
	setPosition( & m_tRect, tPoint ); 
}

////////////////////////////////////////////////////////////////////////////////

void stRectangleRepeatedImpl::SetPos( KREF( stPoint )tPoint, in_un32 unIndex ) {
	setPosition( & m_aBackBufRects[ unIndex ], tPoint );
}
void stRectangleRepeatedImpl::SetRect( KREF( stRect )tRect, in_un32 unIndex ) { 
	m_aBackBufRects[ unIndex ] = tRect; 
	m_aMirrorRects[ unIndex ] = true; 
}

un32 stRectangleRepeatedImpl::GetRectCount() const {
	un32 unSumAvailableRectCount = 0;
		for ( un32 i = 0; i < m_unMaxRects; ++i ) {
			if ( m_aMirrorRects[ i ] ) 
				++unSumAvailableRectCount;
		}
	return unSumAvailableRectCount;
}