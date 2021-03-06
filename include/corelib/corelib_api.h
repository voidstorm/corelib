#pragma once
#include <stdexcept>

#define SCHNARZ_VERSION "0.1.1.1" //major, minor, patch, build

#define LOGGING_ON
#define SM_TIMING 1

#ifdef _DEBUG
#include <iostream>
#ifndef V
#define V(x) std::cout << x << " in file: "<< __FILE__ " at line: " << __LINE__ << std::endl
#endif
#else
#define V(x) 
#endif

#ifdef INLINE
#undef INLINE
#endif
#define INLINE __forceinline
#define NO_INLINE __declspec(noinline)


#ifdef _WIN32
#pragma warning( disable : 4251 )

//--------------------------------------DEBUG SETTINGS---------------------------------

#ifdef _DEBUG
////MEMCHECK MACRO
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#define VT_DO_LEAK_CHECK _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)


//VERBOSE
#include <iostream>
#include <string>
//#define V(x) std::cout << x << " in file: "<< __FILE__ " at line: " << __LINE__ << std::endl



#define VT_EXCEPT(x, msg) throw x(std::string("Exception in file: ") + std::string(__FILE__) + std::string(" in line: ") + std::to_string(__LINE__) + std::string(" -> ") + std::string(msg) + std::string("\n"))
#define VT_EXCEPT_RT(x, msg, e) throw x(std::string("Exception in file: ") + std::string(__FILE__) + std::string(" in line: ") + std::to_string(__LINE__) + std::string(" -> ") + std::string(msg) + std::string("\n") + e.what())


#ifdef LOGGING_ON
#define SYSTEM_LOG_INFO(x, ...) scl::SystemLogger::get().info([]()->auto{return scl::date_str() + scl::info_str();}, x, ##__VA_ARGS__)
#define SYSTEM_LOG_WARN(x, ...) scl::SystemLogger::get().warn([]()->auto{return scl::date_str() + scl::warning_str();},x, ##__VA_ARGS__)
#define SYSTEM_LOG_ERROR(x, ...) scl::SystemLogger::get().error([]()->auto{return scl::date_str() + scl::error_str();},x, ##__VA_ARGS__)
#else 
#define SYSTEM_LOG_INFO(x, ...)
#define SYSTEM_LOG_WARN(x, ...)
#define SYSTEM_LOG_ERROR(x, ...)
#endif

//D3D DEBUG MACROS
#define D3D_DEBUG

#else
//--------------------------------------RELEASE SETTINGS---------------------------------
#define VT_EXCEPT(x, msg) throw x(std::string("Exception in file: ") + std::string(__FILE__) + std::string(" in line: ") + std::to_string(__LINE__) + std::string(" -> ") + std::string(msg) + std::string("\n"))
#define VT_EXCEPT_RT(x, msg, e) throw x(std::string("Exception in file: ") + std::string(__FILE__) + std::string(" in line: ") + std::to_string(__LINE__) + std::string(" -> ") + std::string(msg) + std::string("\n") + e.what())


//#define VK_CHECK_RESULT(x) \
//{ \
//   auto result = (x); \
//      if (result != VkResult::VK_SUCCESS) { \
//         SystemLogger::get().error("Failed to create instance with error: %s", VkErrorHelper::vkResultToStr(result).c_str()); \
//      } \
//} 

#ifdef LOGGING_ON
#define SYSTEM_LOG_INFO(x, ...) scl::SystemLogger::get().info([]()->auto{return scl::date_str() + scl::info_str();}, x, ##__VA_ARGS__)
#define SYSTEM_LOG_WARN(x, ...) scl::SystemLogger::get().warn([]()->auto{return scl::date_str() + scl::warning_str();},x, ##__VA_ARGS__)
#define SYSTEM_LOG_ERROR(x, ...) scl::SystemLogger::get().error([]()->auto{return scl::date_str() + scl::error_str();},x, ##__VA_ARGS__)
#else 
#define SYSTEM_LOG_INFO(x, ...)
#define SYSTEM_LOG_WARN(x, ...)
#define SYSTEM_LOG_ERROR(x, ...)
#endif

#define VT_MEM_CHECK {}
#define V(x) 
#define VT_DO_LEAK_CHECK {}
#endif
#endif


#ifndef VT_PACK32
#define VT_PACK32(h16, l16)	 (unsigned long)(((unsigned long)(h16) << 16) | (unsigned long)(l16)) 
#endif

#ifndef VT_LOWORD
#define VT_LOWORD(l)         ((unsigned short)((unsigned long)(l) & 0xffff))
#endif

#ifndef VT_HIWORD
#define VT_HIWORD(l)         ((unsigned short)((unsigned long)(l) >> 16))
#endif

#ifndef VT_PACK_XY32
#define VT_PACK_XY32 VT_PACK32
#endif

#ifndef VT_GET_X32
#define VT_GET_X32(lp)  ((int)(short)VT_HIWORD(lp))
#endif

#ifndef VT_GET_Y32
#define VT_GET_Y32(lp)  ((int)(short)VT_LOWORD(lp))
#endif

#define RAD2DEG( radian ) ((radian) * (57.29577951f))
#define DEG2RAD( degree ) ((degree) * (0.017453293f))
#define PI 3.1415926535897932384626433832795
#define PI2 6.283185307179586476925286766559



#define VT_TIMING


#define CACHE_LINE_SIZE std::hardware_destructive_interference_size
#define SSE_SIZE std::hardware_destructive_interference_size

#define CACHE_ALIGN __declspec(align(CACHE_LINE_SIZE))

//// every object of type sse_t will be aligned to 16-byte boundary
//struct alignas(16) sse_t {
//   float sse_data[4];
//};
//
//// the array "cacheline" will be aligned to 64-byte boundary
//alignas(CACHE_LINE_SIZE) char cacheline[CACHE_LINE_SIZE];



//False & True Sharing in C++
//
//Remember the remark about false sharing.As of C++17, two interesting compile time constants are added to the thread support library[^ 5].
//
//hardware_destructive_interference_size: the minimum offset between two objects to avoid false sharing.This value can be used as an argument to alignas to keep two fields of a struct apart.
//   hardware_constructive_interference_size : the maximum size of contiguous memory to promote true sharing.
//
//   Basically, these constants provide a portable way to access the L1 data cache line size from C++.