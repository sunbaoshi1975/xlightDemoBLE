/**
 * xlSmartRemote.cpp - contains the major implementation of SmartRemoteClass.
 *
 * Created by Baoshi Sun <bs.sun@datatellit.com>
 * Copyright (C) 2015-2016 DTIT
 * Full contributor list:
 *
 * Documentation:
 * Support Forum:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Created by Baoshi Sun <bs.sun@datatellit.com>
 *
 * DESCRIPTION
 * 1.
 *
 * ToDo:
**/
#include "xlSmartRemote.h"
#include "xlxBLEInterface.h"
#include "xlxSerialConsole.h"

#include "ArduinoJson.h"

// Cloud variables
#define CLV_LastMessage         "lastMsg"         // Can also be a Particle Object

// Notes: The length of the funcKey is limited to a max of 12 characters.
// Cloud functions
#define CLF_JSONCommand         "JSONCommand"     // Can also be a Particle Object

//------------------------------------------------------------------
// Global Data Structures & Variables
//------------------------------------------------------------------
// make an instance for main program
SmartRemoteClass theSys;

//------------------------------------------------------------------
// Smart Controller Class
//------------------------------------------------------------------
SmartRemoteClass::SmartRemoteClass()
{
}

// Primitive initialization before loading configuration
void SmartRemoteClass::Init()
{
  // Open Serial Port
  TheSerial.begin(SERIALPORT_SPEED_DEFAULT);

#ifdef SYS_SERIAL_DEBUG
	// Wait Serial connection so that we can see the starting information
	while(!TheSerial.available()) { Particle.process(); }
	SERIAL_LN(F("BLE demo is starting..."));
#endif

	// Get System ID
	m_SysID = System.deviceID();
	m_SysVersion = System.version();
	m_devStatus = STATUS_INIT;

  // Init CLoud Objects
  Particle.variable(CLV_LastMessage, &m_lastMsg, STRING);
  Particle.function(CLF_JSONCommand, &SmartRemoteClass::CldJSONCommand, this);

  // Open BLE Interface
  theBLE.Init(D3, D1);
}

// Get the remote started
BOOL SmartRemoteClass::Start()
{
	SERIAL_LN(F("BLE demo started."));
	SERIAL_LN("System Info: %s-%s",
			GetSysID().c_str(), GetSysVersion().c_str());

#ifndef SYS_SERIAL_DEBUG
	ResetSerialPort();
#endif

	return true;
}

void SmartRemoteClass::Restart()
{
	SetStatus(STATUS_RST);
	delay(1000);
	System.reset();
}

UC SmartRemoteClass::GetStatus()
{
	return (UC)m_devStatus;
}

BOOL SmartRemoteClass::SetStatus(UC st)
{
	if( st > STATUS_ERR ) return false;
	SERIAL_LN(F("System status changed from %d to %d"), m_devStatus, st);
	if ((UC)m_devStatus != st) {
		m_devStatus = st;
	}
	return true;
}

// Close and reopen serial port to avoid buffer overrun
void SmartRemoteClass::ResetSerialPort()
{
	TheSerial.end();
	TheSerial.begin(SERIALPORT_SPEED_DEFAULT);
}

String SmartRemoteClass::GetSysID()
{
	return m_SysID;
}

String SmartRemoteClass::GetSysVersion()
{
	return m_SysVersion;
}

BOOL SmartRemoteClass::SelfCheck(US ms)
{
	// Check System Status
	if( GetStatus() == STATUS_ERR ) {
		SERIAL_LN("System is about to reset due to STATUS_ERR...");
		Restart();
	}

	// ToDo:add any other potential problems to check
	//...

	delay(ms);
	return true;
}

// Process all kinds of commands
void SmartRemoteClass::ProcessCommands()
{
	// Process Console Command
  theConsole.processCommand();

  // Process BLE commands
  theBLE.processCommand();
}

//------------------------------------------------------------------
// Device Control Functions
//------------------------------------------------------------------
// Turn the switch of specific device and all devices on or off
/// Input parameters:
///   sw: true = on; false = off
///   dev: device id or 0 (all devices under this controller)
int SmartRemoteClass::DevSoftSwitch(BOOL sw, UC dev)
{
	String strCmd = String::format("%d:7:%d", dev, sw ? 1 : 0);
	//return theRadio.ProcessSend(strCmd);
	return 0;
}

//------------------------------------------------------------------
// Place all util func below
//------------------------------------------------------------------
// Format:
/// 1. Date: YYYY-MM-DD
/// 2. Time: hh:mm:ss
/// 3. Date and time: YYYY-MM-DD hh:mm:ss
/// 4. "" or "sync": synchronize with could
int SmartRemoteClass::CldSetCurrentTime(String tmStr)
{
	// synchronize with cloud
	tmStr.trim();
	tmStr.toLowerCase();
	if(tmStr.length() == 0 || tmStr == "sync") {
		//theConfig.CloudTimeSync();
		return 0;
	}

	US _YYYY = Time.year();
	UC _MM = Time.month();
	UC _DD = Time.day();
	UC _hh = Time.hour();
	UC _mm = Time.minute();
	UC _ss = Time.second();

	int nPos, nPos2, nPos3;
	String strTime = "";
	nPos = tmStr.indexOf('-');
	if( nPos > 0 ) {
		_YYYY = (US)(tmStr.substring(0, nPos).toInt());
		if( _YYYY < 1970 || _YYYY > 2100 ) return 1;
		nPos2 = tmStr.indexOf('-', nPos + 1);
		if( nPos2 < nPos + 1 ) return 2;
		_MM = (UC)(tmStr.substring(nPos + 1, nPos2).toInt());
		if( _MM <= 0 || _MM > 12 ) return 2;
		nPos3 = tmStr.indexOf(' ', nPos2 + 1);
		if( nPos3 < nPos2 + 1 ) {
			_DD = (UC)(tmStr.substring(nPos2 + 1).toInt());
		} else {
			_DD = (UC)(tmStr.substring(nPos2 + 1, nPos3).toInt());
			strTime = tmStr.substring(nPos3 + 1);
		}
		if( _DD <= 0 || _DD > 31 ) return 3;
	} else {
		strTime = tmStr;
	}

	strTime.trim();
	nPos = strTime.indexOf(':');
	if( nPos > 0 ) {
		_hh = (UC)(strTime.substring(0, nPos).toInt());
		if( _hh > 24 ) return 4;
		nPos2 = strTime.indexOf(':', nPos + 1);
		if( nPos2 < nPos + 1 ) return 5;
		_mm = (UC)(strTime.substring(nPos + 1, nPos2).toInt());
		if( _mm > 59 ) return 5;
		_ss = (UC)(strTime.substring(nPos2 + 1).toInt());
		if( _ss > 59 ) return 6;
	}

	time_t myTime = tmConvert_t(_YYYY, _MM, _DD, _hh, _mm, _ss);
	//myTime -= theConfig.GetTimeZoneDSTOffset() * 60;	// Convert to local time
	Time.setTime(myTime);
	SERIAL_LN("setTime to %d-%d-%d %d:%d:%d", _YYYY, _MM, _DD, _hh, _mm, _ss);
	return 0;
}

int SmartRemoteClass::CldJSONCommand(String jsonCmd)
{
	SERIAL_LN("Received JSON cmd: %s", jsonCmd.c_str());

	return 1;
}
