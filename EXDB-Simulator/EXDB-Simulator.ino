//*****************************************************************************
//*
//* EXDB-Simulator.ino
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

#include <Arduino.h>

#include "EX-Application.h"

// ...
///////////////////////////////////////////////////////////////////////////////
ERRNO
  _errNo   = ERRNO::NONE;

EXEMODE
  _exeMode = EXEMODE::NONE;

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
VOID setup(VOID)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (DCCEX_Delegate::begin())
    {
    I2CCMD
      I2C_Req;

    I2C_Req._priority = 5;
    I2C_Req._paramW = (WPARAM)0;
    I2C_Req._paramL = (LPARAM)0;

    DCCEX_Delegate::onReceive(DCCEX_APP_VERSION);

    I2C_Req._msg = DCCEX_DEV_UNITTEST;  DCCEX_Delegate::onReceive((PBYTE)&I2C_Req, 0);
    I2C_Req._msg = DCCEX_CMD_CALIBRATE; DCCEX_Delegate::onReceive((PBYTE)&I2C_Req, 0);
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
VOID loop(VOID)
{
  DCCEX_Delegate::run();
  return;
}

//=============================================================================
// DCCEX_Delegate namespace
//-----------------------------------------------------------------------------
namespace DCCEX_Delegate
{
    //=============================================================================
    // EXDB_Simulator namespace
    //-----------------------------------------------------------------------------
    namespace EXDB_Simulator
    {
      namespace
      {
      //=============================================================================
      // Members (private)
      //-----------------------------------------------------------------------------
      DCCEX_Serial
        _objSerial(SERIAL_BAUD);

      EXDB_Cache
        _objCache;

      DCCEX_I2C
        _objI2C;

      std::queue<I2CCMD>
        _objCmdQueue;

      BOOL
        _calibrated = FALSE;

      ULONG
        _lastEvent = 0,
        _lastTick  = 0,
        _lastHeart = millis();

      UINT
        _runtime = 0;
    }

    //=============================================================================
    // Implementation (private)
    //-----------------------------------------------------------------------------
    namespace
    {
      //*****************************************************************************
      //*****************************************************************************
      VOID(* resetFunc) (VOID) = 0; //declare reset function at address 0
    }

    //=============================================================================
    // Implementation (public)
    //-----------------------------------------------------------------------------

    //*****************************************************************************
    //*
    //* Brief:
    //*   ???.
    //*
    //* Parameters:
    //*   None.
    //*
    //* Returns:
    //*   ???.
    //*
    //*****************************************************************************
    BOOL isBusy(VOID)       { return ((WORD)(_exeMode & EXEMODE::BUSY) ? TRUE : FALSE); }
    BOOL isCalibrated(VOID) { return (_calibrated); }

    //*****************************************************************************
    //*
    //* Brief:
    //*   ???.
    //*
    //* Parameters:
    //*   None.
    //*
    //* Returns:
    //*   ???.
    //*
    //*****************************************************************************
    BOOL begin(VOID)
    {
      _objSerial.begin();
      _objCache.begin();

      _objI2C.begin(_objCache.getI2CAddr());

      // Based on our initialization, set the execution mode
      ///////////////////////////////////////////////////////////////////////////////
      _exeMode = (_errNo == ERRNO::NONE ? EXEMODE::IDLE : EXEMODE::NONE);

      return (_exeMode == EXEMODE::IDLE ? TRUE : FALSE);
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
    VOID run(VOID)
    {
      //============================================================================
      // Run the high priority object(s) first.
      //----------------------------------------------------------------------------
      _objSerial.run();
      _objI2C.run();

      //============================================================================
      // Check Command Stack for things-to-do
      //----------------------------------------------------------------------------
      if (_exeMode & (EXEMODE::COMPLETE | EXEMODE::IDLE) && !_objCmdQueue.empty())
        {
        I2CCMD
          I2C_Cmd = _objCmdQueue.front();

        _objCmdQueue.pop();

        _exeMode |= EXEMODE::BUSY;
        _exeMode &= ~(EXEMODE::IDLE | EXEMODE::COMPLETE);

        DCCEX_Delegate::onReceive(I2C_Cmd._msg, I2C_Cmd._paramW, I2C_Cmd._paramL);
        }
      // If complete, transition to IDLE
      ///////////////////////////////////////////////////////////////////////////////
      else if (_exeMode & EXEMODE::COMPLETE)
        {
        _exeMode = EXEMODE::IDLE;
        }
      // Check the clock ticks
      ///////////////////////////////////////////////////////////////////////////////
      else if (_lastEvent && millis() - _lastTick > 1000)
        {
        Serial.print(".");
        _lastTick = millis();

        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        }
      // Check the time for simulating the command
      ///////////////////////////////////////////////////////////////////////////////
      else if (_lastEvent && millis() - _lastEvent > _runtime)
        {
        Serial.println("\nEXDB_Simulator::eventComplete");
        _lastEvent = 0;

        digitalWrite(LED_BUILTIN, LOW);

        if (_exeMode & EXEMODE::CALIBRATE)
          _calibrated = TRUE;

        _exeMode |= EXEMODE::COMPLETE;
        _exeMode &= ~(EXEMODE::UNITTEST | EXEMODE::CALIBRATE | EXEMODE::HOME | EXEMODE::MOVE | EXEMODE::MANEUVER | EXEMODE::BUSY);
        }

      return;
    }

    //*****************************************************************************
    //*
    //* Brief:
    //*   ???.
    //*
    //* Parameters:
    //*   msg   - ???.
    //*   param - ???.
    //*
    //* Returns:
    //*   ???.
    //*
    //*****************************************************************************
    BOOL onReceive(PBYTE data, SIZE_T length)
    {
      Serial.println("EXDB_Simulator::onReceive(--)");
      Serial.flush();

      PI2CCMD
        pI2C_Cmd = (PI2CCMD)data;

      _exeMode |= EXEMODE::BUSY;

      // All priority one jobs get done NOW!
      ///////////////////////////////////////////////////////////////////////////////
      if (pI2C_Cmd->_priority == 1)
        return (DCCEX_Delegate::onReceive(pI2C_Cmd->_msg, pI2C_Cmd->_paramW, pI2C_Cmd->_paramL));

      _objCmdQueue.push(*pI2C_Cmd);

      return (TRUE);
    }
    ///////////////////////////////////////////////////////////////////////////////
    BOOL onReceive(BYTE msg, WPARAM paramW, LPARAM paramL)
    {
      Serial.print("EXDB_Simulator::onReceive(");

      //============================================================================
      // ...
      //----------------------------------------------------------------------------
      switch (msg)
        {
        //============================================================================
        // ...
        //----------------------------------------------------------------------------

        // Show Version/Configuration Information.
        //============================================================================
        case DCCEX_APP_VERSION:
          {
          Serial.println("DCCEX_APP_VERSION, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          break;
          }

        // Reset the CPU.
        //============================================================================
        case DCCEX_APP_RESET:
          {
          Serial.println("DCCEX_APP_RESET, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          resetFunc();
          break;
          }

        // ...
        //============================================================================
        case DCCEX_APP_DECODER:
          {
          Serial.println("DCCEX_APP_DECODER, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          break;
          }

        // ...
        //============================================================================
        case DCCEX_APP_CONFIG:
          {
          Serial.println("DCCEX_APP_CONFIG, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          switch (paramW)
            {
            // ...
            //----------------------------------------------------------------------------
            case DCCEX_I2C_ADDRESS:   _objCache.setI2CAddr((LONG)paramL);   break;
            case DCCEX_DCC_ADDRESS:   _objCache.setDCCAddr((LONG)paramL);   break;

            // ...
            //----------------------------------------------------------------------------
            default:
              _objCache.setCV((WORD)paramW, (LONG)paramL);
              break;
            }

          break;
          }

        // ...
        //============================================================================
        case DCCEX_APP_COMMIT:
          {
          Serial.println("DCCEX_APP_COMMIT, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _objCache.writeCache();
          break;
          }

        // ...
        //============================================================================
        case DCCEX_APP_STATUS:
          {
          Serial.println("DCCEX_APP_STATUS, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          break;
          }

        //=============================================================================
        // ...
        //-----------------------------------------------------------------------------

        // Perform Unit Testing.
        //=============================================================================
        case DCCEX_DEV_UNITTEST:
          {
          Serial.println("DCCEX_DEV_UNITTEST, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _runtime = 5000;
          _exeMode |= (EXEMODE::UNITTEST | EXEMODE::BUSY);
          _lastEvent = millis();

          break;
          }

        // Toggle Debug Mode.
        //=============================================================================
        case DCCEX_DEV_DEBUG:
          {
          Serial.println("DCCEX_DEV_DEBUG, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          break;
          }

        // Toggle Trace Mode.
        //=============================================================================
        case DCCEX_DEV_TRACE:
          {
          Serial.println("DCCEX_DEV_TRACE, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          break;
          }

        //============================================================================
        // ...
        //----------------------------------------------------------------------------

        // Stop Stepper
        //============================================================================
        case DCCEX_CMD_STOP:
          {
          Serial.println("DCCEX_CMD_STOP, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          while (!_objCmdQueue.empty())
            _objCmdQueue.pop();

          digitalWrite(LED_BUILTIN, HIGH);
          delay(100);
          digitalWrite(LED_BUILTIN, LOW);

          _runtime   = 0;
          _exeMode   = EXEMODE::IDLE;
          _lastEvent = 0;

          break;
          }

        // Move Stepper to Home Position.
        //============================================================================
        case DCCEX_CMD_HOME:
          {
          Serial.println("DCCEX_CMD_HOME, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _runtime   = 3000;
          _exeMode  |= (EXEMODE::HOME | EXEMODE::BUSY);
          _lastEvent = millis();

          break;
          }

        // Move Stepper
        //============================================================================
        case DCCEX_CMD_MOVE:
          {
          Serial.println("DCCEX_CMD_MOVE, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _runtime   = 1000;
          _exeMode  |= (EXEMODE::MOVE | EXEMODE::BUSY);
          _lastEvent = millis();

          break;
          }

        // Maneuver stepper to a specific position.
        //============================================================================
        case DCCEX_CMD_MANEUVER:
          {
          Serial.println("DCCEX_CMD_MANEUVER, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _runtime = 5000;
          _exeMode |= (EXEMODE::MANEUVER | EXEMODE::BUSY);
          _lastEvent = millis();

          break;
          }

        // Set SoundFX Volume
        //============================================================================
        case DCCEX_CMD_LOUDNESS:
          {
          Serial.println("DCCEX_CMD_LOUDNESS, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          break;
          }

        // Perform Stepper Calibration.
        //============================================================================
        case DCCEX_CMD_CALIBRATE:
          {
          Serial.println("DCCEX_CMD_CALIBRATE, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _runtime   = 10000;
          _exeMode  |= (EXEMODE::CALIBRATE | EXEMODE::BUSY);
          _lastEvent = millis();

          break;
          }

        //============================================================================
        // ...
        //----------------------------------------------------------------------------

        // ...
        //============================================================================
        case DCCEX_REQ_CVMAP:
          {
          Serial.println("DCCEX_REQ_CVMAP, " + String(paramW) + ", " + String((INT32)paramL) + ")");

          _objI2C.setRequest(DCCEX_REQ_CVMAP);

          _exeMode = EXEMODE::IDLE;
          break;
          }

        //============================================================================
        // ...
        //----------------------------------------------------------------------------

        // Unknown command
        //=============================================================================
        default:
          Serial.println("DCCEX_INVALID(0x" + TO_UPPER(String(msg, HEX)) + ", " + String(paramW) + ", " + String((INT32)paramL) + ")");
          return (FALSE);
        }

      Serial.flush();
      return (TRUE);
    }

    //*****************************************************************************
    //*
    //* Brief:
    //*   ???.
    //*
    //* Parameters:
    //*   msg - ???.
    //*
    //* Returns:
    //*   ???.
    //*
    //*****************************************************************************
    LRESULT onRequest(BYTE msg)
    {
      SIZE_T
        size;

      PBYTE
        data;

      switch (msg)
        {
        // ...
        //============================================================================
        case DCCEX_REQ_CVMAP:
          {
          size = sizeof(CV) * DCCEX_CVMAP;
          data = (PBYTE)malloc(size);

          _objCache.enumCV(data);
          break;
          }

        // ...
        //============================================================================
        default:
          {
          size = sizeof(I2CRES);
          data = (PBYTE)malloc(size);

          ((PI2CRES)data)->_busy = isBusy() ? 1 : 0;
          ((PI2CRES)data)->_calibrated = isCalibrated() ? 1 : 0;
          ((PI2CRES)data)->_bitOption3 = 0;
          ((PI2CRES)data)->_bitOption4 = 0;
          ((PI2CRES)data)->_bitOption5 = 0;
          ((PI2CRES)data)->_bitOption6 = 0;
          ((PI2CRES)data)->_bitOption7 = 0;
          ((PI2CRES)data)->_bitOption8 = 0;

          ((PI2CRES)data)->_stepperPosition = 25810;
          }
        }

      return (MAKELRESULT(data, size));
    }
  }

  //=============================================================================
  // ...
  //-----------------------------------------------------------------------------

  //*****************************************************************************
  //*****************************************************************************
  BOOL begin(VOID)                { return (EXDB_Simulator::begin()); }
  VOID run(VOID)                  { EXDB_Simulator::run(); }

  BOOL onReceive(PBYTE data, SIZE_T length)               { return (EXDB_Simulator::onReceive(data, length)); }
  BOOL onReceive(BYTE msg, WPARAM paramW, LPARAM paramL)  { return (EXDB_Simulator::onReceive(msg, paramW, paramL)); }
  LRESULT onRequest(BYTE msg)                             { return (EXDB_Simulator::onRequest(msg)); }

  //*****************************************************************************
  //*
  //* Brief:
  //*   ???.
  //*
  //* Parameters:
  //*   None.
  //*
  //* Returns:
  //*   ???.
  //*
  //*****************************************************************************
  WORD dictCode2ID(BYTE c)
  {
    for (BYTE indx = 0, max = sizeof(_cmdDict); indx < max; indx++)
      if (_cmdDict[indx].cmdCode == c)
        return (_cmdDict[indx].cmdID);

    return (0);
  }
  ///////////////////////////////////////////////////////////////////////////////
  BYTE dictID2Code(WORD id)
  {
    for (BYTE indx = 0, max = sizeof(_cmdDict); indx < max; indx++)
      if (_cmdDict[indx].cmdID == id)
        return (_cmdDict[indx].cmdCode);

    return ('\0');
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
  //*   ???.
  //*
  //*****************************************************************************
  VOID dumpHEX(PBYTE mem, WORD offset, BYTE page)
  {
    Serial.print(String(fmtHEX((DWORD)mem + offset, 4) + "::"));

    for (WORD indx = 0; indx < page; indx++)
      Serial.print(String(fmtHEX(mem[offset + indx])) + " ");

    Serial.println("");
    return;
  }
  ///////////////////////////////////////////////////////////////////////////////
  String fmtHEX(DWORD val, BYTE digit)
  {
    String
      raw = "00000000" + TO_UPPER(String(val, HEX));

    return (String(raw.substring(raw.length() - digit)));
  }

};

