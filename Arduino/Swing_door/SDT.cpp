#include"SDT.h"

const String SDT::class_name = "Swinging Door";
const String SDT::class_context = "Swing Door::";

void SDT::first(double value)
{
	max_slope_upper = SLOPE_OF_UPPER_DOOR; 	//Step 4
	min_slope_lower = SLOPE_OF_LOWER_DOOR; 	//Step 4
#ifdef SHOW_SDT_INFORMATION_PROCESS
	String method_name = "CHECK";
	String string_helper = "";
	string_helper += "MAX: ";
	string_helper += max_slope_upper;
	string_helper += " MIN: ";
	string_helper += min_slope_lower;
	console->publish(class_context + method_name, string_helper, INF_LOG);
#endif
#ifdef VARIABLE_ERROR_COMPRESSION 
	MAX = { time_line,value };
	MIN = { time_line,value };
#endif
	last_point = { time_line,value };
}

SDT::SDT(Console_manager *t_console)
{
	this->console = t_console;

}

SDT_I SDT::begin(double t_error, double t_maximum_time, double t_minimum_time, double t_value)
{  

	//STEP 1 and 2 of the Swinging door algorithm
    #ifdef SDT_PORCENTUAL_ERROR
		if (t_error > 100 && t_error >= 0) error = t_error;
		else return EXE_ERROR;
    #else
		error = t_error;
    #endif
	init = true;
	maximum_time = t_maximum_time;
	minimum_time = t_minimum_time;
	swing_value = {0,t_value};
	time_line = 0;
	first_time = true;
	return {SDT_VALUE,t_value,t_error};
}

SDT_I SDT::check(double value) //step 3
{
	time_line++;
	double temporal;
	if (first_time)
	{	
		first(value);
		first_time = false;
		return { SDT_OK, 0,error};
	}
	else
	{
		temporal = SLOPE_OF_UPPER_DOOR; 	//Step 4
		if (temporal > max_slope_upper)     //step 5
		{
			max_slope_upper = temporal;
			#ifdef VARIABLE_ERROR_COMPRESSION 
				MAX = { time_line,value };
			#endif
			if (max_slope_upper > min_slope_lower)
			{
				#ifdef SHOW_SDT_INFORMATION_PROCESS
				String method_name = "CHECK";
				String string_helper = "";
				string_helper = "";
				string_helper += "MAX: ";
				string_helper += max_slope_upper;
				string_helper += " MIN: ";
				string_helper += min_slope_lower;
				string_helper += " SO: ";
				string_helper += OUT_SLOPE;
				string_helper += " CX(I+1): ";
				string_helper += UPPER_TIME_OUT_SLOPE;
				string_helper += " OY ";
				string_helper += UPPER_OUT_Y;
				string_helper += '\n';
				string_helper += "UPPER_TIME_OUT_SLOPE FORMULA: ";
				string_helper += '\n';
				string_helper += "("; 
									string_helper += UPPER_PIVOT; 
									string_helper += "-";
									string_helper += last_point.Y;
									string_helper += "+";
									string_helper += OUT_SLOPE;
									string_helper += "X";
									string_helper += last_point.X;
									string_helper += "-";
									string_helper += min_slope_lower;
									string_helper += "X";
									string_helper += swing_value.X;
				string_helper += ")/(";
									string_helper += OUT_SLOPE;
									string_helper += "-";
									string_helper += min_slope_lower;
				string_helper += ")";
				console->publish(class_context + method_name, string_helper, INF_LOG);
				#endif
				#ifdef VARIABLE_ERROR_COMPRESSION 
					if (time_line < minimum_time) error = NEW_ERROR;
					return { SDT_OK, 0,error };
				#endif
				SDT_coordinates coordinates = {UPPER_TIME_OUT_SLOPE,UPPER_NEW_VALUE};
				swing_value = coordinates;
				first(value);
				return { SDT_VALUE,coordinates.Y,error };
			}
		}
		temporal = SLOPE_OF_LOWER_DOOR; 	//Step 4
		if (temporal< min_slope_lower)      //step 5
		{
			min_slope_lower = temporal;
			#ifdef VARIABLE_ERROR_COMPRESSION 
			MIN = { time_line,value };
            #endif
			if (max_slope_upper > min_slope_lower)
			{ 
				#ifdef SHOW_SDT_INFORMATION_PROCESS
				String method_name = "CHECK";
				String string_helper = "";
				string_helper = "";
				string_helper += "MAX: ";
				string_helper += max_slope_upper;
				string_helper += " MIN: ";
				string_helper += min_slope_lower;
				string_helper += " SO: ";
				string_helper += OUT_SLOPE;
				string_helper += " CX(I+1): ";
				string_helper += LOWER_TIME_OUT_SLOPE;
				string_helper += " OY ";
				string_helper += LOWER_OUT_Y;
				string_helper += '\n';
				string_helper += "LOWER_TIME_OUT_SLOPE FORMULA: ";
				string_helper += '\n';
				string_helper += "(";
				string_helper += LOWER_PIVOT;
				string_helper += "-";
				string_helper += last_point.Y;
				string_helper += "+";
				string_helper += OUT_SLOPE;
				string_helper += "X";
				string_helper += last_point.X;
				string_helper += "-";
				string_helper += max_slope_upper;
				string_helper += "X";
				string_helper += swing_value.X;
				string_helper += ")/(";
				string_helper += OUT_SLOPE;
				string_helper += "-";
				string_helper += max_slope_upper;
				string_helper += ")";
				console->publish(class_context + method_name, string_helper, INF_LOG);
                #endif
				#ifdef VARIABLE_ERROR_COMPRESSION 
					if (time_line < minimum_time) error = NEW_ERROR;
					return { SDT_OK, 0,error };
				#endif
				SDT_coordinates coordinates = {LOWER_TIME_OUT_SLOPE,LOWER_NEW_VALUE };
				swing_value = coordinates;
				first(value);
				return { SDT_VALUE,coordinates.Y,error };
			}
		}

	}
	#ifdef SHOW_SDT_INFORMATION_PROCESS
	String method_name = "CHECK";
	String string_helper = "";
	string_helper = "";
	string_helper += "MAX: ";
	string_helper += max_slope_upper;
	string_helper += " MIN: ";
	string_helper += min_slope_lower;
	console->publish(class_context + method_name, string_helper, INF_LOG);
	#endif
	last_point = {time_line,value};
	return { SDT_OK, 0,error};
}
