#ifndef __ST2D_TEXTUREIMPL_H__
#define __ST2D_TEXTUREIMPL_H__

#include <d3d9.h>
#include <core\Def.h>

class stStrW;

namespace st2dImpl {

class TextureBaseImpl {
protected:
	IDirect3DTexture9 *		m_ptexture;
	un32					m_height;
	un32					m_width;

public:
	IDirect3DTexture9 *		Handle() const { return m_ptexture; }
	un32					Height() const { return m_height; }
	un32					Width() const { return m_width; }

							TextureBaseImpl( void *pdevice, const stStrW &filePath );
	virtual					~TextureBaseImpl();
};

} /* st2dImpl */

#endif /* !__ST2D_TEXTUREIMPL_H__ */