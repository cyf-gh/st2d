/// ----------------------------------------------------------------------------
/// \file stRect.h
/// \brief float rect implements
/// ----------------------------------------------------------------------------
#pragma once 

#include <stlib\Maths.h>
#include "Point.h"

class stPoint;
/// ----------------------------------------------------------------------------
/// \brief float rect implements
/// ----------------------------------------------------------------------------
class stRect : public Square {
protected:
	stPoint m_cat1; ///< \brief left top point
	stPoint m_cat2; ///< \brief right bottom point

protected:
	/// simplify access processing
	/// --------------------------------------------------------------------------------
	f32 getX1() const { return m_cat1.GetX(); }
	f32 getX2() const { return m_cat2.GetX(); }
	f32 getY1() const { return m_cat1.GetY(); }
	f32 getY2() const { return m_cat2.GetY(); }

public:
	/// interface implements
	/// --------------------------------------------------------------------------------
	f32 GetWidth() const { return ( getX2() - getX1() ); }
	f32 GetHeight() const { return ( getY2() - getY1() ); }
	Dot::Point GetCat1() const { return m_cat1.Data(); }
	Dot::Point GetCat2() const { return m_cat2.Data(); }
	Dot::Point GetCenter() const {
			return Dot::Point( 
				( float )st_maths::GetAverage( getX1(), getX2() ),
				( float )st_maths::GetAverage( getY1(), getY2() ) 
			);
	}
	void Scale( in_f32 fScale ) {
		f32 fScaledWid = GetWidth() * fScale;
		f32 fScaledHei = GetHeight() * fScale;;

		m_cat2.SetXY( m_cat1.GetX() + fScaledWid, m_cat1.GetY() + fScaledHei );
	}
	void ScaleCenter( in_f32 fScale ) {
		f32 fScaledWid = GetWidth() * fScale;
		f32 fScaledHei = GetHeight() * fScale;;

		m_cat1.SetXY( GetCenter().fX - ( fScaledWid / 2 ), GetCenter().fY - ( fScaledHei / 2 ) );
		m_cat2.SetXY( GetCenter().fX + ( fScaledWid / 2 ), GetCenter().fY + ( fScaledHei / 2 ) );
	}

	/// extend methods
	/// --------------------------------------------------------------------------------
	bool IsPointIntersect( KREF( stPoint ) tPt ) {
		return ( st_maths::IsInRange( getX1(), getX2(), tPt.GetX() ) &&
				 st_maths::IsInRange( getY1(), getY2(), tPt.GetY() ) ) ? true : false;
	}
	RECT ToRECT() { 
		return st_maths::MakeRect( ( n32 )getX1(), ( n32 )getX2(), ( n32 )getY1(), ( n32 )getY2() );
	}
	stRect FromRECT( KREF( RECT ) rectIn ) {
		m_cat1.SetXY( ( f32 )rectIn.left, ( f32 )rectIn.top );
		m_cat2.SetXY( ( f32 )rectIn.right, ( f32 )rectIn.bottom );
		return * this;
	}

	/// constructors & destructors
	/// --------------------------------------------------------------------------------
	stRect() { }
	stRect( in_f32 fX1, in_f32 fX2, in_f32 fY1, in_f32 fY2 )
		: m_cat1( fX1, fY1 ), m_cat2( fX2, fY2 ) {
	}
	virtual ~stRect() { }
};
