// 
// 
// 

#include "SD_algorithm.h"

const String SD_algorithm::class_name = "Swing Door Algorithm";
const String SD_algorithm::class_context = "Swing Door Algorithm::";


SD_algorithm::SD_algorithm( Console_manager *t_console) {
	this->console = t_console;

}
ExecutionState SD_algorithm::begin(float range_value, int time,float value)
{
	const String method_name = "begin";
	current_value = 0;
	range = range_value;
	discrete_time = time;
	current_time = 0;
	first_time = 1;
	range_way = INTEGER;
	current_value = value;
	console->publish(class_context+method_name,"The swing door was set successfully",INF_LOG);
	return EXE_OK;
}
/*
SD_algorithm::SD_algorithm(float range_value, range_type tipe, int time, Console_manager * t_console)
{
}*/

ExecutionState SD_algorithm::check(float value) {
	const String method_name = "check";
	if (range_way == INTEGER) {


			current_time++;
			if (value>(current_value + range) || value<(current_value - range)) {
				current_time = 0;
				current_value = value;
				return EXE_OK;
			}
			if (current_time >= discrete_time) {
				current_time = 0;
				current_value = value;
				return EXE_OK;
			}
			return EXE_WAIT;
		

	}
	else  console->publish(class_context + method_name, "This swing door algorith Was not created yet\n", ERR_LOG);
	return EXE_ERROR;
}

ExecutionState SD_algorithm::synchronization(float value) {
	current_value = value;
	current_time = 0;
	return EXE_OK;
}
