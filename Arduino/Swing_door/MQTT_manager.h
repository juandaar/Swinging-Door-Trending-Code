// MQTT_manager.h

#ifndef _MQTT_MANAGER_h
#define _MQTT_MANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <PubSubClient.h>	
#include "WiFi_manager.h"
#include "Execution_state.h"
#include "Console_manager.h"

#define MQTT_ECHO_TOPIC												"mqtt_echo_topic"
#define DO_FORWARD_TO_MQTT_ECHO_TOPIC
#define MAX_ATTEMPTS_TO_CONNECT_MQTT					5
#define MAX__NUMBER_OF_ALLOWED_SUBSCRIPTION		20


class MQTT_manager {

private:
	static Console_manager *console;
	static const String class_name;
	static const String class_context;
	char  *device_name;
	char  *server;
	int  port;
	WiFiClient        wifi_client;
	PubSubClient      mqtt_client;
	static int index_subscription_wifi;
	static char *arr_subscribed_topic_names_wifi[MAX__NUMBER_OF_ALLOWED_SUBSCRIPTION];
	static void(*arr_subscribed_topics_callbacks_wifi[MAX__NUMBER_OF_ALLOWED_SUBSCRIPTION])(String);

public:

	MQTT_manager(Console_manager *);
	ExecutionState init(char *device_name, char *mqtt_server, int mqtt_port);
	static void callback_mqtt_wifi_manager(char *, uint8_t *, unsigned int);
	ExecutionState connect_to_brocker();
	ExecutionState loop();
	ExecutionState subscribe(char *, void(*function)(String));
	ExecutionState publish(char *, char *);


};



#endif

