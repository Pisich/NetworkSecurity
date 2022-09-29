#ifndef _X86INTRIN_H_INCLUDED
#define _X86INTRIN_H_INCLUDED

#ifdef _MMX_
#include <mmintrin.h>
#endif

#ifdef _SSE_
#include <xmmintrin.h>
#endif

#ifdef _SSE2_
#include <emmintrin.h>
#endif

#ifdef _SSE3_
#include <pmmintrin.h>
#endif

#ifdef _SSSE3_
#include <tmmintrin.h>
#endif

#ifdef _SSE4a_
#include <ammintrin.h>
#endif

#if defined (_SSE4_2) || defined (SSE4_1_)
#include <smmintrin.h>
#endif

#ifdef _SSE5_
#include <bmmintrin.h>
#endif

#if defined (_AES) || defined (PCLMUL_)
#include <wmmintrin.h>
#endif

  /* For including AVX instructions */
#include <immintrin.h>

#ifdef _3dNOW_
#include <mm3dnow.h>
#endif

#endif /* _X86INTRIN_H_INCLUDED */#pragma once