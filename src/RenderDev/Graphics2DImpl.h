/// ----------------------------------------------------------------------------
/// \file Graphics2DImpl.h
/// \brief Graphics2D for processing 2D implements
/// ----------------------------------------------------------------------------
#pragma once

#include "..\RenderDev.h"
#include "..\RenderDev\RenderDevImpl.h"
#include "..\MathsExtend\Vertex.h"
#include "GraphicsImpl.h"

class stRect;
class stRenderDevImpl;
class stQuad;
/// ----------------------------------------------------------------------------
/// \brief Graphics2D implements
/// ----------------------------------------------------------------------------
class stGraphics2DImpl : public stGraphicsImpl< IGraphics2D > {
private:
	class coord;
	class stQuad;

protected:
	IDirect3DVertexBuffer9 * m_pVertexBuf;
	IDirect3DIndexBuffer9 * m_pIndexBuf;

	un16 * m_pIndices;
	stVertex::Vertex * m_pVertices;
	
	un32 m_unMaxIndicesCount;
	un32 m_unMaxVerticesCount;

protected:
	/// fix states
	/// --------------------------------------------------------------------------------		
	void fixProjTransform( void );
	void fixViewTransform( void );

	coord * m_pCoord; ///< \brief coord pointers

public:
	stResult DrawQuad( ITexture * pTexture, in_un32 nRenderQueue );
	stResult DrawNormal( in_un32 nVerticesCount, in_un32 nTriangleCount, in_un32 nRenderQueue );
	stResult PushIndexBuffer();
	stResult PushVertexBuffer();
	stResult ColRects( KREF( stRect ) tAppendRect, in_un32 nCurrentVertex, QuadColor * pQuadColor = NULL );

	stGraphics2DImpl( stRenderDevImpl * pDev, in_un32 nMaxVertexBuf, in_un32 nMaxIndexBuf );
	virtual ~stGraphics2DImpl();
};
/// ----------------------------------------------------------------------------
/// \brief texture quad implements
/// ----------------------------------------------------------------------------
class stGraphics2DImpl::stQuad : public Square {
protected:
	const static un16 m_unVertexCount = 4; ///< \brief each quad has 4 vertex

	stVertex2D m_tVertex2D[ m_unVertexCount ];

public:
	f32 GetWidth() const { return m_tVertex2D[2].GetX() - m_tVertex2D[0].GetX(); }
	f32 GetHeight() const { return m_tVertex2D[1].GetY() - m_tVertex2D[0].GetY(); }
	Dot::Point GetCat1() const { return Dot::Point( m_tVertex2D[0].GetX(), m_tVertex2D[0].GetY() ); }
	Dot::Point GetCat2() const { return Dot::Point( m_tVertex2D[3].GetX(), m_tVertex2D[3].GetY() ); } 
	Dot::Point GetCenter() const { return Dot::Point( m_tVertex2D[1].GetX() + GetWidth(), m_tVertex2D[1].GetY() + GetHeight() ); }
	void Scale( in_f32 fScale ) {
		f32 fScaledWid = GetWidth() * fScale;
		f32 fScaledHei = GetHeight() * fScale;

		m_tVertex2D[0].SetXY( m_tVertex2D[1].GetX(), m_tVertex2D[1].GetY() - fScaledHei );
		m_tVertex2D[1].SetXY( m_tVertex2D[1].GetX(), m_tVertex2D[1].GetY() );
		m_tVertex2D[2].SetXY( m_tVertex2D[1].GetX() + fScaledWid,  m_tVertex2D[1].GetY() );
		m_tVertex2D[3].SetXY( m_tVertex2D[1].GetX() + fScaledWid, m_tVertex2D[1].GetY() - fScaledHei );
	}
	void ScaleCenter( in_f32 fScale ) {
		f32 fScaledWid = GetWidth() * fScale;
		f32 fScaledHei = GetHeight() * fScale;

		m_tVertex2D[0].SetXY( GetCenter().fX - ( fScaledWid / 2 ), GetCenter().fY - ( fScaledHei / 2 ) );
		m_tVertex2D[1].SetXY( GetCenter().fX - ( fScaledWid / 2 ), GetCenter().fY + ( fScaledHei / 2 ) );
		m_tVertex2D[2].SetXY( GetCenter().fX + ( fScaledWid / 2 ), GetCenter().fY + ( fScaledHei / 2 ) );
		m_tVertex2D[3].SetXY( GetCenter().fX + ( fScaledWid / 2 ), GetCenter().fY - ( fScaledHei / 2 ) );
	}
	stVertex::Vertex & Data( const n32 nVertexIndex ) {
		if ( nVertexIndex >= m_unVertexCount || nVertexIndex < 0 )
			return m_tVertex2D[ 0 ].Data(); // return a left top vertex target
		return m_tVertex2D[ nVertexIndex ].Data();
	}
	stQuad( in_f32 fX, in_f32 fY, in_f32 fWidth, in_f32 fHeight ) {
		m_tVertex2D[0] = stVertex2D( fX         , fY - fHeight, 0.0f, 1.0f );
		m_tVertex2D[1] = stVertex2D( fX		    , fY          , 0.0f, 0.0f ); 
		m_tVertex2D[2] = stVertex2D( fX + fWidth, fY		  , 1.0f, 0.0f ); 
		m_tVertex2D[3] = stVertex2D( fX + fWidth, fY - fHeight, 1.0f, 1.0f );
	}
	stQuad() { }
	virtual ~stQuad() { }
};

/// ----------------------------------------------------------------------------
/// \brief coord procession implements
/// ----------------------------------------------------------------------------
class stGraphics2DImpl::coord {
private:
	f32 m_fScreenRatio;
	f32 m_fZDeep;
	stRect m_tRect3D;
	stRect m_tRectScreen;

private:
	void fixRect3D();

public:
	stGraphics2DImpl::stQuad To3D( KREF( stRect ) tRectScreen );
	stVertex2D To3DPoint( KREF( stPoint ) tVertex );
	stRect ToScreen( KREF( stQuad ) tQuad3D );

	void SetScreenRect( stRect rect ) { m_tRectScreen = rect; }
	void SetScreenRatio( in_f32 fScrRatio ) { m_fScreenRatio = fScrRatio; }
	void SetZDeep( in_f32 fZDeep ) { m_fZDeep = fZDeep; }
		
	f32 GetScreenRatio() const { return m_fScreenRatio; }
	f32 GetZDeep() const { return m_fZDeep; }

	coord( in_f32 fScrRatio, in_f32 fZDeep, KREF( stRect ) rectScreen ) 
		: m_fScreenRatio( fScrRatio ), m_fZDeep( fZDeep ), 
		m_tRect3D( -( m_fZDeep * m_fScreenRatio ), m_fZDeep, m_fZDeep * m_fScreenRatio, -m_fZDeep ),
		m_tRectScreen( rectScreen ) { }
	~coord() { }
}; 
