//general Settings

#define CONSOLE_STREAMER_BAUDRATE 9600
#define CONSOLE_STREAMER Serial
#define CONSOLE_NAME "Terminal"
#define DHT11_PIN 16 //defines the pin used it to receive the information.
#define MAXIMUM_WINDOWS_SIZE 120 //defines the maximum quantity of samples that need to send a information
#define MINIMUM_WINDOWS_SIZE 10 //defines the minimum quantity of samples that need to do for sending information
#define HUMIDITY_RANGE 2.0 //all change between +-3% is acceptable
#define TEMPERATURE_RANGE 1.0//all change between +1.5 grade is acceptable
#define ROUND_TIME 5000 //Round time
#define LOCAL_SSID "Project"
#define LOCAL_PASSWORD "admin123"
#define SSID_WIFI "Project"
#define PASSWORD_WIFI "C1A8FBE7"
#define DEVICE_ID "2"
#define DEVICE_NAME "B"
#define MQTT_HOST                 "192.168.0.253"  //Remote host
#define MQTT_PORT                 1883
#define MQTT_TOPIC_OUT            "/sensor/B"
#define MQTT_TOPIC_IN             "/configuration/B"
#define MQTT_MAX_PACKET_SIZE      2000


//"{\"ID\":\"10\",\"TEST\":\"prueba\"}"