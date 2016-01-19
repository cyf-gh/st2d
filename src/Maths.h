/// ----------------------------------------------------------------------------
/// \file Maths.h
/// \brief maths support for Stom2D
/// ----------------------------------------------------------------------------
#pragma once 

#include <stlib\Maths.h>

/// \addtogroup Stom2D Maths
/// \brief Stom2D maths stuffs interfaces
/// \{

/// ----------------------------------------------------------------------------
/// \brief Dot interface
/// ----------------------------------------------------------------------------
struct Dot {
public:
	/// \brief base point structure implement
	struct Point {		
	public:
		f32 fX, fY;

		Point( KREF( f32 ) fX, KREF( f32 ) fY ) 
			: fX( fX ), fY( fY ) { }
	};
	virtual f32 GetX() const = 0; ///< \brief get x value of a dot
	virtual f32 GetY() const = 0; ///< \brief get y value of a dot

	/// \brief set the x y value
	/// \param[in] x value
	/// \param[in] y value
	virtual void SetXY( KREF( f32 )fX, KREF( f32 )fY ) = 0;
};

/// ----------------------------------------------------------------------------
/// \brief square interface
/// ----------------------------------------------------------------------------
struct Square {
public:  
	static bool IsValid( KREF( Square ) tSq ) {  return tSq.GetHeight() > 0 && tSq.GetWidth() > 0; }
	virtual Dot::Point GetCat1() const = 0;		///< \brief get the left top point of square
	virtual Dot::Point GetCat2() const = 0;		///< \brief get the right bottom point of square
	virtual Dot::Point GetCenter() const = 0;	///< \brief get the center point of square
	virtual f32 GetWidth() const = 0;			///< \brief get width of square
	virtual f32 GetHeight() const = 0;			///< \brief get height of square
	virtual void Scale( KREF( f32 )fScale ) = 0; ///< \brief the square self
													///< \note scale with left top point
	virtual void ScaleCenter( KREF( f32 )fScale ) = 0; ///< \brief the square self
														  ///< \note scale with center point
};
/// \}