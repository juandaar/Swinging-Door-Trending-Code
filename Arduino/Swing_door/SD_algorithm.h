





/*
Santa Catarina Federal University  "Universidad Federal de santa catarina " UFSC

April 2018 - Semester 01 and bimester 01.

Curse:
Técnicas de Implementação de sistemas Automatizados.

Program:
Mestrado em Engenharia de Automação e systemas

Teacher:
Dr. Carlos Barros Montez

Creating for:
Juan David Arias Correa

*/



#ifndef _SWING_h
#define _SWING_h

#include "Settings.h"
#include  "Execution_state.h"
#include "Console_manager.h"
//#include <stdio.h>


typedef enum range_type {
	INTEGER,
	PORCENTUAL
}range_type;




class SD_algorithm {
private:
	Console_manager * console;
	int current_value; //current value of the swing door
	int range; //range current_value +-range
	int discrete_time; //how many times the swing need to be used for accept a new value
	int current_time;  //current time, current time <discrete time
	int first_time; //has a diferent behavior when receive a first value;
	range_type range_way;
public:
	static const String class_name;
	static const String class_context;
	SD_algorithm( Console_manager *t_console);
	ExecutionState begin(float range_value, int time,float value);
	//SD_algorithm(float range_value, range_type tipe, int time,Console_manager *t_console);
	ExecutionState check(float value); //is used to check a external value with the present "door"
	ExecutionState synchronization(float value);
};


#endif

