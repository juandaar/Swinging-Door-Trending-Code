// SwingDoor_manager.h

#ifndef _SWINGDOOR_MANAGER_h


#define _SWINGDOOR_MANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <ArduinoJson.h>
#include<DHT11.h>
#include "settings.h"
#include "WiFi_manager.h"
#include "Console_manager.h"
#include "Execution_state.h"
//#include "SD_algorithm.h"}
#include "SDT.h"
#include "MQTT_manager.h"
extern "C" {
#include "user_interface.h"

}


class SwingDoor_manager
{
private:
	Console_manager * console;
	WiFi_manager *wifi;
	MQTT_manager *mqtt;
	SDT *Swing_of_humidity;
	SDT *Swing_of_temperature;
	StaticJsonDocument<300> JsonDocument;
	JsonObject JsonStructure;
	unsigned long time;
	unsigned long humidity_time;
	unsigned long temperature_time;
	unsigned long reference_time;
	float last_temperature;
	float last_humidity;
	/* This variables are static because, they is used in an interruption*/
	 DHT11 *dht;
	 static void timerCallback(void *pArg);
	 void user_init(void);
	 os_timer_t myTimer;
	 static bool collected;
	 float temporal_humidity;
	 float temporal_temperature;
	 static bool init;
	 static void in_callback(String string_received);
	 static String mqtt_string;
	 int Round;

public:
	static const String class_name;
	static const String class_context;
	SwingDoor_manager();
	ExecutionState setup();
	ExecutionState loop();
};



#endif



