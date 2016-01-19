/// ----------------------------------------------------------------------------
/// \file RenderObjects.h
/// \brief Stom2D main render objects interface declare
/// ----------------------------------------------------------------------------
#pragma once

#include <d3d9.h>

#include <stlib\Misc\Def.h>
#include "MathsExtend\Rect.h"

/// \addtogroup Stom2D RenderObj
/// \brief Stom2D render object stuffs interfaces
/// \{

/// ----------------------------------------------------------------------------
/// \brief quad color
/// ----------------------------------------------------------------------------
class QuadColor {
protected:
	D3DCOLOR m_aColor[4];

public:
	/// \brief get the assigned color
	/// \note the number out of 0 - 4 will be seem as a loop 
	/// \param[in] color index
	D3DCOLOR GetColor( in_un32 nColorIndex ) const { return m_aColor[ nColorIndex % 4 ]; } 

	/// \brief return a white default quad color
	QuadColor() { 
		D3DCOLOR tDefault = D3DCOLOR_ARGB( 255, 255, 255, 255 );
		m_aColor[ 0 ] = tDefault;
		m_aColor[ 1 ] = tDefault;
		m_aColor[ 2 ] = tDefault;
		m_aColor[ 3 ] = tDefault;
	}
	/// \brief initialize with 4 points colors
	QuadColor( 		
		KREF( D3DCOLOR ) tLeftBtm, 
		KREF( D3DCOLOR ) tLeftTop, 
		KREF( D3DCOLOR ) tRightTop, 
		KREF( D3DCOLOR ) tRightBtm 
	) { 
		m_aColor[ 0 ] = tLeftBtm;
		m_aColor[ 1 ] = tLeftTop;
		m_aColor[ 2 ] = tRightTop;
		m_aColor[ 3 ] = tRightBtm;
	}; 
	virtual ~QuadColor() { };
};
/// ----------------------------------------------------------------------------
/// \brief Rectangle form with 4 points
/// ----------------------------------------------------------------------------
struct IRectangle {
public:
	/// \brief check if the current Rectangle is repeated texture 
	virtual bool IsRepeated() const = 0;

	/// \brief get rect of Rectangle on the back buffer coord
	/// \param[in] get the indexed rect
	virtual stRect GetRect( in_un32 unIndex ) = 0;

	/// \brief set Rectangle rect
	/// \param[in] rect of Rectangle, left point is position on the back buffer, other, the size of the Rectangle presents
	/// \param[in] the index of current input rect
	virtual void SetRect( KREF( stRect )tRect, in_un32 unIndex ) = 0;

	/// \brief set the left right point of Rectangle in screen and height width is in current size
	/// \param[in] left right point of rect
	/// \param[in] the index of current input rect
	virtual void SetPos( KREF( stPoint )tPoint, in_un32 unIndex = 0 ) = 0;

	/// \brief set the rectangle quad color
	/// \param[in] quad color
	virtual void SetQuadColor( KREF( QuadColor ) tQuadColor ) = 0;
	
	/// \brief get the rect count
	virtual un32 GetRectCount() const = 0;

	/// \brief get current quad color
	virtual QuadColor GetQuadColor() = 0;
};
/// ----------------------------------------------------------------------------
/// \brief texture interface
/// ----------------------------------------------------------------------------
struct ITexture {
public:
	/// \brief get D3D texture handle
	virtual void * GetHandle() = 0;

	/// \brief get D3D texture original height
	virtual un32 GetHeight() const = 0;

	/// \brief get D3D texture original width
	virtual un32 GetWidth() const = 0;
};

/// ----------------------------------------------------------------------------
/// \brief texture results
/// ----------------------------------------------------------------------------
enum ST_TEXTURE { 
	ST_ERR_INVALIDRECT = -1, ///< \brief invalid rect detected
};

/// \}