//  xlSmartRemote.h - Xlight xlSmartRemote project scale definitions header

#ifndef xlSmartRemote_h
#define xlSmartRemote_h

#include "xliCommon.h"
#include "MyMessage.h"
#include "ArduinoJson.h"

//------------------------------------------------------------------
// Smart Remote Class
//------------------------------------------------------------------
class SmartRemoteClass
{
public:
  String m_SysID;
  String m_SysVersion;    // firmware version
  int m_devStatus;
  String m_tzString;
  String m_lastMsg;

  SmartRemoteClass();
  void Init();

  BOOL Start();
  UC GetStatus();
  BOOL SetStatus(UC st);
  void ResetSerialPort();
  void Restart();

  String GetSysID();
  String GetSysVersion();

  BOOL SelfCheck(US ms);

  // Process all kinds of commands
  void ProcessCommands();

  int CldSetCurrentTime(String tmStr = "");
  int CldJSONCommand(String jsonCmd);

  // Device Control Functions
  int DevSoftSwitch(BOOL sw, UC dev = 0);
};

//------------------------------------------------------------------
// Function & Class Helper
//------------------------------------------------------------------
extern SmartRemoteClass theSys;

#endif /* xlSmartRemote_h */
