//
//
//

#include "Console_manager.h"

Console_manager::Console_manager(Stream &stream) : m_stream(stream) {
}


void Console_manager::init(String console_name = "Console") {
  m_console_name = console_name;
}



void Console_manager::publish(String str_context, String str_message, MsgLogType type_message) {
  #if  defined(SHOW_CONSOLE_MESSAGES) & (defined(SHOW_ERR_MESSAGES) || defined(SHOW_INF_MESSAGES) || defined(SHOW_VER_MESSAGES) || defined(SHOW_WAR_MESSAGES))
  String aux_string = "";
  switch (type_message) {
    case INF_LOG:
      #ifdef SHOW_INF_MESSAGES
      aux_string = m_console_name + " >> " + str_context + " >> INF >> " + str_message;
      #endif // SHOW_ERR_MESSAGES
      break;
    case ERR_LOG:
      #ifdef SHOW_ERR_MESSAGES
      aux_string = m_console_name + " >> " + str_context + " >> ERR >> " + str_message;
      #endif	// SHOW_ERR_MESSAGES
      break;
    case VER_LOG:
      #ifdef SHOW_VER_MESSAGES
      aux_string = m_console_name + " >> " + str_context + " >> VER >> " + str_message;
      #endif	//SHOW_VER_MESSAGES
      break;
    case EXE_LOG:
      #ifdef SHOW_VER_MESSAGES
      aux_string = m_console_name + " >> " + str_context + " >> EXE >> " + str_message;
      #endif	//SHOW_VER_MESSAGES
      break;
    case WAR_LOG:
      #ifdef SHOW_WAR_MESSAGES
      aux_string = m_console_name + " >> " + str_context + " >> WAR >> " + str_message;
      //EMIT_WARNING_LOOP_LIGHT
      #endif	//SHOW_WAR_MESSAGES
      break;
    default:
      aux_string = m_console_name + " >> helo_message  >> ERR >> No valid type message";
      break;
  }
  m_stream.println(aux_string);
  #endif
}

//Console_Manager m_console;

