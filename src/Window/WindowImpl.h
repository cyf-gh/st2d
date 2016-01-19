/// ----------------------------------------------------------------------------
/// \file WindowImpl.h
/// \brief Stom2D main window implements
/// ----------------------------------------------------------------------------
#pragma once

#include <stlib\String\StringImpl.h>
#include "..\Window.h"
#include "..\RenderDev\RenderQueueImpl.h"
#include "..\Render\FPSControllerImpl.h"
#include "..\Render\FontRendererImpl.h"

/// ----------------------------------------------------------------------------
/// \brief main window implements
/// ----------------------------------------------------------------------------
class stWindowImpl : public Window {
private:
	static stWindowImpl * m_pThis;

	HWND m_hHandle;

	stStringW * m_strTitleW;
	DWORD m_dwStyle;
	stRect m_tWindowRect;
	bool m_IsVisiable;

	WindowEventManager * m_pEvtManager;

private:
	friend LRESULT CALLBACK windowBump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT	messageBump( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

public:
	int Run();
	void Show( bool IsShow = true );
	void HideCursor( bool IsShow = true );
	void ShowCenter();

	HWND GetHandle() const { return m_hHandle; }
	stRect GetClientRect() const;
	const wchar_t *GetTitle() const { return m_strTitleW->GetString(); }
	DWORD GetStyle() const { return m_dwStyle; }
	stRect GetRect() const { return m_tWindowRect; }

	void SetTitle( const wchar_t *strTitleW );
	void SetStyle( KREF( DWORD ) dwStyle ); 
	void SetRect( KREF( stRect ) tRect ); 
	void AttachManager( WindowEventManager * pEvtManager );

	IRenderQueueManager * m_rqm;
	IEffectAnimation * m_pAn;

	stWindowImpl( const wchar_t *strTitleW, WindowEventManager * pEvtManager, KREF( stRect )tWindowRect, DWORD dwWindowStyle );
	virtual ~stWindowImpl();
};