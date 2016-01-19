/// ----------------------------------------------------------------------------
/// \file Window.h
/// \brief main window interfaces declare
/// ----------------------------------------------------------------------------
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "MathsExtend\Rect.h"
#include <stlib\Misc\Def.h>

struct WindowEventManager;

/// ----------------------------------------------------------------------------
/// \brief window interface
/// ----------------------------------------------------------------------------
struct Window {
public:
	/// \brief window loop 
	virtual int Run() = 0;

	/// \brief get window handle
	virtual HWND GetHandle() const = 0;

	/// \brief get window title
	virtual const wchar_t *GetTitle() const = 0;

	/// \brief get window rectangle
	virtual stRect GetRect() const = 0;

	/// \brief get client rectangle
	virtual stRect GetClientRect() const = 0;

	/// \brief get window style
	virtual DWORD GetStyle() const = 0;

	/// \brief show window
	/// \param[in] if show the window
	virtual void Show( bool IsShow = true ) = 0;
	
	/// \brief hide cursor
	/// \param[in] if show the cursor
	virtual void HideCursor( bool IsShow = true ) = 0; 

	/// \brief show the window to center of screen
	virtual void ShowCenter() = 0;
	
	/// \brief set title of window
	/// \param[in] title of window
	virtual void SetTitle( const wchar_t *strTitleW ) = 0;
	
	/// \brief set rectangle of window
	/// \param[in] rectangle of window
	virtual void SetRect( KREF( stRect ) tRect ) = 0; 

	/// \brief set window style
	/// \param[in] window style
	virtual void SetStyle( KREF( DWORD ) dwStyle ) = 0; 

	/// \brief set window event manager
	/// \param[in] window event manager
	virtual void AttachManager( WindowEventManager * pEvtManager ) = 0;
};

/// ----------------------------------------------------------------------------
/// \brief event manager of window 
/// ----------------------------------------------------------------------------
struct WindowEventManager {
public:
	virtual void OnClose() = 0;
	virtual void OnPaint() = 0;
	virtual void OnSize( const n32 ClientWidth, const n32 ClientHeight ) = 0;
	virtual void OnKeyDown( const n32 KeyCode, const n32 Flag ) = 0;
	virtual void OnKeyUp( const n32 KeyCode, const n32 Flag ) = 0;
	virtual void OnMouseMove( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseWheel( const n32 X, const n32 Y, in_f32 Wheel, const n32 Flag ) = 0;
	virtual void OnMouseLBDown( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseLBUp( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseLBDouble( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseMBDown( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseMBUp( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseMBDouble( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseRBDown( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseRBUp( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnMouseRBDouble( const n32 X, const n32 Y, const n32 Flag ) = 0;
	virtual void OnGetFocus() = 0;
	virtual void OnLostFocus() = 0;
};
