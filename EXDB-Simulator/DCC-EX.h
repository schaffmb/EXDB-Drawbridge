//*****************************************************************************
//*
//* DCC-EX.h
//*   Copyright © 2025-2026 Michael Schaff
//*   All rights reserved.
//*
//* Brief:
//*   ???.
//*
//* Notes:
//*   This is free software: you can redistribute it and/or modify
//*   it under the terms of the GNU General Public License as published by
//*   the Free Software Foundation, either version 3 of the License, or
//*   (at your option) any later version.
//*
//*   It is distributed in the hope that it will be useful,
//*   but WITHOUT ANY WARRANTY; without even the implied warranty of
//*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//*   GNU General Public License for more details.
//*
//*   You should have received a copy of the GNU General Public License
//*   along with the product.  If not, see <https://www.gnu.org/licenses/>.
//*
//*****************************************************************************

#ifndef DCCEX_h
#define DCCEX_h

// ...
///////////////////////////////////////////////////////////////////////////////
typedef char          TCHAR;
typedef unsigned char UCHAR;
typedef TCHAR *       PCHAR;
typedef const TCHAR   CSTR;
typedef const TCHAR * PCSTR;

typedef int           INT;
typedef int8_t        INT8;
typedef int16_t       INT16;
typedef int32_t       INT32;
typedef INT *         PINT;
typedef INT8 *        PINT8;
typedef INT16 *       PINT16;
typedef INT32 *       PINT32;
typedef unsigned int  UINT;
typedef uint8_t       UINT8;
typedef uint16_t      UINT16;
typedef uint32_t      UINT32;
typedef UINT *        PUINT;
typedef UINT8 *       PUINT8;
typedef UINT16 *      PUINT16;
typedef UINT32 *      PUINT32;

typedef INT8          SHORT;
typedef INT8 *        PSHORT;
typedef UINT8         USHORT;
typedef UINT8 *       PUSHORT;

typedef INT32         LONG;
typedef INT32 *       PLONG;
typedef UINT32        ULONG;
typedef UINT32 *      PULONG;

typedef UINT32        DWORD;
typedef PUINT32       PDWORD;
typedef UINT16        WORD;
typedef PUINT16       PWORD;
typedef UINT8         BYTE;
typedef PUINT8        PBYTE;

typedef WORD          WPARAM;
typedef DWORD         LPARAM;

typedef WORD          WRESULT;
typedef DWORD         LRESULT;

typedef size_t        SIZE_T;

typedef UINT8         PIN_T;

typedef float         FLOAT;
typedef double        DOUBLE;

typedef bool          BOOL;
typedef bool *        PBOOL;

typedef void          VOID;
typedef void *        PVOID;

// ...
///////////////////////////////////////////////////////////////////////////////
inline String TO_UPPER(String s) { String _s=s; _s.toUpperCase(); return(_s); }

#define LOWORD(l)     ((WORD)(((DWORD)(l)) & 0xFFFF))
#define HIWORD(l)     ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

#define LOBYTE(w)     ((BYTE)(((WORD)(w)) & 0xFF))
#define HIBYTE(w)     ((BYTE)((((WORD)(w)) >> 8) & 0xFF))

#define MAKEWRESULT(l, h) ((WRESULT)( ((WORD)(l)) | ((WORD)(h) << 8) ))
#define MAKELRESULT(l, h) ((LRESULT)( ((DWORD)(l)) | ((DWORD)(h) << 16) ))

#define MAKEWPARAM(l, h)  ((WPARAM)MAKEWRESULT(l, h))
#define MAKELPARAM(l, h)  ((LPARAM)MAKELRESULT(l, h))

#define TRUE          true
#define FALSE         false

#define SUCCESS       0
#define FAILURE       -1

#endif
