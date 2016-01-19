/// ----------------------------------------------------------------------------
/// \file Vertex.h
/// \brief vertex implements
/// ----------------------------------------------------------------------------
#pragma once

#include <stlib\Misc\Def.h>
#include "..\Maths.h"
#include <d3d9.h>

/// ----------------------------------------------------------------------------
/// \brief vertex implements
/// ----------------------------------------------------------------------------
class stVertex {
public:
	static const ul32 FVF = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 ); 
	/// \brief base struct implements
	struct Vertex {
		f32 x, y, z;
		f32 nx, ny, nz;
		D3DCOLOR color;
		f32 u, v; 
	};
protected:
	Vertex m_vertex;

public:
	void SetXYZ( in_f32 fX, in_f32 fY, in_f32 fZ )  {
		m_vertex.x = fX;
		m_vertex.y = fY;
		m_vertex.z = fZ;
	}
	void SetColor( KREF( D3DCOLOR ) dwColor ) {
		m_vertex.color = dwColor;
	}
	void SetUV( in_f32 fU, in_f32 fV ) {
		m_vertex.u = fU;
		m_vertex.v = fV;
	}
	f32 GetX() const { return m_vertex.x; }
	f32 GetY() const { return m_vertex.y; }
	f32 GetZ() const { return m_vertex.z; }
	f32 GetU() const { return m_vertex.u; }
	f32 GetV() const { return m_vertex.v; }
	
	stVertex() {
		m_vertex = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x0, 0.0f, 0.0f };
	}
	virtual ~stVertex() { }
};

/// ----------------------------------------------------------------------------
/// \brief 2D vertex implements
/// ----------------------------------------------------------------------------
class stVertex2D : protected stVertex, public Dot {
protected:
	/// \brief fix the normal value
	void fixNormal() {
		m_vertex.nx = 0.0f;
		m_vertex.ny = 0.0f;
		m_vertex.nz = -1.0f;
	}

public:
	f32 GetX() const { return stVertex::GetX(); }
	f32 GetY() const { return stVertex::GetY(); }
	void SetXY( in_f32 fX, in_f32 fY ) {
		SetXYZ( fX, fY, 1.0f );
	}
	
	void SetARGB( const n32 nA, const n32 nR, const n32 nG, const n32 nB ) {
		SetColor( D3DCOLOR_ARGB( nA, nR, nG, nB ) );
	}
	void SetUV( in_f32 fU, in_f32 fV ) {
		stVertex::SetUV( fU, fV );
	}
	Vertex & Data() { return m_vertex; }

	stVertex2D() {
		fixNormal();
	}
	stVertex2D( in_f32 fX, in_f32 fY, in_f32 fU, in_f32 fV ) {
		fixNormal();
		SetXY( fX, fY );
		SetUV( fU, fV );
		SetARGB( 255, 255, 255, 255 );
	};
	stVertex2D( in_f32 fU, in_f32 fV, const n32 nA, const n32 nR, const n32 nG, const n32 nB, in_f32 fX, in_f32 fY, in_f32 fZ = 1.0f ) {
		fixNormal();
		SetXYZ( fX, fY, fZ );
		SetUV( fU, fV );
		SetARGB( nA, nR, nG, nB );
	}
	virtual ~stVertex2D() { };
};
/// ----------------------------------------------------------------------------
/// \brief extended vertex2d type
/// ----------------------------------------------------------------------------
class stVertex2DEx : public stVertex2D { };