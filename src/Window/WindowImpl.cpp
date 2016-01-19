#include "WindowImpl.h"
#include <stlib\Misc\Converter.h>

stWindowImpl * stWindowImpl::m_pThis = NULL;

LRESULT CALLBACK windowBump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    if ( stWindowImpl::m_pThis != NULL )
        return stWindowImpl::m_pThis->messageBump( hWnd, msg, wParam, lParam );
	else
        return DefWindowProc( hWnd, msg, wParam, lParam );
}

stWindowImpl::stWindowImpl( const wchar_t *strTitleW, WindowEventManager * pEvtManager, KREF( stRect )tWindowRect, DWORD dwWindowStyle ) 
	: m_pEvtManager( pEvtManager ), m_strTitleW( new stStringW ), m_tWindowRect( tWindowRect ), 
	m_dwStyle( dwWindowStyle ), m_IsVisiable( true ) {
	m_strTitleW->SetString( strTitleW );
	WNDCLASSEX wcex;
	ZeroMemory( & wcex, sizeof( WNDCLASSEX ) );
    wcex.cbClsExtra     = NULL;
    wcex.cbSize         = sizeof( WNDCLASSEX );
    wcex.cbWndExtra     = NULL;
	wcex.hbrBackground  = ( HBRUSH )GetStockObject( NULL_BRUSH );
    wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wcex.hIcon          = LoadIcon( NULL, IDI_WINLOGO );
    wcex.hIconSm        = LoadIcon( NULL, IDI_WINLOGO );
    wcex.hInstance      = GetModuleHandle( NULL );
    wcex.lpfnWndProc    = windowBump;
    wcex.lpszClassName  = L"ST2DCLASS";
    wcex.lpszMenuName   = NULL;
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
	if ( ! RegisterClassEx( &wcex ) )
		return;
	m_hHandle = CreateWindow( L"ST2DCLASS", strTitleW, dwWindowStyle, tWindowRect.GetCat1().fX, tWindowRect.GetCat1().fY, 
							  tWindowRect.GetWidth(), tWindowRect.GetHeight(), NULL, NULL, GetModuleHandle( NULL ), NULL );
	if ( ! m_hHandle )
		return;
	else 
		m_pThis = this;
}

stWindowImpl::~stWindowImpl() {
	DestroyWindow( m_hHandle );
}

LRESULT	stWindowImpl::messageBump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch ( msg ) {
		case WM_DESTROY :
			PostQuitMessage( 0 );
			return 0;
		case WM_CLOSE :
			if ( m_pEvtManager ) m_pEvtManager->OnClose();
			break;
		case WM_SIZE : {
				int nX = LOWORD( lParam );
				int nY = HIWORD( lParam );
				if ( m_pEvtManager ) m_pEvtManager->OnSize( nX, nY );
			}
			break;
		case WM_KEYDOWN :
			if ( m_pEvtManager ) m_pEvtManager->OnKeyDown( wParam, lParam );
			break;
		case WM_KEYUP :
			if ( m_pEvtManager ) m_pEvtManager->OnKeyUp( wParam, lParam );
			break;
		case WM_MOUSEMOVE :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseMove( LOWORD( lParam ), HIWORD( lParam ), wParam);
			break;
		case WM_MOUSEWHEEL :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseWheel( LOWORD( lParam ), HIWORD( lParam ), ( ( short ) HIWORD( wParam ) ) / ( float ) WHEEL_DELTA, LOWORD( wParam ) );
			break;
		case WM_LBUTTONDOWN :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseLBDown( LOWORD( lParam ), HIWORD( lParam ), wParam );
			break;
		case WM_LBUTTONUP :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseLBUp( LOWORD( lParam ), HIWORD( lParam ), wParam );
			break;
		case WM_LBUTTONDBLCLK :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseLBDouble( LOWORD( lParam ), HIWORD( lParam ), wParam );
			break;
		case WM_RBUTTONDOWN :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseRBDown( LOWORD( lParam ), HIWORD( lParam ), wParam );
			break;
		case WM_RBUTTONUP :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseRBUp( LOWORD( lParam ), HIWORD( lParam ), wParam );
			break;
		case WM_RBUTTONDBLCLK :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseRBDouble( LOWORD( lParam ), HIWORD( lParam ),wParam );
			break;
		case WM_MBUTTONDOWN :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseMBDown( LOWORD( lParam ) , HIWORD( lParam ), wParam );
			break;
		case WM_MBUTTONUP :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseMBUp( LOWORD( lParam ), HIWORD( lParam ),wParam );
			break;
		case WM_MBUTTONDBLCLK :
			if ( m_pEvtManager ) m_pEvtManager->OnMouseMBDouble( LOWORD( lParam ), HIWORD( lParam ),wParam );
			break;
		case WM_SETFOCUS :
			if ( m_pEvtManager ) m_pEvtManager->OnGetFocus();
			break;
		case WM_KILLFOCUS :
			if ( m_pEvtManager ) m_pEvtManager->OnLostFocus();
			break;
		default :
			return DefWindowProc( hWnd, msg, wParam, lParam );
			break;
	}
}

int stWindowImpl::Run() {
    MSG msg = { 0 };
	
    while ( msg.message != WM_QUIT ) {
        if ( PeekMessage( & msg, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( & msg );
            DispatchMessage( & msg );
        }
		if ( m_rqm != NULL ) {
			//m_pAn->DoTransform();
			m_pAn->GetAnimationInfo( ST_X, ST_EFFECTROTATION )->DoRepeat();
			m_rqm->Render();
			Sleep( 1 );
		}
    }
    return ( int )( msg.wParam );
}

stRect stWindowImpl::GetClientRect() const { 
	RECT rc; 
	::GetClientRect( m_hHandle, & rc ); 

	return stRect().FromRECT( rc ); 
}

void stWindowImpl::Show( bool IsShow ) {
	int nShow = 0;
	m_IsVisiable = IsShow;
	nShow = ( IsShow )? SW_SHOW : SW_HIDE; 
	ShowWindow( m_hHandle, nShow );
}

void stWindowImpl::ShowCenter() {
	RECT tWindowRect = { 0 };
	stRect tRect;

	Show();
	GetWindowRect( m_hHandle, & tWindowRect );
	tRect.FromRECT( tWindowRect );
	m_tWindowRect = stRect( 
		( GetSystemMetrics( SM_CXSCREEN ) >> 1 ) - ( tRect.GetWidth() / 2 ), 		
		( GetSystemMetrics( SM_CXSCREEN ) >> 1 ) - ( tRect.GetWidth() / 2 ) + tRect.GetWidth(), 
		( GetSystemMetrics( SM_CYSCREEN ) >> 1 ) - ( tRect.GetHeight() / 2 ),
		( GetSystemMetrics( SM_CYSCREEN ) >> 1 ) - ( tRect.GetHeight() / 2 ) + tRect.GetHeight()
	);
	SetRect( m_tWindowRect );
}

void stWindowImpl::HideCursor( bool IsShow ) {
	::ShowCursor( IsShow );
}

void stWindowImpl::SetTitle( const wchar_t *strTitleW ) {
	m_strTitleW->SetString( strTitleW );
	::SetWindowText( m_hHandle, m_strTitleW->GetString() );
}

void stWindowImpl::SetStyle( KREF( DWORD ) dwStyle ) {
	::SetWindowLong( m_hHandle, GWL_STYLE, dwStyle );
}

void stWindowImpl::SetRect( KREF( stRect ) tRect ) {
	m_tWindowRect = tRect;
	MoveWindow( 
		m_hHandle, 
		m_tWindowRect.GetCat1().fX, 
		m_tWindowRect.GetCat1().fY, 
		m_tWindowRect.GetWidth(), 
		m_tWindowRect.GetHeight(), 
		true );
}

void stWindowImpl::AttachManager( WindowEventManager * pEvtManager ) {
	m_pEvtManager = pEvtManager;
}
