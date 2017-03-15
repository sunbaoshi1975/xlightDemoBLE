/*
This is a demo of xlight BLE interfaces.

Users can send message in Serial console. Please connect a serial port with speed 115200 bps.
And type ? or help for instructions.

*/

#include "application.h"
#include "xliCommon.h"
#include "xlSmartRemote.h"
#include "xlxSerialConsole.h"

void setup()
{
  // System Initialization
  theSys.Init();

	// Wait the system started
	while( millis() < 2000 ) {
		Particle.process();
	}

	// Initialize Serial Console
  theConsole.Init();

  // System Starts
  theSys.Start();
}

// Notes: approximate RTE_DELAY_SELFCHECK ms per loop
/// If you need more accurate and faster timer, do it with sysTimer
void loop()
{
  static UC tick = 0;

  // Process commands
  IF_MAINLOOP_TIMER( theSys.ProcessCommands(), "ProcessCommands" );

  // Self-test & alarm trigger, also insert delay between each loop
  IF_MAINLOOP_TIMER( theSys.SelfCheck(RTE_DELAY_SELFCHECK), "SelfCheck" );
}
