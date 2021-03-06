/*
 * MagicShifter3000 OS, Copyright (c) wizards@Work
 * Authors: wizard23(pt), seclorum(jv)
 * Notes: 
 *          All code is conflated by headers (.h):w
 *          Code prefix- is ms*, as in msConfig, msSystem, etc.
 */

extern "C" {
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <math.h>				/* for HUGE_VAL */
#include <json/json.h>
#include <json/jsonparse.h>
#include <json/jsontree.h>
}
//compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig -lc -ljson
#include <FS.h>
// note: local configuration, globals, and system objects get created now.
#include "msConfig.h"

#include "msGlobals.h"
MagicShifterGlobals msGlobals;
// note: beyond this point, please consider the above globals.

//
// !J!  note: here we use a timer to sample the
// middle button (power-down/brightness control)
// it is done in a timer because the AD op is slow
// and therefore better done outside the context
// of the main runloop
//
#include "user_interface.h"

os_timer_t aPowerButtonTimer;
#define POWER_BUTTON_TIMER_PERIOD 125

// !J! TODO: Adjust this:
#define MIN_POWER_LEVEL_THRESHOLD (3.0f)

float calculateVoltage(int adValue, int batVoltCalibration)
{
	float voltage;
	
	if (batVoltCalibration == 0) {
		int ad1V = 1024;
		float r1 = 220, r2 = 820, r3 = 0;	// alpha
		voltage = ((float) (r1 + r2 + r3) * adValue) / (r1 * ad1V);
	} else {
		voltage = (4.2 * adValue) / batVoltCalibration; 
	}

	return voltage;
}

void PowerButtonTimerCallback(void *pArg) {
	os_intr_lock();
	// tickOccured = true;
	msGlobals.ggLastADValue = analogRead(A0);

	// !J! Not needed
	// if (calculateVoltage(msGlobals.ggLastADValue) <= MIN_POWER_LEVEL_THRESHOLD)
	// 	msGlobals.ggFault = FAULT_VERY_LOW_POWER;

	os_intr_unlock();
} // End of timerCallback

void initPowerButtonTimer()
{
	os_timer_setfn(&aPowerButtonTimer, PowerButtonTimerCallback, NULL);
	os_timer_arm(&aPowerButtonTimer, POWER_BUTTON_TIMER_PERIOD, true);
}




#include "msSystem.h"
MagicShifterSystem msSystem;
// note: WebServer and msSystem are in love
#include "WebServer/WebServer.h"
MagicShifterWebServer msWebServer;

// GUI modes, well actually .. modes are more of an 'app' ..
#include "Modes/Modes.h"


// Begin MagicShifter3000 operation
void setup()
{
	// record our bootup time
	msGlobals.ggBootTime = millis();

	// start the system
	msSystem.setup();

	// get the web interface started
	msWebServer.start();

	ServerConfig sysLogInfo;
	// set up syslog ..
	if (msSystem.Settings.getSyslogConfig(&sysLogInfo))
		msSystem.msSysLog.setup(sysLogInfo.hostname);

	msSystem.msSysLog.
		sendSysLogMsg(String("LOG WAS ENABLED.................."));

	msSystem.showBatteryStatus(true);

	// configure all modes available in the main menu

	msGlobals.ggModeList.push_back(&msMagicShake);
	msGlobals.ggModeList.push_back(&msMagicLight);
	msGlobals.ggModeList.push_back(&msMagicMagnet);
	msGlobals.ggModeList.push_back(&msSysText);
	msGlobals.ggModeList.push_back(&msMagicRemote);
	msGlobals.ggModeList.push_back(&msMagicBeat);

#ifdef CONFIG_ENABLE_MIDI
	msGlobals.ggCurrentMode = 6;
	msGlobals.ggModeList.push_back(&msMIDIShifter);
#endif

	msGlobals.ggModeList[msGlobals.ggCurrentMode]->start();

	msModeSelector.start();
}


void loop()
{
	// inside time-frame
	msWebServer.step();

	msSystem.step();

	msGlobals.ggLFrameTime =
		msGlobals.ggCurrentMicros - msGlobals.ggLastFrameMicros;
	msGlobals.ggCurrentFrame++;
	msGlobals.ggLastFrameMicros = msGlobals.ggCurrentMicros;

	if (msSystem.modeMenuActivated) {
		msSystem.setLocalYieldState(false);
		int newMode = msModeSelector.select();
		if (newMode >= 0) {
			// stop all modes..
			msGlobals.ggModeList[msGlobals.ggCurrentMode]->stop();
			msSystem.modeMenuActivated = false;
			msGlobals.ggCurrentMode = newMode;
			msGlobals.ggModeList[msGlobals.ggCurrentMode]->start();
		}
	} else {
		if (msGlobals.ggCurrentMode < msGlobals.ggModeList.size()) {
			// despatch to mode

			msSystem.setLocalYieldState(
				!msGlobals.ggModeList[msGlobals.ggCurrentMode]->step()
				);
		}
	}

	// fault-checks
	if (msGlobals.ggFault > 0) {
		Serial.print("FAULT:");
		Serial.println(String(msGlobals.ggFault));
		switch (msGlobals.ggFault) {
			case FAULT_VERY_LOW_POWER:
				msPowerEmergencyMode.start();
			break;
			case FAULT_NO_ACCELEROMETER:
				msSystem.msLEDs.errorSwipe();
			break;
			default: {};
		}
	}
}
