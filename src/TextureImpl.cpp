#include <d3dx9.h>

#include <base\Str.h>

#include <core\Core.h>
using namespace stLibCore;

#include "..\include\ErrCode.h"
using namespace st2dErrCode;

#include "TextureImpl.h"
using namespace st2dImpl;

TextureBaseImpl::TextureBaseImpl( void *pdevice, const stStrW &filePath ) {
	D3DXIMAGE_INFO info = { 0 };

	if( FAILED( D3DXCreateTextureFromFileEx(
		( IDirect3DDevice9 * )pdevice,
		filePath.Data(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_FROM_FILE,
		D3DPOOL_DEFAULT,
		D3DFMT_A8B8G8R8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_COLORVALUE( 0.0f, 0.0f, 0.0f, 1.0f ),
		&info,
		NULL,
		&m_ptexture ) ) ) {
		st_core_return( ST_ERR_CREATETEXTUREFORMFILE );
	}
	m_width  = info.Width;
	m_height = info.Height;

	st_core_return( ST_NOERR );
}

TextureBaseImpl::~TextureBaseImpl() {
	m_ptexture->Release();
}
