#ifndef _LUADATACONF_H_
#define _LUADATACONF_H_

/* Configure the LUADATA_API marker for static and shared
 * libraries builds.
 */
#ifdef LUADATA_BUILD_AS_DLL
  #ifdef LUADATA_LIB
    #define LUADATA_API __declspec(dllexport)
  #else
    #define LUADATA_API __declspec(dllimport)
  #endif
  #define INLINE_ON_DLL inline
#else
  #define LUADATA_API
  #define INLINE_ON_DLL
#endif

#endif // _LUADATACONF_H_