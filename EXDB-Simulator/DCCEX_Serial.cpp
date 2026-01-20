//*****************************************************************************
//*
//* DCCEX_Serial.cpp
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

#include "DCCEX_Delegate.h"
#include "DCCEX_Serial.h"

// Constructor ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DCCEX_Serial::DCCEX_Serial(ULONG baud) : _baud { baud }
{
}

//*****************************************************************************
//*
//* Brief:
//*   Start Serial communication.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   true if successful, otherwise false.
//*
//*****************************************************************************
BOOL DCCEX_Serial::begin(VOID)
{
  // Basic setup.
  Serial.begin(_baud);
  delay(1000);

  return (_errNo == ERRNO::NONE);
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
VOID DCCEX_Serial::run(VOID)
{
  //=============================================================================
  // ...
  //-----------------------------------------------------------------------------
  while (Serial.available() > 0)
    {
    if (readBuffer((CSTR)Serial.read()))
      {
      dispatchCmd(serialBuffer);

      while (Serial.available() > 0)
        Serial.read();

      break;
      }
    }

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   value - ???.
//*
//* Returns:
//*   ???.
//*
//*****************************************************************************
INT DCCEX_Serial::write(PCSTR value)
{
  return (Serial.write(String(String(value) + "\n").c_str()));
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   serialChar - ???.
//*
//* Returns:
//*   ???.
//*
//*****************************************************************************
BOOL DCCEX_Serial::readBuffer(TCHAR serialChar)
{
  static BYTE
    bytesRecvd     = 0;

  static BOOL
    readInProgress = FALSE;

  //=============================================================================
  // ...
  //-----------------------------------------------------------------------------
  switch (serialChar)
    {
    // ...
    //=============================================================================
    case '<':
      {
      bytesRecvd     = 0;
      readInProgress = TRUE;

      break;
      }

    // ...
    //=============================================================================
    case '>':
      {
      readInProgress = FALSE;

      serialBuffer[bytesRecvd] = '\0';
      return (TRUE);
      }

    // ...
    //=============================================================================
    default:
      {
      if (readInProgress)
        {
        serialBuffer[bytesRecvd++] = serialChar;

        if (bytesRecvd == SERIAL_BUFFER_SZ)
          bytesRecvd = SERIAL_BUFFER_SZ - 1;
        }

      break;
      }
    }

  return (FALSE);
}

//*****************************************************************************
//*
//* Brief:
//*   ???.
//*
//* Parameters:
//*   serialCmd - ???.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
VOID DCCEX_Serial::dispatchCmd(PCHAR serialCmd)
{
  //=============================================================================
  // ...
  //-----------------------------------------------------------------------------
  TCHAR
    *strtokIndex = strtok(serialCmd, " "),
    command = strtokIndex[0];         // first parameter is activity

  strtokIndex  = strtok(NULL, " ");   // space separator

  // ...
  ///////////////////////////////////////////////////////////////////////////////
  DCCEX_Delegate::onReceive(DCCEX_Delegate::dictCode2ID(command), atoi(strtokIndex));
  return;
}
