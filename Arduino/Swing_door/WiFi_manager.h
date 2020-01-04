// WiFi_manager.h

#ifndef _WIFI_MANAGER_h
#define _WIFI_MANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
  //LIBRARIES INCLUSION
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "Console_manager.h"


//changes the LOCAL NETWORK SSID depending of the device type
#if !defined(LOCAL_SSID)
  #define LOCAL_SSID "Proyect"
#endif


//LOCAL PASSWORD DEFINITION
#if !defined(LOCAL_PASSWORD)
  #define  LOCAL_PASSWORD "admin123"
#endif

//LOCAL NETWORK IP
#ifndef LOCAL_IP
  #define LOCAL_IP IPAddress(192,168,4, 1)
#endif
//LOCAL NETWORK MASK
#ifndef LOCAL_MASK
  #define LOCAL_MASK IPAddress(255,255,255,0)
#endif
//DNS PORT for the Captive portal
#ifndef DNS_PORT
  #define DNS_PORT 53
#endif
//maximum  attemps  to connect to a wifi network
#ifndef  MAX_ATTEMPTS_TO_CONNECT_WIFI
  #define MAX_ATTEMPTS_TO_CONNECT_WIFI    20
#endif


class WiFi_manager {
 private:
  //**********************************************VARIABLES*************************************************************
  bool m_initialized; //initialization state. false: noninitialized and true: initialized
  // Update these with values suitable for your network.
  char *m_ssid, *m_password; //saves the  ssid and password of a wi-fi network
  DNSServer *m_DNSServer; //DNS server object pointer
  ESP8266WiFiMulti *m_wifi_multi; //wifi multi object pointer
  bool m_local_state; //local state. true: activated and false; disabled
  bool m_network_state;//external network state. true: activated and functional, false : disabled and nonfunctional
  bool m_captive_portal_state; //captive portal state. true: activated and functional, false: disabled and nonfunctional

  //********************************************** DRIVERS *************************************************************
  Console_manager *m_console;
 public:
  //**********************************************VARIABLES*************************************************************

  //********************************************** METHODS *************************************************************
  WiFi_manager(Console_manager *); //constructor
  void init(); //initialization, puts everything in false (local state, network state, captive portal state) and initialized in true.
  bool connect_to_wifi(String str_ssid, String str_password); //connects to an external network
  bool disconnect_wifi(); //disconnects from a network
  bool state_wifi();
  bool switchOnAP(); //activates the local network
  bool switchOffAP(); //disables the local network
  bool CaptivePortalStart(); //activates the captive portal
  bool CaptivePortalStop();  //disables the captive portal
  bool loop(); //this only checks the  next request with the DNSserver(this allows that the captive portal working)
  //********************************************STATIC MEMBERS**********************************************************
  static const String s_class_name;
  static const String class_context;

};
#endif

