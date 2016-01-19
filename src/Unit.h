#pragma once
#include <stlib\Misc\Def.h>

struct IUnit {
public:
	virtual wchar_t *  GetTag() const = 0;
	virtual void SetTag( const wchar_t *strTagW ) = 0;
};

struct IUintParent {
public:
	virtual void * FindWithTag( const wchar_t *strTagW ) = 0;
};