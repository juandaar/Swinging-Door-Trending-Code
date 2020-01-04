// Console_Manager.h

#ifndef _CONSOLE_MANAGER_h
#define _CONSOLE_MANAGER_h


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"

#endif
/*
#ifndef  CONSOLE_STREAMER_BAUDRATE
  #define CONSOLE_STREAMER_BAUDRATE 115200
#endif // !#define CONSOLE_STREAMER_BAUDRATE
*/
//*******************************************CONDITIONAL DEFINITIONS - DEFINITIONS**********************************//
#define SHOW_CONSOLE_MESSAGES
#define SHOW_ERR_MESSAGES
#define SHOW_INF_MESSAGES
#define SHOW_VER_MESSAGES
#define SHOW_WAR_MESSAGES
/// <summary>
/// Enum with the different types of messages that can be sent through of a Console_Manager object to the m_console
/// </summary>
/// ----------------------------------------------------------------------------------------------
/// ************************************************TYPES DEFINITION**************************************************//
typedef enum MsgLogTypes {
  /// <summary>
  /// Used to send an infomative message
  /// </summary>
  INF_LOG,
  /// <summary>
  /// Used to send an error message
  /// </summary>
  ERR_LOG,
  /// <summary>
  /// The executable log
  /// </summary>
  EXE_LOG,
  /// <summary>
  /// Used to send a verbose message
  /// </summary>
  VER_LOG,
  /// <summary>
  /// Used to send a warning message
  /// </summary>
  WAR_LOG
} MsgLogType;

/// <summary>
/// Console_msg_manager  are objects that allow to implement a m_console log using a serial port as streamer
/// </summary>
class Console_manager {
 private:
  /// <summary>
  /// Reference to serial or software serial object
  /// </summary>
  Stream  &m_stream;
  /// <summary>
  /// Name that is showed in the m_console when a message log is sent. Default value: "Console"
  /// </summary>
  String m_console_name;



 protected:

 public:
  /// <summary>
  /// Initializes a new instance of the Console_msg_managerClass.
  /// </summary>
  /// <param name="stream">The stream.</param>
  Console_manager(Stream &stream);
  /// <summary>
  /// Initializes the specified m_console name.
  /// </summary>
  /// <param name="console_name">Name of the m_console.</param>
  void init(String console_name);
  /// <summary>
  /// Publishes a message in the m_console log
  /// </summary>
  /// <param name="str_context">Specify the context from is sent the message</param>
  /// <param name="str_message">Message.</param>
  /// <param name="type_message">Type message.</param>
  void publish(String str_context, String str_message, MsgLogType type_message);
};
#endif

extern Console_manager console;


