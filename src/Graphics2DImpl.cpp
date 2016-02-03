#include "..\include\ErrCode.h"
using namespace stLibCore;
using namespace stLibUtils::Maths;
using namespace st2d::stVertex;
using namespace st2dErrCode;

#include "..\include\Enum.h"
using namespace st2dEnum;

#include "RenderListImpl.h"
#include "Graphics2DImpl.h"
using namespace st2dImpl;


class st2dImpl::CoordImpl {
private:
	const f32						m_screenRatio;
	const f32						m_z;
	const stRect					m_screen;
	const stRect					m_3d;

public:
	QuadImpl						To3D( const stRect &screen );
	Vertex							To3DPoint( const stVec2 &vec );
	stRect							ToScreen( const QuadImpl &q3d );
									CoordImpl( const f32 ratio, const f32 z, const stRect &screen );
									~CoordImpl() { } 
};

ST_INLINE CoordImpl::CoordImpl( const f32 ratio, const f32 z, const stRect &screen )
	  : m_screenRatio( ratio ), m_z( z ), m_screen( screen ), m_3d( -( m_z * m_screenRatio ), m_z * m_screenRatio, m_z, -m_z )
{ }

ST_INLINE CoordImpl::~CoordImpl() { }

ST_INLINE QuadImpl CoordImpl::To3D( const stRect &screen ) {
	const f32 xr = m_3d.Width() / m_screen.Width();
	const f32 yr = ( m_3d.a.y - m_3d.b.y ) / m_screen.Height();

	return QuadImpl( 
		( screen.a.x - m_screen.Center().x ) * xr,
	   -( screen.a.y - m_screen.Center().y ) * yr,
		  screen.Width() * xr,
		  screen.Height() * yr );
}

ST_INLINE Vertex CoordImpl::To3DPoint( const stVec2 &vec ) {
	const f32 xr = m_3d.Width() / m_screen.Width();
	const f32 yr = ( m_3d.a.y - m_3d.b.y ) / m_screen.Height();
	Vertex	  v;

	normal_vertex( &v );
	v.x =  ( vec.x - m_screen.Center().x ) * xr;
	v.y = -( vec.y - m_screen.Center().y ) * yr;

	return v;
}

ST_INLINE stRect CoordImpl::ToScreen( const QuadImpl &q3d ) {
	const f32 xr = m_screen.Width() / m_3d.Width();
	const f32 yr = m_screen.Height() / ( m_3d.a.y - m_3d.b.y );

	return stRect( 
		( q3d.Data(0).x + q3d.Width() )  * xr ,
		( q3d.Data(0).y + q3d.Height() ) * yr ,
		  q3d.Width() * xr,
		  q3d.Width() * yr );
}


Graphics2DImpl::Graphics2DImpl( st2d::stDevice *pdev, const un32 maxVerticesBuf, const un32 maxIndicesBuf, 
	const f32 bufWidth, const f32 bufHeight, const f32 bufRatio )
	: GraphicsImpl( pdev ), m_maxIndicesCount( maxVerticesBuf ), m_maxVerticesCount( maxIndicesBuf ), m_ptrans( NULL ) {
	
	m_pvertices  = st_new<Vertex>( m_maxVerticesCount );
	m_pindices   = st_new<un16>( m_maxIndicesCount );
	m_ptrans	 = st_new_class<CoordImpl>( CoordImpl( bufRatio, 1.0f, stRect( 0.0, static_cast<un32>( bufWidth ), 0.0, static_cast<un32>( bufHeight ) ) ) );

	if ( FAILED( m_pdevice->CreateVertexBuffer( 
		sizeof( Vertex ) * m_maxVerticesCount, 
		D3DUSAGE_WRITEONLY, 
		s_textureFVF, 
		D3DPOOL_MANAGED, 
		&m_pvertexBuf,
		NULL ) ) ) {
		st_core_return( ST_ERR_CREATEVERTEXBUFFER );
	}

	if ( FAILED( m_pdevice->CreateIndexBuffer( 
		sizeof( un16 ) * m_maxIndicesCount,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pindexBuf, 
		NULL ) ) ) {
		st_core_return( ST_ERR_CREATEVERTEXBUFFER );
	}

	fixAll();
	st_core_return( ST_NOERR );
}

Graphics2DImpl::~Graphics2DImpl() {
	st_delete<Vertex>( &m_pvertices );
	st_delete<un16>( &m_pindices );
}

Graphics2DImpl &Graphics2DImpl::PushBuf() {
	st2d::stElementInfo info   = { 0 };
	Vertex *		    pheadV = m_pvertices;
	un16 *				pheadI = m_pindices;

	for ( nbus i = 0; i < m_list.size(); ++i ) {
		memcpy( &info, &m_list[i]->Info(), sizeof( st2d::stElementInfo ) );

		if ( ( ( pheadI + info.CountI ) < ( m_pindices + m_maxIndicesCount ) ) && 
			 ( ( pheadV + info.CountV ) < ( m_pvertices + m_maxVerticesCount ) ) ) {
			st_core_return_with_var( ST_ERR_NOMOREBUF, *this );
		}
		// push vertices
		memcpy( pheadV, m_list[i]->DataV(), sizeof( Vertex ) * info.CountV );
		pheadV += info.CountV;
		// push indices
		memcpy( pheadV, m_list[i]->DataI(), sizeof( un16 ) * info.CountI );
		pheadI += info.CountI;
	}
	st_core_return_with_var( ST_NOERR, *this );
}

void Graphics2DImpl::Render( n32 startEleIndex, n32 endEleIndex ) {
	st2d::stElementInfo info		   = { 0 };
	un32				curIndexIndex  = 0;
	un32				curVertexIndex = 0;

	startEleIndex = ( startEleIndex < 0 ) ? 0 : startEleIndex;
	endEleIndex   = ( endEleIndex < 0 )   ? m_list.size() : endEleIndex;

	for ( nbus i = startEleIndex; i < endEleIndex; ++i ) {
		if ( FAILED( m_pdevice->SetStreamSource( 0, m_pvertexBuf, 0, sizeof( Vertex ) ) ) ) {
			st_core_return( ST_ERR_SETSTREAMSOURCE );
		}
		if ( FAILED( m_pdevice->SetIndices( m_pindexBuf ) ) ) {
			st_core_return( ST_ERR_SETINDIECES );
		}
		if ( FAILED( m_pdevice->SetTexture( 0, NULL ) ) ) {
			st_core_return( ST_ERR_SETTEXTURE );
		}
	
		memcpy( &info, &m_list[i]->Info(), sizeof( st2d::stElementInfo ) );
		
		// load EX handles
		for ( nbus i = 0; i < info.CountEX; ++i ) {
			void * phandle = NULL;
			const un32 genre = m_list[i]->DataEX( i, &phandle );

			switch ( genre ) {
			case ST_ELEMENT_GENRE_MARTRIX :
				if ( FAILED( m_pdevice->SetTransform( D3DTS_WORLD, ( ( st2d::stAnimation * )phandle )->Handle() ) ) ) {
					st_core_return( ST_ERR_SETTEXTURE );
				}
				break;
			case ST_ELEMENT_GENRE_TEXTURE :
				if ( FAILED( m_pdevice->SetTexture( 0, ( ( st2d::stTexture * )phandle )->Handle() ) ) ) {
					st_core_return( ST_ERR_SETTEXTURE );
				}
				break;
			}
		}
		
		if ( FAILED( m_pdevice->DrawIndexedPrimitive( D3DPRIMITIVETYPE( info.PrimtiveGenre ), curVertexIndex, 0, info.CountV, curIndexIndex, info.PrimCount ) ) ) {
			st_core_return( ST_ERR_DRAWINDEXEDPRIMITIVE );
		}

		curVertexIndex += static_cast<un32>( info.CountV );
		curIndexIndex  += static_cast<un32>( info.CountI );
	}
	st_core_return( ST_NOERR );
}


void Graphics2DImpl::fixAll() {
	SetViewTransform( 
	&D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    
	&D3DXVECTOR3 (0.0f, 0.0f, 1.0f),  
	&D3DXVECTOR3 (0.0f, 1.0f, 0.0f) );	
}

st2d::stRenderListElement *Graphics2DImpl::CreateQuad( const stRect &screen, st2d::stTexture *ptexture, st2d::stAnimation *panimation ) {
	QuadImpl *pquad = st_new_class<QuadImpl>( m_ptrans->To3D( screen ) );
	
	pquad->SetEX( 0, ptexture );
	pquad->SetEX( 1, panimation );

	return pquad;
}

st2d::stGraphics2D *Graphics2DImpl::ReleaseQuad( st2d::stRenderListElement **ppquad ) {
	st_delete_class<QuadImpl>( ( QuadImpl ** )ppquad );
}

