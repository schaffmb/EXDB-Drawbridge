//*****************************************************************************
//*
//* Defines.h
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

#ifndef Defines_h
#define Defines_h

#include "DCC-EX.h"

// ...
///////////////////////////////////////////////////////////////////////////////
typedef struct
  {
  BYTE
    _priority,
    _msg;

  WPARAM
    _paramW;

  LPARAM
    _paramL;

  } I2CCMD;

typedef struct
  {
  WORD
    _busy       : 1,
    _calibrated : 1,
    _bitOption3 : 1,
    _bitOption4 : 1,
    _bitOption5 : 1,
    _bitOption6 : 1,
    _bitOption7 : 1,
    _bitOption8 : 1;

  BYTE
    filler[3];

  ULONG
    _stepperPosition;

  } I2CRES;

typedef struct
  {
  BYTE
    cv;

  BYTE
    filler[3];

  LONG
    val;

  } CV;

typedef I2CCMD  *PI2CCMD;
typedef I2CRES  *PI2CRES;
typedef CV      *PCV;

// Execution modes
///////////////////////////////////////////////////////////////////////////////
enum class EXEMODE : WORD
  {
  NONE       = 0b0000000000000000,
  IDLE       = 0b0000000000000001,
  BUSY       = 0b0000000000000010,
  CALIBRATE  = 0b0000000000000100,
  HOME       = 0b0000000000001000,
  MOVE       = 0b0000000000010000,
  MANEUVER   = 0b0000000000100000,
  COMPLETE   = 0b0000000001000000,
  UNITTEST   = 0b0000000010000000,
  INITIALIZE = 0b0000000100000000,
  ERROR      = 0b0000001000000000
  };

inline WORD operator==(EXEMODE a, EXEMODE b)      { return static_cast<WORD>(static_cast<WORD>(a) == static_cast<WORD>(b)); };
inline WORD operator!=(EXEMODE a, EXEMODE b)      { return static_cast<WORD>(static_cast<WORD>(a) != static_cast<WORD>(b)); };
inline BOOL operator&&(EXEMODE a, BYTE b)         { return static_cast<BOOL>(static_cast<WORD>(a) && static_cast<BYTE>(b)); };
inline WORD operator&(EXEMODE a, EXEMODE b)       { return static_cast<WORD>(static_cast<WORD>(a) & static_cast<WORD>(b)); };
inline EXEMODE operator&=(EXEMODE& a, EXEMODE b)  { return a = static_cast<EXEMODE>(static_cast<EXEMODE>(a) & static_cast<EXEMODE>(b)); };
inline EXEMODE operator|(EXEMODE a, EXEMODE b)    { return static_cast<EXEMODE>(static_cast<WORD>(a) | static_cast<WORD>(b)); };
inline EXEMODE operator|=(EXEMODE& a, EXEMODE b)  { return a = static_cast<EXEMODE>(static_cast<EXEMODE>(a) | static_cast<EXEMODE>(b)); };
inline EXEMODE operator~(EXEMODE a)               { return static_cast<EXEMODE>(~static_cast<WORD>(a)); };

// Error codes
///////////////////////////////////////////////////////////////////////////////
enum class ERRNO : USHORT
  {
  NONE = 0,
  NOSERIAL,
  NOWIRE,
  NOCACHE,
  NODECODER,
  NOPWMDRIVER,
  NOSTEPPER,
  NOSOUND,
  NOMICROSD,
  APPSTOP,
  APPSIGNATURE,
  APPVERSION
  };

inline USHORT operator==(ERRNO a, ERRNO b)  { return static_cast<USHORT>(static_cast<USHORT>(a) == static_cast<USHORT>(b)); };
inline USHORT operator!=(ERRNO a, ERRNO b)  { return static_cast<USHORT>(static_cast<USHORT>(a) != static_cast<USHORT>(b)); };

// ...
///////////////////////////////////////////////////////////////////////////////
#define BASCULE             0   // ...
#define VERTICALLIFT        1   // ...
#define BOBTAIL             2   // ...
#define SWINGSPAN           3   // ...

// Stepper motor codes
///////////////////////////////////////////////////////////////////////////////
#define ULN2003_HALF        1
#define ULN2003_FULL        2
#define L9910S_HALF         3
#define L9910S_FULL         4
#define A4988_DRVR          5
#define TMC2209_DRVR        6

// ...
///////////////////////////////////////////////////////////////////////////////
extern ERRNO
  _errNo;

#define _hasERR   (_errNo != ERRNO::NONE)

extern EXEMODE
  _exeMode;

#endif
