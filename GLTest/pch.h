#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

template<typename T> T CastPtr(System::IntPtr ptr) {
	return reinterpret_cast<T>(ptr.ToPointer());
}