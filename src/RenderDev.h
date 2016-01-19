/// ----------------------------------------------------------------------------
/// \file RenderDev.h
/// \brief Stom2D main render interface declare
/// ----------------------------------------------------------------------------
#pragma once 

// --- d3d9 implicit load ---
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

// --- StomLIB implicit load ---
#include <stlib\Misc\Def.h>
#pragma comment( lib, "StomLIB.lib" )

#include "Unit.h"

/// \addtogroup Stom2D RenderDev
/// \brief Stom2D render stuffs interfaces
/// \{

/// --- graphics interfaces ---

class QuadColor;
class stRect;
class ITexture;
class IRectangle;

/// ----------------------------------------------------------------------------
/// \brief success results
/// ----------------------------------------------------------------------------
enum ST_SUCCESS {
	ST_ERR_OK_INTERNAL = 10,	///< \brief internal method success
	ST_ERR_OK = 0,				///< \brief normal method success
};
/// ----------------------------------------------------------------------------
/// \brief graphics device interface
/// ----------------------------------------------------------------------------
struct IGraphics {
public:
	/// \brief begin rendering
	virtual stResult Begin() = 0;
	
	/// \brief end rendering
	virtual stResult End() = 0;

	/// \brief force finish rendering
	virtual stResult Flush() = 0;

	/// \brief set the view transform
	/// \param[in] eye position
	/// \param[in] eye look at
	/// \param[in] up vector
	virtual stResult SetViewTransform( D3DXVECTOR3 * pPos, D3DXVECTOR3 * pViewAt, D3DXVECTOR3 * pUp ) = 0;
	
	/// \brief set the projection transform
	/// \param[in] ratio of back buffer
	/// \param[in] near plane
	/// \param[in] far plane
	virtual stResult SetProjTransform( in_f32 fRatio, in_f32 fNear, in_f32 fFar ) = 0;

	/// \brief set world transform
	/// \param[in] world transform matrix
	virtual stResult SetWorldTransform( KREF( D3DXMATRIX ) tMatrix ) = 0;

	/// \brief present the scene to back buffer
	/// \param[in] a pointer to a stRect structure containing the source rectangle. If NULL, the entire source surface is presented
	/// \param[in]  pDestRect is a pointer to a stRect structure containing the destination rectangle, in window client coordinates. If NULL, the entire client area is filled
	virtual stResult Present( stRect * pSrcRect, stRect * pDestRect ) = 0;

	/// \brief Clears one or more surfaces such as a render target, multiple render targets, a stencil buffer, and a depth buffer.
	/// \param[in] Clear a render target to this ARGB color.
	virtual stResult Clear( KREF( D3DCOLOR ) tColor = D3DCOLOR_ARGB(255, 0, 0, 0) ) = 0;
};
/// ----------------------------------------------------------------------------
/// \brief 2D graphics device interface
/// ----------------------------------------------------------------------------
struct IGraphics2D : public IGraphics {
public:
	/// \brief draw the texture into a 2D quad
	/// \note if the pTexture is NULL value, the texture will be remove
	/// \param[in] texture to draw
	/// \param[in] the vertex corresponds to current texture
	virtual stResult DrawQuad( ITexture * pTexture, in_un32 nRenderQueue ) = 0;

	/// \brief draw normal geometry
	/// \param[in] vertices count
	/// \param[in] triangle count
	/// \param[in] index of render queue
	virtual stResult DrawNormal( in_un32 nVerticesCount, in_un32 nTriangleCount, in_un32 nRenderQueue ) = 0;

	/// \brief collect the rects, converts to the quad and pushes into the vertices memory
	/// \param[in] texture to draw
	/// \param[in] the vertex corresponds to current texture
	virtual stResult ColRects( KREF( stRect ) tAppendRect, in_un32 nCurrentVertex, QuadColor * pQuadColor = NULL ) = 0;

	/// \brief give the vertex to vertex buffer
	virtual stResult PushVertexBuffer() = 0;

	/// \brief give the index to index buffer
	virtual stResult PushIndexBuffer() = 0;
};
/// ----------------------------------------------------------------------------
/// \brief simple 2D font renderer interface
/// ----------------------------------------------------------------------------
struct IFontRenderer {
public:
	/// \brief draw the text
	/// \param[in] text to draw
	/// \param[in] count of text to draw
	/// \param[in] pointer to the region to draw the text
	/// \param[in] text color
	/// \param[in] text drawing format check for ID3DXFont::DrawText method format param values
	virtual stResult Draw( 
		const wchar_t *strDrawW, 
		in_un32  nTextCount, 
		stRect * pRect, 
		KREF( D3DCOLOR ) tColor, 
		in_ul32 dFormat ) = 0;

	/// \brief less CPU usage to use preload method
	/// \param[in] text to draw
	/// \param[in] count of text to draw
	virtual stResult PreLoad( const wchar_t *strDraw, in_un32 nTextCount ) = 0;
};
/// ----------------------------------------------------------------------------
/// \brief graphics 2d result
/// ----------------------------------------------------------------------------
enum ST_GRAPHICS2D {
	ST_ERR_SETVIEWTRANSFORM = -4,			///< \brief error when calling SetViewTransform
	ST_ERR_SETPROJTRANSFORM = -5,			///< \brief error when calling SetProjTransform
	ST_ERR_SETWORLDTRANSFORM = - 14,		///< \brief error when calling SetWorldTransform
	ST_ERR_INDEXBUFFERLOCK = -1,			///< \brief error when locking index buffer
	ST_ERR_INDEXBUFFERUNLOCK = -2,			///< \brief error when unlocking index buffer
	ST_ERR_PRESENT = -3,					///< \brief error when calling Present
	ST_ERR_NULLTEXTURE = -10,				///< \brief error when the texture is NULL value
	ST_ERR_SETSTREAMSOURCE = -6,			///< \brief error when calling SetSteamSource
	ST_ERR_SETINDIECES = -13,				///< \brief error when calling SetIndices
	ST_ERR_SETTEXTURE = -7,					///< \brief error when calling SetTexture
	ST_ERR_DRAWINDEXEDPRIMITIVE = -8,		///< \brief error when calling DrawIndexPraimitive
	ST_ERR_VERTEXBUFFERLOCK = -11,			///< \brief error when locking the buffer
	ST_ERR_VERTEXBUFFERUNLOCK = -12,		///< \brief error when unlocking the buffer

	ST_ERR_NOMOREVERTEXALLOC = -9,			///< \brief error no more allocated memory for vertices
};

/// --- render device ---	

/// ----------------------------------------------------------------------------
/// \brief render device interface
/// ----------------------------------------------------------------------------
struct IRenderDevice {
public:
	/// \brief initialize the D3D device and interface
	/// \param[in] window handle
	/// \param[in] full screen or windowed
	/// \param[in] back buffer width
	/// \param[in] back buffer height
	/// \param[in] AA level
	/// \param[in] if the FPS is limited
	virtual stResult Init( 
		HWND hWnd, 
		bool IsWindowed, 
		in_un32 nBufWidth, 
		in_un32 nBufHeight, 
		const n32  nMultiSampleQuality, 
		bool IsFpsLimited 
	) = 0;

	//virtual stResult CreateGraphics2D( Graphics2D ** ppG2D ) = 0;
	
	/// \brief create texture from file
	/// \note  the texture must be NULL
	/// \param[in] file path
	/// \param[out] texture
	virtual stResult CreateTextureFromFile( wchar_t *  strPathW, ITexture ** ppTexture ) = 0;
	
	/// \brief enum the supported resolutions
	/// \param[in] index of resolution
	virtual RECT EnumResolutions( in_un32 nIndex ) = 0;
	
	/// \brief set the texture filter
	/// \param[in] check ST_TEXTUREQUALITRY enum
	/// \param[in] the max anisotropy level, the larger number is, the better quality
	virtual void SetTextureFilter( const n32 nFlag, const n32 nLevel ) = 0;

	/// \brief get the max count of supported resolutions
	virtual int GetMaxResolutionsCount() = 0;
	
	/// \brief get the D3D device handle
	virtual void * GetHandle() = 0;
	
	/// \brief check current mode is windowed or full screen
	virtual n32 IsWindowed() = 0;
};

/// ----------------------------------------------------------------------------
/// \brief render device result
/// ----------------------------------------------------------------------------
enum ST_RENDERDEVICE {
	ST_ERR_INTERFACE = -1,				///< \brief error when creating D3D interface
	ST_ERR_DEVICE = -2,					///< \brief error when creating D3D device
	ST_ERR_DEVICECAPS = -3,				///< \brief error when get device caps
	ST_ERR_ADAPTERDISPLAYMODE = -4,		///< \brief error when change the display mode
	ST_ERR_RESET = -1,					///< \brief error when reset the device
	ST_ERR_TEXTURECREATEFROMFILE = -5,	///< \brief error when create texture from file
	ST_ERR_OK_TEXTUREEXSIT = 1,			///< \brief success but the texture has been created
};

/// ----------------------------------------------------------------------------
/// \brief texture quality level
/// ----------------------------------------------------------------------------
enum ST_TEXTUREQUALITRY {
	ST_NEARESTPOINTSAMLING = 1,	///< \brief nearest point samling
	ST_LINEARFILTERING = 2,		///< \brief linear filtering
	ST_ANISOTROPICFILTERING = 3,///< \brief anisotropic filtering 
};

/// ----------------------------------------------------------------------------
/// \brief Anti-aliasing level
/// ----------------------------------------------------------------------------
enum ST_AA {
	ST_AA_NONE = 0,  ///< \brief close AA
	ST_AA_2X   = 2,  ///< \brief 2x AA
	ST_AA_4X   = 4,  ///< \brief 4x AA
	ST_AA_8X   = 8,  ///< \brief 8x AA
	ST_AA_16X  = 16  ///< \brief 16 AA
};

/// --- render queue ---	

/// ----------------------------------------------------------------------------
/// \brief render queue manager
/// ----------------------------------------------------------------------------
struct IRenderQueueManager : public IUintParent {
public:
	/// \brief get the current render queue genre's need render handle
	/// \param[in] genre of current render queue call GetGenre() as the param
	virtual void * GetRenderHandle( const n32 nGenre ) = 0;

	/// \brief render all visiable render queue
	virtual stResult Render( void ) = 0;
};

/// ----------------------------------------------------------------------------
/// \brief base render queue declare
/// ----------------------------------------------------------------------------
struct IRenderQueue : public IUnit {
public:
	/// \brief get genre of render queue
	virtual n32 GetGenre() const = 0;

	/// \brief check if the render queue is set visiable
	virtual bool IsVisiable() const = 0;

	/// \brief check if the render queue is animation supported
	virtual bool IsDynamic() const = 0;

	/// \brief set animation effect target 
	virtual void SetAnimation( void * pAnimationEffect ) = 0;

	/// \brief set the visiable of render queue
	/// \param[in] render queue is visiable or not
	virtual void SetVisiable( bool IsVisiable ) = 0;

	/// \brief render this render queue
	/// \param[in] render queue manager pointer
	virtual stResult RenderSingle( IRenderQueueManager * pRdrQueueManager ) = 0;
};

/// ----------------------------------------------------------------------------
/// \brief render queue rectangle 
/// ----------------------------------------------------------------------------
struct IRenderQueueRectangle : public IRenderQueue {
public:
	/// \brief set current start vertex index
	/// \param[in] start vertex index
	virtual void SetCurrentSerial( in_un32 nCurrentSerial ) = 0;

	/// \brief get rectangle handle
	virtual void * GetRectangle() = 0;

	/// \brief set the rectangle handle
	/// \param[in] rectangle handle
	virtual void SetRectangle( IRectangle * pRectangle ) = 0;
};

/// ----------------------------------------------------------------------------
/// \brief render queue for texture
/// ----------------------------------------------------------------------------
struct IRenderQueueTexture : public IRenderQueueRectangle {
public:
	/// \brief get texture handle
	virtual void * GetTexture() = 0;

	/// \brief set texture handle
	/// \param[in] texture handle
	virtual void SetTexture( ITexture * pTexture ) = 0;
};

/// ----------------------------------------------------------------------------
/// \brief render queue for font
/// ----------------------------------------------------------------------------
struct IRenderQueueFont : public IRenderQueue {
public:
	/// \brief get current assigned text
	virtual wchar_t *  GetText() const = 0;

	/// \brief get count of visiable char
	virtual un32 GetVisiableCharCount() const = 0;
	
	/// \brief get render queue color
	virtual D3DCOLOR * GetColor() = 0;
};

/// ----------------------------------------------------------------------------
/// \brief render queue genre
/// ----------------------------------------------------------------------------
enum ST_RENDERQUEUEGENTRE {
	ST_RENDERQUEUE_TEXTURE,		///< \brief texture render queue
	ST_RENDERQUEUE_FONT,		///< \brief font render queue
	ST_RENDERQUEUE_RECTANGLE,	///< \brief rectangle render queue
};

/// ----------------------------------------------------------------------------
/// \brief render queue manager result
/// ----------------------------------------------------------------------------
enum ST_RENDERQUEUEMANAGER {
	ST_ERR_DETACHNOTFOUND = -5,					///< \brief no such render queue in the render queue vector
	ST_ERR_INTERNAL_PUSHVERTEXTODEV = -4,		///< \brief error occurs when push the vertex to device
	ST_ERR_INTERNAL_PUSHRECTVERTEXTODEV = -7,	///< \brief internal push vertices to device fails
	ST_ERR_INTERNAL_SETWORLDTRANSFORM = -10,	///< \brief internal set world transform fails
	ST_ERR_NULLRENDERQUEUE = -1,				///< \brief render queue is NULL value
	ST_ERR_NULLRENDERQUEUETEXTURE = -2,			///< \brief render queue texture is NULL value
	ST_ERR_NULLREDNERQUEUERECTANGLE = - 8,		///< \brief render queue rectangle is NULL value
	ST_ERR_RENDERSINGLE = -3,					///< \brief error occurs when rendering single render queue target
	ST_ERR_UNKNOWNRENDERQUEUEGENRE = -6			///< \brief unknown render queue detected
};
/// \}