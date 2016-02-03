#include "..\include\Enum.h"
using namespace st2dEnum;

#include "RenderListImpl.h"

using namespace st2dImpl;

const st2d::stElementInfo &QuadImpl::Info() {
	static const st2d::stElementInfo info { 
		4, /* 4 vertices */
		6, /* 6 indices */
		2, /* 2 EX handle */
		D3DPT_TRIANGLELIST, /* triangle type */
		2  /* 2 triangles */ };

	return info;
}

const un16 *QuadImpl::DataI() {
	static const un16 ind[] = { 0, 1, 2, 0, 2, 3 };

	return ind;
}

QuadImpl::QuadImpl( const f32 x, const f32 y, const f32 width, const f32 height ) {
	for ( nbus i = 0; i < 4; ++i ) {
		normal_vertex( &m_quad[i] );
	}
	m_quad[0].x = x;
	m_quad[0].y = y - height;
	m_quad[0].u = 0.0f;
	m_quad[0].v = 1.0f;

	m_quad[1].x = x;
	m_quad[1].y = y;
	m_quad[1].u = 0.0f;
	m_quad[1].v = 0.0f;

	m_quad[2].x = x + width;
	m_quad[2].y = y;
	m_quad[2].u = 1.0f;
	m_quad[2].v = 0.0f;

	m_quad[3].x = x + width;
	m_quad[3].y = y - height;
	m_quad[3].u = 1.0f;
	m_quad[3].v = 0.0f;
}

QuadImpl &QuadImpl::Scale( const f32 scale ) {
	const f32 scaledWid = Width() * scale;
	const f32 scaledHei = Height() * scale;

	m_quad[0].x = m_quad[1].x;
	m_quad[0].y = m_quad[1].y - scaledHei;
	
	m_quad[1].x = m_quad[1].x;
	m_quad[1].y = m_quad[1].y;

	m_quad[2].x = m_quad[1].x + scaledWid;
	m_quad[2].y = m_quad[1].y;

	m_quad[3].x = m_quad[1].x + scaledHei;
	m_quad[3].y = m_quad[1].y - scaledHei;
	return *this;
}

QuadImpl &QuadImpl::ScaleCenter( const f32 scale ) {
	const f32 scaledWid = Width() * scale;
	const f32 scaledHei = Height() * scale;

	m_quad[0].x = Center().x - ( scaledWid / 2 );
	m_quad[0].y = Center().y - ( scaledHei / 2 );
	
	m_quad[1].x = Center().x - ( scaledWid / 2 );
	m_quad[1].y = Center().y + ( scaledHei / 2 );

	m_quad[2].x = Center().x + ( scaledWid / 2 );
	m_quad[2].y = Center().y + ( scaledHei / 2 );

	m_quad[3].x = Center().x + ( scaledWid / 2 );
	m_quad[3].y = Center().y - ( scaledHei / 2 );
	return *this;
}

QuadImpl &QuadImpl::SetColor( const un32 *pcolor ) {
	for ( nbus i = 0; i < 4; ++i ) {
		m_quad[i].color = pcolor[i];
	}
	return *this;
}

const un32 QuadImpl::DataEX( const un32 index, void ** pphandle ) {
	switch ( index ) {
	case 0 :
		*pphandle = m_ptexture;
		return ST_ELEMENT_GENRE_TEXTURE;
	case 1:
		*pphandle = m_peffect;
		return ST_ELEMENT_GENRE_MARTRIX;
	default:
		return ST_ELEMENT_GENRE_UNKNOWN;
	}
	return ST_ELEMENT_GENRE_UNKNOWN;
}

st2d::stRenderListElement &QuadImpl::SetEX( const un32 index, void *phandle ) {
	switch ( index ) {
	case 0 :
		m_ptexture = ( st2d::stTexture * )phandle;
		break;
	case 1:
		m_peffect = ( st2d::stAnimation * )phandle;
		break;
	default:
		break;
	}
	return *this;
}
