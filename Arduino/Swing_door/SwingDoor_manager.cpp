// 
// 
// 

#include "SwingDoor_manager.h"



const String SwingDoor_manager::class_name = "SwingDoor_manager";
const String SwingDoor_manager::class_context = "SwingDoor_manager::";
bool SwingDoor_manager::collected = false;
SwingDoor_manager::SwingDoor_manager()
{
	Serial.begin(CONSOLE_STREAMER_BAUDRATE);
	console = new Console_manager(CONSOLE_STREAMER);
	dht= new DHT11(DHT11_PIN);
	wifi = new WiFi_manager(console);
	mqtt = new MQTT_manager(console);
	Swing_of_humidity = new SDT(console);
	Swing_of_temperature = new SDT(console);

}

ExecutionState SwingDoor_manager::setup()
{
	const String method_name = "Setup";
	console->init(CONSOLE_NAME);
	console->publish(class_context + method_name, "Console streamer was set successfully", INF_LOG);
	wifi->init();
	wifi->connect_to_wifi(SSID_WIFI, PASSWORD_WIFI);
	mqtt->init(DEVICE_NAME, MQTT_HOST, MQTT_PORT);
	if (mqtt->subscribe(MQTT_TOPIC_IN,in_callback) == EXE_ERROR) return EXE_ERROR;
	//Do a topic publication
	if (mqtt->publish(MQTT_TOPIC_OUT, "--") == EXE_ERROR) return EXE_ERROR;
	Round = 1;
	time = micros();
	/*Init of algoritm*/
	dht->read(last_humidity, last_temperature);
	humidity_time = micros();
	Swing_of_humidity->begin(HUMIDITY_RANGE, MAXIMUM_WINDOWS_SIZE, MINIMUM_WINDOWS_SIZE, (double)last_humidity);
	temperature_time = micros();
	Swing_of_temperature->begin(TEMPERATURE_RANGE,MAXIMUM_WINDOWS_SIZE,MINIMUM_WINDOWS_SIZE,(double)last_temperature);
	reference_time = micros();
	time = reference_time - time;
	humidity_time = temperature_time - humidity_time;
	temperature_time = reference_time - temperature_time;
	String String_helper = "";
	JsonDocument.clear();
	JsonStructure = JsonDocument.to<JsonObject>();
	JsonStructure["id"] = DEVICE_ID;
	JsonStructure["Temperature"] = last_temperature;
	JsonStructure["Humidity"] = last_humidity;
	JsonStructure["Humidity time compression"] = humidity_time;
	JsonStructure["Temperature time compression"] = temperature_time;
	JsonStructure["Humidity compression"] = 1;
	JsonStructure["Compression humidity"] = last_humidity;
	JsonStructure["Humidity error"] = HUMIDITY_RANGE;
	JsonStructure["Temperature compression"] = 1;
	JsonStructure["Compression temperature"] = last_temperature;
	JsonStructure["Temperature error"] = HUMIDITY_RANGE;
	JsonStructure["Total process time"] = time;
	JsonStructure["Round"] = Round;
	serializeJson(JsonStructure, String_helper);
	mqtt->publish(MQTT_TOPIC_OUT, &String_helper[0u]);
	/*Init of algoritm*/
	user_init();
	serializeJsonPretty(JsonStructure, String_helper);
	Serial.println(String_helper);
	return EXE_OK;
}

ExecutionState SwingDoor_manager::loop()
{
	
	const String method_name = "Loop";
	String String_helper = "";
	JsonDocument.clear();
	if (collected)
	{
		unsigned long rutine_time;
		rutine_time = micros();
		Round++;
		collected = false;

		JsonDocument.clear();
		if (dht->read(temporal_humidity, temporal_temperature) == 0)
		{ 
			//String_helper = "";
			//String_helper += "New Sample:  TEMPERATURE: " + (String)temporal_temperature + " and HUMIDITY: " + (String)temporal_humidity;
			//console->publish(class_context + method_name, String_helper, INF_LOG);
			//mqtt
			String_helper = "";
			JsonStructure = JsonDocument.to<JsonObject>();
			JsonStructure["id"] = DEVICE_ID;
			humidity_time= micros();
			SDT_I SH = Swing_of_humidity->check((double)temporal_humidity);
			humidity_time = micros() - humidity_time;
			JsonStructure["Humidity"] = temporal_humidity;
			if (SH.state == SDT_VALUE)
			{
				JsonStructure["Humidity compression"] = 1;
				JsonStructure["Compression humidity"] = SH.value;
				

			}
			else JsonStructure["Humidity compression"] = 0;
			JsonStructure["Humidity error"] = SH.error;
			temperature_time= micros();
			SDT_I ST = Swing_of_temperature->check((double)temporal_temperature);
			temperature_time = micros()-temperature_time;
			JsonStructure["Temperature"] = temporal_temperature;
			if (ST.state == SDT_VALUE)
			{
				JsonStructure["Temperature compression"] = 1;
				JsonStructure["Compression temperature"] = ST.value;
				

			}
			else JsonStructure["Temperature compression"] = 0;
			JsonStructure["Temperature error"] = ST.error;
			JsonStructure["Humidity time compression"] = humidity_time;
			JsonStructure["Temperature time compression"] = temperature_time;
			JsonStructure["Total process time"] = time;
			JsonStructure["Round"] = Round;
			serializeJson(JsonStructure, String_helper);
			mqtt->publish(MQTT_TOPIC_OUT, &String_helper[0u]);
			time = micros() - rutine_time;
			String_helper = "";
			serializeJsonPretty(JsonStructure, String_helper);
			Serial.println(String_helper);
			
		}
		else
		{
			console->publish(class_context + method_name, "DHT11 is working wrong", ERR_LOG);
			return EXE_ERROR;
		}

	}
	if (!mqtt_string.equals(""))
	{
		String_helper = "";
		String_helper = "MQTT message received: " + mqtt_string;
		console->publish(class_context + method_name, String_helper, INF_LOG);

		DeserializationError error = deserializeJson(JsonDocument, mqtt_string);

		if (error)
		{
			String_helper = "ERROR: ";
			String_helper += error.c_str();
			console->publish(class_context + method_name, String_helper, ERR_LOG);
			mqtt_string = "";
			return EXE_ERROR;
		}

		JsonStructure = JsonDocument.as<JsonObject>();
		int id = JsonStructure["id"];
		String_helper = "id: " + (String)id;
		console->publish(class_context + method_name, String_helper, INF_LOG);
		String test= JsonStructure["TEST"];
		String_helper = "TEST: " + test;
		console->publish(class_context + method_name, String_helper, INF_LOG);
		mqtt_string = "";
	}
	
	//mqtt loop
	mqtt->loop();

	return EXE_OK;
}



void SwingDoor_manager::user_init(void) {
	//os_timer_setfn - Define a function to be called when the timer fires
	//os_timer_arm -  Enable a millisecond granularity timer.
	os_timer_setfn(&myTimer, timerCallback, NULL);
	os_timer_arm(&myTimer, ROUND_TIME, true); //5 segundos
}

void SwingDoor_manager::timerCallback(void *pArg) {

	collected = true;

}



void SwingDoor_manager::in_callback(String string_received) {
	/****************************************************************/
	const String method_name = "in_callback";               //Mandatory  line
															/****************************************************************/

	mqtt_string = string_received;
}

String SwingDoor_manager::mqtt_string = "";
