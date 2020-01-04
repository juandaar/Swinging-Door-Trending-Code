//Pendant::::Create wifi monitor
#include "WiFi_manager.h"

//Static values initialization
const String WiFi_manager::s_class_name = "WiFi_manager";
const String WiFi_manager::class_context = "WiFi_manager::";

WiFi_manager::WiFi_manager(Console_manager *Console_msg_manager) {
  this->m_console = Console_msg_manager;
  m_wifi_multi = new ESP8266WiFiMulti;
  m_DNSServer = new DNSServer;
  m_initialized = false;
}

void WiFi_manager::init() {
  /****************************************************************/
  const String k_method_name = "Init";				//Mandatory  line
  /****************************************************************/
  if (m_initialized) {m_console->publish(class_context + k_method_name, "the network was initialized previously", ERR_LOG); return ;}

  m_network_state = false;
  m_local_state = false;
  m_captive_portal_state = false;
  m_initialized = true;
  m_console->publish(class_context + k_method_name, "the network was initialized correctly", INF_LOG);
}

bool WiFi_manager::connect_to_wifi(String str_ssid, String str_password) {
  /****************************************************************/
  const String method_name = "connect_to_wifi";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {m_console->publish(class_context + method_name, "Instance not initialized", ERR_LOG); return false;}

  m_ssid = &str_ssid[0u];
  m_password = &str_password[0u];
  short attempts = 0;
  m_console->publish(class_context + method_name, "Connecting to network", INF_LOG);
  m_console->publish(class_context + method_name, "Connecting to\tSSID: " + str_ssid + "\t Password: " + str_password, INF_LOG);
  // We start by connecting to a WiFi network using
  m_wifi_multi->addAP(m_ssid, m_password);
  m_wifi_multi->run();
  delay(4000);
  while (m_wifi_multi->run() != WL_CONNECTED) {

    Serial.print(".");
    if (++attempts == MAX_ATTEMPTS_TO_CONNECT_WIFI) {
      Serial.println("");
      m_console->publish(class_context + method_name, "Impossible connect to the network!", ERR_LOG);
      return false;
    }
	delay(1000);
  }
  //Some importante settings
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  Serial.println("");
  m_console->publish(class_context + method_name, "Wifi connection was successfull!\tIP Address: " + WiFi.localIP().toString(), INF_LOG);
  return true;
}

bool WiFi_manager::disconnect_wifi() {
  /****************************************************************/
  const String method_name = "disconnect_wifi";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {m_console->publish(class_context + method_name, "Instance not initialized", ERR_LOG); return false;}

  if (WiFi.disconnect(true)) {
    m_console->publish(class_context + method_name, "Wifi disconnection was successfull!", INF_LOG);
    return true;
  } else {
    m_console->publish(class_context + method_name, "Wifi disconnection failed", ERR_LOG);
    return false;
  }
}

bool WiFi_manager::state_wifi() {
  /****************************************************************/
  const String method_name = "State wifi";	//Mandatory  line
  if (m_wifi_multi->run() == WL_CONNECTED) return true;

  m_console->publish(class_context + method_name, "Wifi does not work", ERR_LOG);
  return false;
}

bool WiFi_manager::switchOnAP() {
  /****************************************************************/
  const String k_method_name = "switchOnAP";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {m_console->publish(class_context + k_method_name, "Instance not initialized", ERR_LOG); return false;}

  m_console->publish(class_context + k_method_name, "Configuring access point...", INF_LOG);
  const char *password_ap = LOCAL_PASSWORD;
  const char *ssid_ap = LOCAL_SSID;


  for (int i = 0; i < 5; i++) {
    if (WiFi.softAPConfig(LOCAL_IP, LOCAL_IP, LOCAL_MASK)) {
      delay(100);
      break;
    }
    delay(100);
  }

  delay(500);

  if (WiFi.softAP(ssid_ap, password_ap)) {
    delay(1000);

    m_console->publish(class_context + k_method_name, "AP IP address: " + WiFi.softAPIP().toString() + " AP MAC address: " + ((String)WiFi.macAddress()), INF_LOG);
    m_local_state = true;
    return true;
  } else {
    m_console->publish(class_context + k_method_name, "Impossible configure the AP ", ERR_LOG);
    return false;
  }
}

bool WiFi_manager::switchOffAP() {
  /****************************************************************/
  const String k_method_name = "switchOnAP";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {    m_console->publish(class_context + k_method_name, "Instance not initialized", ERR_LOG); return false;}

  if (WiFi.softAPdisconnect()) {
    m_console->publish(class_context + k_method_name, "AP Disconnection was successful", INF_LOG);
    m_local_state = false;
    return true;
  } else {
    m_console->publish(class_context + k_method_name, "There was an error disconnecting the AP ", WAR_LOG);
    return false;
  }
}

bool WiFi_manager::CaptivePortalStart() {
  /****************************************************************/
  const String k_method_name = "captiveportalON";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {m_console->publish(class_context + k_method_name, "Instance not initialized", ERR_LOG); return false;}

  if (m_DNSServer->start(DNS_PORT, "*", WiFi.softAPIP())) {
    m_console->publish(class_context + k_method_name, "Captive Portal was started correctly", INF_LOG);
    m_captive_portal_state = true;
    return true;
  }
  m_console->publish(class_context + k_method_name, "Captive Portal was not  started correctly", ERR_LOG);
  return false;
}

bool WiFi_manager::CaptivePortalStop() {
  /****************************************************************/
  const String k_method_name = "CaptivePortalStop";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {m_console->publish(class_context + k_method_name, "Instance not initialized", ERR_LOG); return false;}

  m_DNSServer->stop();
  m_captive_portal_state = false;
  m_console->publish(class_context + k_method_name, "Captive Portal was not  started correctly", ERR_LOG);
  return true;
}

bool WiFi_manager::loop() {
  /****************************************************************/
  const String k_method_name = "loop";	//Mandatory  line
  /****************************************************************/
  if (!m_initialized) {    m_console->publish(class_context + k_method_name, "Instance not initialized", ERR_LOG); return false;}
  if (m_captive_portal_state)   m_DNSServer->processNextRequest();
  return true;
}
