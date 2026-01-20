//*****************************************************************************
//*
//* EXDB_Cache.h
//*   Copyright © 2025-2026 Michael Schaff
//*   All rights reserved.
//*
//* Brief:
//*   ???.
//*
//* Notes:
//*   This is free software: you can redistribute it and/or modify it under
//*   the terms of the GNU General Public License as published by the Free
//*   Software Foundation, either version 3 of the License, or (at your
//*   option) any later version.
//*
//*   It is distributed in the hope that it will be useful, but WITHOUT ANY
//*   WARRANTY; without even the implied warranty of MERCHANTABILITY FITNESS
//*   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
//*   details.
//*
//*   You should have received a copy of the GNU General Public License along
//*   with the product. If not, see <https://www.gnu.org/licenses/>.
//*
//*****************************************************************************

#ifndef obj_Cache_h
#define obj_Cache_h

#include <map>
#include <queue>

#include "Version.h"

//=============================================================================
// EXDB_Cache class
//-----------------------------------------------------------------------------
class EXDB_Cache
{
public:
// Constructor
  EXDB_Cache();

// Implementation
public:
  BOOL begin(VOID);

  VOID writeCache(BOOL fullWrite = TRUE);
  VOID clearCache(VOID);
  VOID forceCache(VOID);

// Accessors
public:
  BYTE getVolume(VOID);
  VOID setVolume(BYTE v, BOOL w = FALSE);

  WORD getI2CAddr(VOID);
  VOID setI2CAddr(WORD v, BOOL w = FALSE);

  WORD getDCCAddr(VOID);
  VOID setDCCAddr(WORD v, BOOL w = FALSE);

  LONG getCV(BYTE cv);
  VOID setCV(BYTE cv, LONG v, BOOL w = FALSE);

  VOID enumCV(PBYTE pData);

// Members
private:
  struct
    {
    struct
      {
      TCHAR
        cacheFlag[16]     = { SIGNATURE },
        cacheVersion[16]  = { VERSION };

      } signature;

    struct
      {
      BYTE
        volume = { 10 };

      WORD
        i2cAddr = { DCCEX_I2C_ADDRESS },
        dccAddr = { DCCEX_DCC_ADDRESS };

      } config;

    } _objLastConfig;

  std::map<BYTE, LONG>
    _mapCV;

};

#endif
