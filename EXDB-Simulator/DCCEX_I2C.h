//*****************************************************************************
//*
//* DCCEX_I2C.h
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

#ifndef obj_I2C_h
#define obj_I2C_h

#include <Arduino.h>
#include <Wire.h>

#include "Config.h"

// I2C Communication
///////////////////////////////////////////////////////////////////////////////
extern TwoWire
  *_wireTT;

//=============================================================================
// DCCEX_I2C class
//-----------------------------------------------------------------------------
class DCCEX_I2C
{
public:
// Constructor
  DCCEX_I2C(TwoWire *wire = &Wire);

// Implementation
public:
  BOOL begin(const USHORT addr = DCCEX_I2C_ADDRESS);
  VOID run(VOID);

  static VOID receiveEvent(INT received);
  static VOID requestEvent(VOID);

// Accessors
public:
  TwoWire* getWire() const  { return (_wireTT); }

  VOID setRequest(BYTE v)   { DCCEX_I2C::_lastRequest = v; }

// Members
private:
  static BYTE
    _lastRequest;

};

#endif
