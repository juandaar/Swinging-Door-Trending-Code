/*
Santa Catarina Federal University  "Universidad Federal de santa catarina " UFSC

April 2018 - Semester 01 and bimester 01.

Curse:
Técnicas de Implementação de sistemas Automatizados.

Program:
Mestrado em Engenharia de Automação e systemas

Teacher:
Alex pinto.

Creating for:
Juan David Arias Correa

*/



#ifndef _SDT_h
#define _SDT_h

#include "Settings.h"
#include  "Execution_state.h"
#include "Console_manager.h"
//#include <stdio.h>
#if  !defined(SDT_PORCENTUAL_ERROR) && !defined(SDT_VALUE_ERROR)
#define SDT_VALUE_ERROR
#endif
#define SCALE 100

#ifdef  SDT_PORCENTUAL_ERROR
#define UPPER_PIVOT (swing_value.Y*(1+error/100))
#define LOWER_PIVOT (swing_value.Y*(1-error/100))
#define UPPER_NEW_VALUE (UPPER_OUT_Y*(1-error/200))
#define LOWER_NEW_VALUE (LOWER_OUT_Y*(1+error/200))
#endif

#ifdef SDT_VALUE_ERROR
#define UPPER_PIVOT (swing_value.Y+error)
#define LOWER_PIVOT (swing_value.Y-error)
#define UPPER_NEW_VALUE (UPPER_OUT_Y-(error/2))
#define LOWER_NEW_VALUE (LOWER_OUT_Y+(error/2))
#endif


#define SLOPE_OF_UPPER_DOOR ((value-UPPER_PIVOT)/(time_line-swing_value.X))
#define SLOPE_OF_LOWER_DOOR ((value-LOWER_PIVOT)/(time_line-swing_value.X))
#define OUT_SLOPE           ((value-last_point.Y)/(time_line-last_point.X))



//#define UPPER_TIME_OUT_SLOPE  ((UPPER_PIVOT-last_point.Y-min_slope_lower*last_point.X+OUT_SLOPE*swing_value.X)/(OUT_SLOPE-min_slope_lower))
//#define LOWER_TIME_OUT_SLOPE ((LOWER_PIVOT-last_point.Y-max_slope_upper*last_point.X+OUT_SLOPE*swing_value.X)/(OUT_SLOPE-max_slope_upper))
//#define UPPER_TIME_OUT_SLOPE ((UPPER_PIVOT-last_point.Y-(min_slope_lower*last_point.X)+(OUT_SLOPE*swing_value.X)) /(OUT_SLOPE-min_slope_lower))
//#define LOWER_TIME_OUT_SLOPE ((LOWER_PIVOT-last_point.Y-(max_slope_upper*last_point.X)+(OUT_SLOPE*swing_value.X))/(OUT_SLOPE-max_slope_upper) )

#define UPPER_TIME_OUT_SLOPE ((UPPER_PIVOT-last_point.Y+(OUT_SLOPE*last_point.X)-(min_slope_lower*swing_value.X)) /(OUT_SLOPE-min_slope_lower))
#define LOWER_TIME_OUT_SLOPE ((LOWER_PIVOT-last_point.Y+(OUT_SLOPE*last_point.X)-(max_slope_upper*swing_value.X))/(OUT_SLOPE-max_slope_upper) )
#define UPPER_OUT_Y (UPPER_PIVOT+min_slope_lower*(UPPER_TIME_OUT_SLOPE-swing_value.X))
#define LOWER_OUT_Y (LOWER_PIVOT+max_slope_upper*(LOWER_TIME_OUT_SLOPE-swing_value.X))
//#define SHOW_SDT_INFORMATION_PROCESS
//#define VARIABLE_ERROR_COMPRESSION
#define EQUAL_SLOPE ((MIN.Y+MAX.Y-2*swing_value.Y)/(MIN.X+MAX.X-2*swing_value.X))
#define NEW_ERROR  ((abs(MAX.Y-MIN.Y)-EQUAL_SLOPE*(MAX.X-MIN.X))/2)



typedef enum SDT_state
{
	SDT_ERROR,
	SDT_OK,
	SDT_VALUE
}SDT_state;

//Sctruct that returns the SDT conclusion and variable (when it necessary to send information to storage in the databases)
struct  SDT_I
{
	SDT_state state;
	double value ;
	double error;
};



struct SDT_coordinates
{
	double X;
	double Y;
};


class SDT
{
private:
	Console_manager * console;
	bool porcentual; //if it is true, the error is taken from a percentage of the first swinging door value
	double error; //if porcentual is false this is a specific value if not is a percentage value
	bool init;
	bool first_time;
	double time_line;
	double minimum_time;
	double maximum_time; //the maximum windows size
	SDT_coordinates swing_value;
	SDT_coordinates last_point;
    //possivel changes
	double max_slope_upper;
	SDT_coordinates MAX;
	double min_slope_lower;
	SDT_coordinates MIN;
	void first(double value);
public:
	static const String class_name;
	static const String class_context;
	SDT(Console_manager *t_console);
	SDT_I begin(double t_error,double t_maximum_time,double t_minimum_time, double t_value);
	SDT_I check(double value); //is used to check a external value with the present "door"
	//ExecutionState synchronization(float value);
};


#endif

