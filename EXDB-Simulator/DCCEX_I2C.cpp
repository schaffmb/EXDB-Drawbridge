//*****************************************************************************
//*
//* DCCEX_I2C.cpp
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
#include "DCCEX_I2C.h"

// I2C Communication
///////////////////////////////////////////////////////////////////////////////
TwoWire
  *_wireTT;

BYTE
  DCCEX_I2C::_lastRequest = 0x00;

// Constructor ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DCCEX_I2C::DCCEX_I2C(TwoWire *wire)
{
  _wireTT = wire;
}

//*****************************************************************************
//*
//* Brief:
//*   Start I2C communication.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   true if successful, otherwise false.
//*
//*****************************************************************************
BOOL DCCEX_I2C::begin(const USHORT addr)
{
  Serial.println("DCCEX_I2C::begin(0x" + TO_UPPER(String(addr, HEX)) + ")");

  if (addr != 0xFF)
    {
    _wireTT->begin(addr);

    _wireTT->onReceive(DCCEX_I2C::receiveEvent);
    _wireTT->onRequest(DCCEX_I2C::requestEvent);
    }
  else
    _wireTT->begin();

  _wireTT->setClock(I2C_CLOCK);

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
VOID DCCEX_I2C::run(VOID)
{
  ;
  ; // Nothing to do here
  ;

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Function to define the action on a received I2C event.
//*
//* Parameters:
//*   received - ???.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
VOID DCCEX_I2C::receiveEvent(INT received)
{
  if (received)
    {
    PBYTE
      data = (PBYTE)malloc(received);

    for (BYTE indx = 0; _wireTT->available() || indx < received; indx++)
      data[indx] = _wireTT->read();

    DCCEX_Delegate::onReceive(data, received);
    free(data);
    }

  return;
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
VOID DCCEX_I2C::requestEvent(VOID)
{
  static LRESULT
    lResult = MAKELRESULT(0, 0);

  static PBYTE
    data   = NULL,
    packet = NULL;

  static SIZE_T
    size = 0;

  // See if we need new data for the request.
  ///////////////////////////////////////////////////////////////////////////////
  if (size < 1)
    {
    lResult = DCCEX_Delegate::onRequest(_lastRequest);
    data = (PBYTE)LOWORD(lResult);
    }

  // Grab the size and pointer of the packet to send.
  size   = (SIZE_T)HIWORD(lResult);
  packet = (PBYTE)LOWORD(lResult);

  // Send it.
  _wireTT->write(packet, size <= I2C_BUFFER_SZ ? size : I2C_BUFFER_SZ);

  //DCCEX_Delegate::dumpHEX(packet, 0, size <= I2C_BUFFER_SZ ? size : I2C_BUFFER_SZ);

  // See if we're done.
  ///////////////////////////////////////////////////////////////////////////////
  if (size <= I2C_BUFFER_SZ)
    {
    // Yes, get rid of the packet...
    free(data);

    // and reset our data.
    lResult = MAKELRESULT(0, 0);
    size    = 0;

    _lastRequest = 0x00;
    }
  else
    // Otherwise, get setup for the next request.
    lResult = MAKELRESULT(packet + I2C_BUFFER_SZ, size - I2C_BUFFER_SZ);

  return;
}
