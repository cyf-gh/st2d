/// ----------------------------------------------------------------------------
/// \file RectangleImpl.h
/// \brief rectangle objects implements
/// ----------------------------------------------------------------------------
#pragma once

#include "..\RenderObject.h"

/// ----------------------------------------------------------------------------
/// \brief rectangle base interface implements
/// ----------------------------------------------------------------------------
template< typename T >
class RectangleImpl : public T {
protected:
	QuadColor m_tQuadColor;
	void setPosition( stRect * pModifyRect, KREF( stPoint ) tPoint ) {
		const un32 unCurrentTextureWidth = ( un32 )pModifyRect->GetWidth();
		const un32 unCurrentTextureHeight = ( un32 )pModifyRect->GetHeight();

		* pModifyRect = stRect( tPoint.GetX(), unCurrentTextureWidth + tPoint.GetX(), tPoint.GetY(), unCurrentTextureHeight + tPoint.GetY() );	
	}
public:
	void SetQuadColor( KREF( QuadColor ) tQuadColor ) { m_tQuadColor = tQuadColor; }
	QuadColor GetQuadColor() { return m_tQuadColor; }

	RectangleImpl( QuadColor tQuadColor ) : m_tQuadColor( tQuadColor ) { }
	virtual ~RectangleImpl() {}
};

/// ----------------------------------------------------------------------------
/// \brief single rectangle object implement
/// ----------------------------------------------------------------------------
class stRectangleImpl : public RectangleImpl< IRectangle > {
protected:
	stRect m_tRect;

public:
	bool IsRepeated() const { return false; }

	un32 GetRectCount() const { return 1; }

	stRect GetRect( in_un32 unIndex = 0 ) { return m_tRect; }
	void SetRect( KREF( stRect )tRect, in_un32 unIndex = 0 ) { m_tRect = tRect; }	
	void SetPos( KREF( stPoint )tPoint, in_un32 unIndex = 0 );

	stRectangleImpl( KREF( stRect ) tRect, KREF( QuadColor ) tQuadColor ) 
		: RectangleImpl( tQuadColor ), m_tRect( tRect ) { }
	virtual ~stRectangleImpl() { }
};

/// ----------------------------------------------------------------------------
/// \brief repeated rectangle object implement
/// ----------------------------------------------------------------------------
class stRectangleRepeatedImpl : public RectangleImpl< IRectangle > {
private:
	static const un32 m_unMaxRects = 20; ///< \brief max repeated texture count

protected:
	stRect m_aBackBufRects[ m_unMaxRects ];
	bool m_aMirrorRects[ m_unMaxRects ];

public:
	bool IsRepeated() const { return true; }

	un32 GetRectCount() const;
	stRect GetRect( in_un32 unIndex ) { return m_aBackBufRects[ unIndex ]; }
	void SetRect( KREF( stRect )tRect, in_un32 unIndex );
	void SetPos( KREF( stPoint )tPoint, in_un32 unIndex = 0 );

	stRectangleRepeatedImpl( KREF( QuadColor ) tQuadColor ) 
		: RectangleImpl( tQuadColor ) {
		for ( un32 i = 0; i < m_unMaxRects; ++i )
			m_aMirrorRects[ i ] = false;
	}
	virtual ~stRectangleRepeatedImpl() { }
};
