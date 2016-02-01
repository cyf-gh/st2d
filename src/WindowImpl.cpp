#include <base\Str.h>
#include <core\Core.h>
#include <utils\Maths.h>
using namespace stLibUtils::Maths;
using namespace stLibCore;

#include "..\include\Enum.h"
using namespace st2dEnum;

#include "..\include\ErrCode.h"
using namespace st2dErrCode;

#include "..\include\WindowImpl.h"
using namespace st2d;
using namespace st2dImpl;

const wchar_t * const className = L"WinClass";

std::unordered_map<HWND, WindowImpl*> WindowClass::s_wins;

WindowClass::WindowClass() {
	WNDCLASSEX	wcex;

	st_zero_memory( &wcex, sizeof( WNDCLASSEX ) );

    wcex.cbClsExtra     = NULL;
    wcex.cbSize         = sizeof( WNDCLASSEX );
    wcex.cbWndExtra     = NULL;
	wcex.hbrBackground  = ( HBRUSH )GetStockObject( NULL_BRUSH );
    wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wcex.hIcon          = LoadIcon( NULL, IDI_WINLOGO );
    wcex.hIconSm        = LoadIcon( NULL, IDI_WINLOGO );
    wcex.hInstance      = GetModuleHandle( NULL );
    wcex.lpfnWndProc    = bump;
    wcex.lpszClassName  = L"WinClass";
    wcex.lpszMenuName   = NULL;
    wcex.style          = CS_HREDRAW | CS_VREDRAW;

	if ( RegisterClassEx( &wcex ) == 0 ) {
		st_core_return( ST_ERR_REGISTERCLASS );
	}
}

WindowClass::~WindowClass() {
	UnregisterClass( className, GetModuleHandle( NULL ) );
}

LRESULT CALLBACK WindowClass::bump( HWND handle, UINT msg, WPARAM wp, LPARAM lp ) {
	WindowImpl *pwin = s_wins[handle];

	if ( pwin == NULL ) {
        return DefWindowProc( handle, msg, wp, lp );
	}
	
	using namespace stWindowEvents;
	
	switch( msg ){
	case WM_CREATE:
		/* INSERT CODE */
		break;
	case WM_DESTROY:
		/* INSERT CODE */
		break;
	case WM_CLOSE:
		ST_CALL_EVENT( pClose, () );
		return 0;
	case WM_PAINT:
		ST_CALL_EVENT( pPaint, () );
		break;
	case WM_SIZE:
		ST_CALL_EVENT( pSize, ( LOWORD( lp ), HIWORD( lp ) ) );
		break;
	case WM_KEYDOWN:
		ST_CALL_EVENT( pKeyDown, ( wp, lp ) );
		break;
	case WM_KEYUP:
		ST_CALL_EVENT( pKeyUp, ( wp, lp ) );
		break;
	case WM_CHAR:
		/* INSERT CODE */
		break;
	case WM_MOUSEMOVE:
		ST_CALL_EVENT( pMove, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_MOUSEWHEEL:
		ST_CALL_EVENT( pMouseWheel, ( LOWORD( lp ), HIWORD( lp ),
									 ( ( n8 )HIWORD( wp ) ) / ( f32 )WHEEL_DELTA, LOWORD( wp ) ) );
		break;
	case WM_LBUTTONDOWN:
		ST_CALL_EVENT( pLBDown, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_LBUTTONUP:
		ST_CALL_EVENT( pLBUp, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_LBUTTONDBLCLK:
		ST_CALL_EVENT( pLBDouble, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_RBUTTONDOWN:
		ST_CALL_EVENT( pRBDouble, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_RBUTTONUP:
		ST_CALL_EVENT( pRBUp, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_RBUTTONDBLCLK:
		ST_CALL_EVENT( pRBDouble, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_MBUTTONDOWN:
		ST_CALL_EVENT( pMBDown, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_MBUTTONUP:
		ST_CALL_EVENT( pMBUp, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_MBUTTONDBLCLK:
		ST_CALL_EVENT( pMBDouble, ( LOWORD( lp ), HIWORD( lp ), wp ) );
		break;
	case WM_SETFOCUS:
		ST_CALL_EVENT( pGetFocus, () );
		break;
	case WM_KILLFOCUS:
		ST_CALL_EVENT( pLostFocus, () );
		break;
	}
	return DefWindowProc( handle, msg, wp, lp );
}

st2d::stWindow *WindowClass::Create( const stStrW &title, const stRect &rect, const un32 style ) {
	WindowImpl	*pwin  = NULL;
	HWND		handle = NULL;

	handle = CreateWindow( 
			className, 
			pwin->m_ptitle->Data(), 
			style, 
			pwin->m_rect.a.x, 
			pwin->m_rect.a.y, 
			pwin->m_rect.Width(), 
			pwin->m_rect.Height(), 
			NULL, 
			NULL, 
			GetModuleHandle( NULL ), 
			NULL );

	if ( NULL == handle ) {
		st_core_return_with_var( ST_ERR_CREATEWINDOW, pwin );
	} else {
		pwin = st_new_class<WindowImpl>( WindowImpl( handle, title, rect, style ) );
		s_wins[handle] = pwin;
	}
	st_core_return_with_var( ST_NOERR, pwin );
}


WindowImpl::WindowImpl( HWND handle, const stStrW &title, const stRect &rect, const un32 style  ) 
	: m_rect( rect ), m_ptitle( NULL ), m_style( style ), m_isVisiable( false ), m_handle( handle ) {

	m_ptitle = st_new_class<stStrW>( title );
}

WindowImpl::~WindowImpl() {
	std::unordered_map<HWND, WindowImpl*>::iterator i = WindowClass::s_wins.find( m_handle );

	if( i != WindowClass::s_wins.end() ) {
		WindowClass::s_wins.erase(i);
	}
	DestroyWindow( m_handle );
	st_delete_class<stStrW>( &m_ptitle );
}

stRect WindowImpl::ClientRect() {
	RECT rc; 
	GetClientRect( m_handle, & rc ); 

	return stRect().FromRECT( rc ); 
}

WindowImpl &WindowImpl::Show( bool isShow ) {
	int show = 0;
	m_isVisiable = show;
	show = ( isShow ) ? SW_SHOW : SW_HIDE; 
	ShowWindow( m_handle, show );
	return *this;
}

WindowImpl &WindowImpl::ShowCenter() {
	RECT winRect = { 0 };
	stRect rect;

	Show();

	GetWindowRect( m_handle, &winRect );
	rect.FromRECT( winRect );
	memcpy( &rect, &stRect( 
		( GetSystemMetrics( SM_CXSCREEN ) >> 1 ) - ( rect.Width() / 2 ), 		
		( GetSystemMetrics( SM_CXSCREEN ) >> 1 ) - ( rect.Width() / 2 ) + rect.Width(), 
		( GetSystemMetrics( SM_CYSCREEN ) >> 1 ) - ( rect.Height() / 2 ),
		( GetSystemMetrics( SM_CYSCREEN ) >> 1 ) - ( rect.Height() / 2 ) + rect.Height() ), sizeof( stRect ) );
	SetRect( rect );
	return *this;
}

WindowImpl &WindowImpl::SetCursor( bool isShow ) {
	ShowCursor( isShow );
	return *this;
}

WindowImpl &WindowImpl::SetRect( const stLibUtils::Maths::stRect &rect ) {
	memcpy( &m_rect, &rect, sizeof( stRect ) );
	MoveWindow( m_handle, m_rect.a.x, m_rect.b.y, m_rect.Width(), m_rect.Height(), true );
	return *this;
}

WindowImpl &WindowImpl::SetTitle(const stStrW & text) {
	m_ptitle->SetStr( text );
	SetWindowText( m_handle, m_ptitle->Data() );
	return *this;
}

WindowImpl &WindowImpl::SetStyle( const un32 style ) {
	SetWindowLong( m_handle, GWL_STYLE, style );
	return *this;
}

WindowImpl & st2dImpl::WindowImpl::SetTopMost( bool isTop ) {
	HWND insertAfter = ( isTop ) ? HWND_TOPMOST : HWND_NOTOPMOST;

	if( SetWindowPos( m_handle, insertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ) == FALSE ) {
		st_core_return_with_var( ST_ERR_SETTOPMOST, *this );
	} else {
		st_core_return_with_var( ST_NOERR, *this );
	}
}
