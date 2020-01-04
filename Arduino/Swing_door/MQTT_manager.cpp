// 
// 
// 

#include "MQTT_manager.h"



//Static values initialization
const String MQTT_manager::class_name = "MQTT_manager";
const String MQTT_manager::class_context = "MQTT_manager::";


MQTT_manager::MQTT_manager(Console_manager *t_console) {
	this->console = t_console;
}


ExecutionState MQTT_manager::init(char *device_name, char *mqtt_server, int mqtt_port) 
{ 
	const String method_name = "init";				
		mqtt_client = wifi_client;
		this->device_name = device_name;
		this->server = mqtt_server;
		this->port = mqtt_port;
		mqtt_client.setServer(this->server, this->port);
		mqtt_client.setCallback(callback_mqtt_wifi_manager);
		if (connect_to_brocker() == EXE_ERROR) return EXE_ERROR;
		return EXE_OK;
	

}

void MQTT_manager::callback_mqtt_wifi_manager(char *topic, uint8_t *payload, unsigned int length) {
	const String method_name = "callback_mqtt_manager";		

	String str_payload;
	for (int i = 0; i < length; i++) {
		str_payload += (char)payload[i];
	}
	console->publish(class_context + "WIFI::" + method_name, "Message arrived [" + String(topic) + "]: " + str_payload, INF_LOG);

	for (int i = 0; i <= index_subscription_wifi; i++) {
		if (strcmp(arr_subscribed_topic_names_wifi[i], topic) == 0) {
			arr_subscribed_topics_callbacks_wifi[i](str_payload);
			console->publish(class_context + "WIFI::" + method_name, "Topic callback was called successfully!", VER_LOG);
		}
	}
}





ExecutionState MQTT_manager::connect_to_brocker() {
	/****************************************************************/
	const String method_name = "connect_to_brocker"; //Mandatory  line
													 /****************************************************************/
	short attempts = 0;
	// Loop until we're reconnected
	while (!mqtt_client.connected()) {
		console->publish(class_context  + method_name, "Attempting MQTT connection to brocker: " + String(server), INF_LOG);
		// Attempt to connect
		if (mqtt_client.connect(device_name)) {
			console->publish(class_context  + method_name, "connected", INF_LOG);
		}
		else {
			if (++attempts == MAX_ATTEMPTS_TO_CONNECT_MQTT) return EXE_ERROR;
			console->publish(class_context +  method_name, "Failed, rc=" + String(mqtt_client.state()) + ", Attempt: " + String(attempts) + ", try again in 3 seconds", WAR_LOG);

		}
	}
	return EXE_OK;
}

ExecutionState MQTT_manager::loop() {
	/****************************************************************/
	const String method_name = "mqtt_loop";					//Mandatory  line
															/****************************************************************/
	if (!mqtt_client.connected()) {
		//Reconnection
		if (connect_to_brocker() == EXE_ERROR) return EXE_ERROR;
	}
	mqtt_client.loop();
	return EXE_OK;
}


ExecutionState MQTT_manager::subscribe(char *topic_to_sub, void(*topic_callback_function)(String)) {
	/****************************************************************/
	const String method_name = "subscribe";				//Mandatory  line
														/****************************************************************/
														//Add to the callback_mqtt_manager lists


		if (++index_subscription_wifi < MAX__NUMBER_OF_ALLOWED_SUBSCRIPTION - 1) {
			if (mqtt_client.connected()) {
				// ... and resubscribe
				if (mqtt_client.subscribe(topic_to_sub)) {
					arr_subscribed_topics_callbacks_wifi[index_subscription_wifi] = topic_callback_function;
					arr_subscribed_topic_names_wifi[index_subscription_wifi] = topic_to_sub;
					console->publish(class_context  + method_name, "Subscription in topic: '" + String(topic_to_sub) + "'  was successfull!", INF_LOG);
					return EXE_OK;
				}
				else {
					console->publish(class_context  + method_name, "Subscription in topic: '" + String(topic_to_sub) + "'  failed!", WAR_LOG);
					return EXE_WARNING;
				}
			}
			else {
				console->publish(class_context  + method_name, "There aren't a stablished connection to the brocker", ERR_LOG);
				return EXE_ERROR;
			}
		}
		else {
			console->publish(class_context  + method_name, "Max number of subscriptions was reached", WAR_LOG);
			return EXE_WARNING;
		}

}

ExecutionState MQTT_manager::publish(char *topic_to_pub, char *msg) {
	/****************************************************************/
	const String method_name = "publish";			//Mandatory  line
													/****************************************************************/
	if (mqtt_client.connected()) {
		// Once connected, publish an announcement...
		if (mqtt_client.publish(topic_to_pub, msg)) {
			console->publish(class_context  + method_name, "the device message  was published in topic: " + String(topic_to_pub) + " successfully!", INF_LOG);
			return EXE_OK;
		}
		else {
			console->publish(class_context  + method_name, "Publication in topic: '" + String(topic_to_pub) + "'  failed!", WAR_LOG);
			return EXE_WARNING;
		}
	}
	else {
		console->publish(class_context + method_name, "There aren't a stablished connection to the brocker", ERR_LOG);
		return EXE_ERROR;
	}
}


int MQTT_manager::index_subscription_wifi = -1;
char *MQTT_manager::arr_subscribed_topic_names_wifi[MAX__NUMBER_OF_ALLOWED_SUBSCRIPTION];
void(*MQTT_manager::arr_subscribed_topics_callbacks_wifi[MAX__NUMBER_OF_ALLOWED_SUBSCRIPTION])(String);
Console_manager *MQTT_manager::console;

