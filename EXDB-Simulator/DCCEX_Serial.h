//*****************************************************************************
//*
//* DCCEX_Serial.h
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

#ifndef obj_Serial_h
#define obj_Serial_h

#include <SoftwareSerial.h>

#include "Defines.h"
#include "Config.h"

// Serial communication configuration
///////////////////////////////////////////////////////////////////////////////
#define SERIAL_BUFFER_SZ      40  // Size of serial buffer

//=============================================================================
// DCCEX_Serial class
//-----------------------------------------------------------------------------
class DCCEX_Serial
{
public:
// Constructor
  DCCEX_Serial(ULONG baud = SERIAL_BAUD);

// Implementation
public:
  BOOL begin(VOID);
  VOID run(VOID);

  INT write(PCSTR value);

private:
  BOOL readBuffer(TCHAR serialChar);
  VOID dispatchCmd(PCHAR serialBuffer);

// Members
private:
  ULONG
    _baud;

protected:
  TCHAR
    serialBuffer[SERIAL_BUFFER_SZ];

};

#endif
