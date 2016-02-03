#ifndef __ST2D_WINDOWIMPL_H__
#define __ST2D_WINDOWIMPL_H__

#include <unordered_map>

#include <core\Def.h>
#include <utils\Maths.h>

#include "..\include\Window.h"

class stStrW;

namespace st2dImpl {

class WindowClass {

	friend class WindowImpl;

private:
	static std::unordered_map<HWND, WindowImpl*> 
										s_wins;
	static LRESULT CALLBACK				bump( HWND handle, UINT msg, WPARAM wp, LPARAM lp );

public:
	st2d::stWindow *					Create( const stStrW &title, const stLibUtils::Maths::stRect &rect, const un32 style );
										WindowClass();
										~WindowClass();
};

class WindowImpl : public st2d::stWindow {
	
	friend class WindowClass;
	
protected:
	stLibUtils::Maths::stRect			m_rect;
	stStrW *							m_ptitle;
	un32								m_style;
	bool								m_isVisiable;
	HWND								m_handle;

public:
	n32									Run();

	stLibUtils::Maths::stRect			ClientRect();
	const stLibUtils::Maths::stRect	&	Rect() const { return m_rect; }
	const stStrW &						Title() const { return *m_ptitle; }

	WindowImpl &						Show( bool isShow = true );
	WindowImpl &						ShowCenter();
	WindowImpl &						SetCursor( bool isShow = true );
	WindowImpl &						SetRect( const stLibUtils::Maths::stRect &rect );
	WindowImpl &						SetTitle( const stStrW &text );
	WindowImpl &						SetStyle( const un32 style );
	WindowImpl &						SetTopMost( bool isTop = true );
										WindowImpl( HWND handle, const stStrW &title, const stLibUtils::Maths::stRect &rect, const un32 style  );
	virtual								~WindowImpl();
};

}

#endif /* !__ST2D_WINDOWIMPL_H__ */

