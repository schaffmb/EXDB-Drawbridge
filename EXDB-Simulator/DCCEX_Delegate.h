//*****************************************************************************
//*
//* DCCEX_Delegate.h
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

#ifndef DCCEX_Delegate_h
#define DCCEX_Delegate_h

#include <Arduino.h>

#include "Defines.h"
#include "Config.h"

//=============================================================================
// DCCEX_Delegate namespace
//-----------------------------------------------------------------------------
namespace DCCEX_Delegate
{
// Implementation
  BOOL begin(VOID);
  VOID run(VOID);

  WORD dictCode2ID(BYTE c);
  BYTE dictID2Code(WORD id);

  VOID dumpHEX(PBYTE mem, WORD offset, BYTE page);
  String fmtHEX(DWORD val, BYTE digit = 2);

  BOOL onReceive(PBYTE data, SIZE_T length);
  BOOL onReceive(BYTE msg, WPARAM paramW = 0, LPARAM paramL = 0);
  LRESULT onRequest(BYTE msg);
};

#endif
