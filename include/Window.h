#ifndef __ST2D_WINDOW_H__
#define __ST2D_WINDOW_H__

#include <core\Def.h>
#include <core\Event.h>

ST_EVENT_START_IMPL
	ST_EVENT_DECLARE( EventMouse,		void, ( n8 x, n8 y, un32 flag ) )
	ST_EVENT_DECLARE( EventMouseWheel,	void, ( n8 x, n8 y, f32 wheel, un32 flag ) )
	ST_EVENT_DECLARE( EventOnSize,		void, ( un32 clientWidth, un32 clientHeight ) )
	ST_EVENT_DECLARE( EventKey,			void, ( un32 keyCode, un32 flag ) )
	ST_EVENT_DECLARE( EventNoParam,		void, ( void ) )
ST_EVENT_END_IMPL

namespace st2d {

st_interface( stWindow ) {
public:

};

namespace stWindowEvents {
	ST_EVENT( EventKey )   pKeyDown = NULL;
	ST_EVENT( EventKey )   pKeyUp   = NULL;
	ST_EVENT( EventMouse ) pMove	= NULL;
	ST_EVENT( EventMouse ) pLBDown	= NULL;
	ST_EVENT( EventMouse ) pLBUp	= NULL;
	ST_EVENT( EventMouse ) pLBDouble= NULL;
	ST_EVENT( EventMouse ) pMBDown	= NULL;
	ST_EVENT( EventMouse ) pMBUp	= NULL;
	ST_EVENT( EventMouse ) pMBDouble= NULL;	
	ST_EVENT( EventMouse ) pRBDown	= NULL;
	ST_EVENT( EventMouse ) pRBUp	= NULL;
	ST_EVENT( EventMouse ) pRBDouble= NULL;
	ST_EVENT( EventMouseWheel ) pMouseWheel = NULL;
	ST_EVENT( EventOnSize )  pSize		= NULL;
	ST_EVENT( EventNoParam ) pClose     = NULL;
	ST_EVENT( EventNoParam ) pPaint     = NULL;
	ST_EVENT( EventNoParam ) pGetFocus  = NULL;
	ST_EVENT( EventNoParam ) pLostFocus = NULL;
} /* stWindowEvents */

} /* st2d */

#endif /* !__ST2D_WINDOW_H__ */
