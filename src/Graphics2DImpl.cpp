#include "Graphics2DImpl.h"
using namespace stLibCore;
using namespace stLibUtils::Maths;
using namespace st2dImpl;
using namespace st2d::stVertex;


class Quad {
private:
	Vertex		m_quad[4];

public:
	stRect		Rect() const			{ return stLibUtils::Maths::stRect( m_quad[0].x, m_quad[0].y, m_quad[3].x, m_quad[3].x ); }
	stVec2		Center() const			{ return stLibUtils::Maths::stVec2( m_quad[1].x + Width(), m_quad[1].y + Height() ); }
	const Vertex &Data( const un8 index ) const	{ return m_quad[index]; }
	f32			Width() const			{ return m_quad[2].x - m_quad[0].x; }
	f32			Height() const			{ return m_quad[1].y - m_quad[0].y; }
	Quad &		Scale( const f32 scale );
	Quad &		ScaleCenter( const f32 scale );

				Quad() { }
				Quad( const f32 x, const f32 y, const f32 width, const f32 height ); 
				~Quad() { }
};

Quad::Quad( const f32 x, const f32 y, const f32 width, const f32 height ) {
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

Quad &Quad::Scale( const f32 scale ) {
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

Quad &Quad::ScaleCenter( const f32 scale ) {
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

class Coord {
private:
	const f32	 m_screenRatio;
	const f32	 m_z;
	const stRect m_screen;
	const stRect m_3d;

public:
	Quad	To3D( const stRect &screen );
	Vertex	To3DPoint( const stVec2 &vec );
	stRect	ToScreen( const Quad &q3d );
			Coord( const f32 ratio, const f32 z, const stRect &screen );
};

Coord::Coord( const f32 ratio, const f32 z, const stRect &screen )
	  : m_screenRatio( ratio ), m_z( z ), m_screen( screen ), m_3d( -( m_z * m_screenRatio ), m_z * m_screenRatio, m_z, -m_z ){ }

Coord::~Coord() { }

Quad Coord::To3D( const stRect &screen ) {
	const f32 xr = m_3d.Width() / m_screen.Width();
	const f32 yr = ( m_3d.a.y - m_3d.b.y ) / m_screen.Height();

	return Quad( 
		( screen.a.x - m_screen.Center().x ) * xr,
	   -( screen.a.y - m_screen.Center().y ) * yr,
		  screen.Width() * xr,
		  screen.Height() * yr );
}

Vertex Coord::To3DPoint( const stVec2 &vec ) {
	const f32 xr = m_3d.Width() / m_screen.Width();
	const f32 yr = ( m_3d.a.y - m_3d.b.y ) / m_screen.Height();
	Vertex	  v;

	normal_vertex( &v );
	v.x = ( vec.x - m_screen.Center().x ) * xr;
	v.y = -( vec.y - m_screen.Center().y ) * yr;

	return v;
}

stRect Coord::ToScreen( const Quad &q3d ) {
	const f32 xr = m_screen.Width() / m_3d.Width();
	const f32 yr = m_screen.Height() / ( m_3d.a.y - m_3d.b.y );

	return stRect( 
		( q3d.Data( 0 ).x + q3d.Width() )  * xr ,
		( q3d.Data( 0 ).y + q3d.Height() ) * yr ,
		  q3d.Width() * xr,
		  q3d.Width() * yr );
}

Graphics2DImpl::Graphics2DImpl( st2d::stDevice *pdev, const un32 maxVerticesBuf, const un32 maxIndicesBuf )
	: GraphicsImpl( pdev ), m_maxIndicesCount( maxVerticesBuf ), m_maxVerticesCount( maxIndicesBuf ) {
	
	m_pvertices = st_new<Vertex>( m_maxVerticesCount );
	m_pindices  = st_new<un16>( m_maxIndicesCount );
	
	m_pdevice->CreateVertexBuffer( 
		sizeof( Vertex ) * m_maxVerticesCount, 
		D3DUSAGE_WRITEONLY, 
		s_textureFVF, 
		D3DPOOL_MANAGED, 
		&m_pvertexBuf,
		NULL );

	m_pdevice->CreateIndexBuffer( 
		sizeof( un16 ) * m_maxIndicesCount,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pindexBuf,
		NULL );
}

Graphics2DImpl::~Graphics2DImpl() {
	st_delete<Vertex>( &m_pvertices );
	st_delete<un16>( &m_pindices );
}

void Graphics2DImpl::fixAll() {
	SetViewTransform( 
	& D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    
	& D3DXVECTOR3 (0.0f, 0.0f, 1.0f),  
	& D3DXVECTOR3 (0.0f, 1.0f, 0.0f) );	
}