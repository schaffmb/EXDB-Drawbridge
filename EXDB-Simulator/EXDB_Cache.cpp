//*****************************************************************************
//*
//* EXDB_Cache.cpp
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

#include "EX-Application.h"
#include "EXDB_Cache.h"

#if defined(DCCEX_MEM_EEPROM) || defined(DCCEX_MEM_RP2040)
  #include <EEPROM.h>
#elif defined(DCCEX_MEM_FLASH)
  #include <FlashAsEEPROM.h>
#else
  #warning No memory model specified in config.h.
#endif

// Constructor ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EXDB_Cache::EXDB_Cache()
{
  ;
  ; // TODO: Complete
  ;
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
//*   true if successful, otherwise false.
//*
//*****************************************************************************
BOOL EXDB_Cache::begin(VOID)
{
  Serial.println("EXDB_Cache::begin()");

  // Read our configuration table
  //=============================================================================
  WORD
    eeAddr = 0;

  CV
    objCV;

  ///////////////////////////////////////////////////////////////////////////////
  #if defined(DCCEX_MEM_EEPROM)
    {
    //forceCache();

    EEPROM.get(eeAddr, _objLastConfig);
    eeAddr = sizeof(_objLastConfig);

    do
      {
      EEPROM.get(eeAddr, objCV);
      eeAddr += sizeof(CV);

      _mapCV.insert(std::pair<BYTE, LONG>(objCV.cv, objCV.val));

      } while (objCV.cv != 0xFF);
    }
  ///////////////////////////////////////////////////////////////////////////////
  #elif defined(DCCEX_MEM_RP2040)
    {
    EEPROM.begin(256);
    forceCache();
    EEPROM.get(eeAddr, _objLastConfig);
    }
  ///////////////////////////////////////////////////////////////////////////////
  #elif defined(DCCEX_MEM_FLASH)
    {
    if (EEPROM.isValid())
      for (INT max = sizeof(_objLastConfig); eeAddr < max; eeAddr++)
        ((PUINT8)&_objLastConfig)[eeAddr] = EEPROM.read(eeAddr);
    else
      forceCache();
    }
  #endif

//  Serial.println(String(_objLastConfig.signature.cacheFlag));
//  Serial.println(String(_objLastConfig.signature.cacheVersion));

  // Check to make sure our signature and version match before attempting to
  // read the remainder of our Cache.
  //=============================================================================
  if (0 == strcmp(_objLastConfig.signature.cacheFlag, SIGNATURE))
    {
    if (0 == strcmp(_objLastConfig.signature.cacheVersion, VERSION))
      {
//      Serial.println(String(_objLastConfig.config.volume));
//
//      Serial.println(TO_UPPER(String(_objLastConfig.config.i2cAddr, HEX)));
//      Serial.println(TO_UPPER(String(_objLastConfig.config.dccAddr, HEX)));
//
//      for (auto& x : _mapCV)
//        Serial.println(String(x.first) + "::" + String(x.second));
      }
    else
      _errNo = ERRNO::APPVERSION;
    }
  else
    _errNo = ERRNO::APPSIGNATURE;

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
VOID EXDB_Cache::writeCache(BOOL fullWrite)
{
  Serial.println("EXDB_Cache::writeCache()");

  // ...
  //=============================================================================
  INT
    eeAddr = 0;

  ///////////////////////////////////////////////////////////////////////////////
  #if defined(DCCEX_MEM_EEPROM) || defined(DCCEX_MEM_RP2040)
    {
    EEPROM.put(eeAddr, _objLastConfig);
    }
  ///////////////////////////////////////////////////////////////////////////////
  #elif defined(DCCEX_MEM_FLASH)
    {
    for (INT max = sizeof(_objLastConfig); eeAddr < max; eeAddr++)
      EEPROM.write(eeAddr, ((PUINT8)&_objLastConfig)[eeAddr]);
    }
  #endif

  ///////////////////////////////////////////////////////////////////////////////
  #if defined(DCCEX_MEM_RP2040) || defined(DCCEX_MEM_FLASH)
    {
    EEPROM.commit();
    }
  #endif

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Clear Cache.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
VOID EXDB_Cache::clearCache(VOID)
{
  Serial.println("EXDB_Cache::clearCache()");

  // for (INT eeAddr = 0, maxAddr = EEPROM.length(); eeAddr < maxAddr; eeAddr++)
  //   EEPROM.write(eeAddr, 0);

  _mapCV.clear();

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Force an image into the Cache.
//*
//* Parameters:
//*   None.
//*
//* Returns:
//*   None.
//*
//*****************************************************************************
VOID EXDB_Cache::forceCache(VOID)
{
  Serial.println("EXDB_Cache::forceCache()");

  // ...
  //=============================================================================
  clearCache();

  // ...
  //=============================================================================
  WORD
    eeAddr = 0;

  CV
    objCV;

  strcpy(_objLastConfig.signature.cacheFlag, SIGNATURE);
  strcpy(_objLastConfig.signature.cacheVersion, VERSION);

  _objLastConfig.config.volume = 10;

  _objLastConfig.config.i2cAddr = DCCEX_I2C_ADDRESS;
  _objLastConfig.config.dccAddr = DCCEX_DCC_ADDRESS;

  _mapCV.clear();

  _mapCV.insert(std::pair<BYTE, LONG>(DCCEX_CV_RESET,  -1L));
  _mapCV.insert(std::pair<BYTE, LONG>(DCCEX_CV_OPEN, 1024L));
  _mapCV.insert(std::pair<BYTE, LONG>(DCCEX_CV_NTRL,    0L));
  _mapCV.insert(std::pair<BYTE, LONG>(DCCEX_CV_CLSE, 2048L));
  _mapCV.insert(std::pair<BYTE, LONG>(DCCEX_CV_STOP,   -1L));
  _mapCV.insert(std::pair<BYTE, LONG>(0xFF, -1L));

  ///////////////////////////////////////////////////////////////////////////////
  #if defined(DCCEX_MEM_EEPROM) || defined(DCCEX_MEM_RP2040)
    {
    EEPROM.put(eeAddr, _objLastConfig);
    eeAddr = sizeof(_objLastConfig);

    for (auto& x : _mapCV)
      {
      objCV.cv = x.first;
      objCV.val = x.second;

      EEPROM.put(eeAddr, objCV);
      eeAddr += sizeof(CV);
      }
    }
  ///////////////////////////////////////////////////////////////////////////////
  #elif defined(DCCEX_MEM_FLASH)
    {
    for (INT max = sizeof(_objLastConfig); eeAddr < max; eeAddr++)
      EEPROM.write(eeAddr, ((PUINT8)&_objLastConfig)[eeAddr]);

    ;
    ;
    ;
    }
  #endif

  ///////////////////////////////////////////////////////////////////////////////
  #if defined(DCCEX_MEM_RP2040) || defined(DCCEX_MEM_FLASH)
    {
    EEPROM.commit();
    }
  #endif

  return;
}

//*****************************************************************************
//*
//* Brief:
//*   Accessors.
//*
//* Parameters:
//*   Varies.
//*
//* Returns:
//*   Varies.
//*
//*****************************************************************************
BYTE EXDB_Cache::getVolume(VOID)                  { return (_objLastConfig.config.volume); }
VOID EXDB_Cache::setVolume(BYTE v, BOOL w)        {
                                                    _objLastConfig.config.volume = v;

                                                    if (w)
                                                      writeCache();
                                                  }
///////////////////////////////////////////////////////////////////////////////
WORD EXDB_Cache::getI2CAddr(VOID)                 { return (_objLastConfig.config.i2cAddr); }
VOID EXDB_Cache::setI2CAddr(WORD v, BOOL w)       {
                                                    _objLastConfig.config.i2cAddr = v;

                                                    if (w)
                                                      writeCache();
                                                  }
///////////////////////////////////////////////////////////////////////////////
WORD EXDB_Cache::getDCCAddr(VOID)                 { return (_objLastConfig.config.dccAddr); }
VOID EXDB_Cache::setDCCAddr(WORD v, BOOL w)       {
                                                    _objLastConfig.config.dccAddr = v;

                                                    if (w)
                                                      writeCache();
                                                  }
///////////////////////////////////////////////////////////////////////////////
LONG EXDB_Cache::getCV(BYTE cv)                   { return (_mapCV.find(cv)->second);  }
VOID EXDB_Cache::setCV(BYTE cv, LONG v, BOOL w)   {
                                                    std::map<BYTE, LONG>::iterator
                                                      it = _mapCV.find(cv);

                                                    if (it != _mapCV.end())
                                                      it->second = v;

                                                    if (w)
                                                      writeCache();
                                                  }
VOID EXDB_Cache::enumCV(PBYTE pData)              {
                                                    PCV
                                                      pCV = (PCV)pData;

                                                    for (auto& x : _mapCV)
                                                      {
                                                      pCV->cv = x.first;
                                                      pCV->val = x.second;

                                                      pCV++;
                                                      }
                                                  }
