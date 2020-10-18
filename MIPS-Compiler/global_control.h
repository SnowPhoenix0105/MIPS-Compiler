#ifndef __GLOBAL_CONTROL_H__
#define __GLOBAL_CONTROL_H__





// global defines /////////////////////////////////////////////////////////////////////
#ifndef __DELETE_ALL_GLOBAL_MACROS__


#define HW_2

// undefine DEBUG_LEVEL before release
// #define DEBUG_LEVEL 10



#endif // !__DELETE_ALL_GLOBAL_MACROS__
/////////////////////////////////////////////////////////////////////////////////////


















// macros for logging debug information /////////////////////////////////////////////////
#ifdef DEBUG_LEVEL



#include <iostream>

// @param required_level: if DEBUG_LEVEL < required_level, no message will be print;
// @param operations: some statements, divided with ';'
#define DEBUG_DO(required_level, operations) do			\
{														\
	if (DEBUG_LEVEL >= (required_level))				\
	{													\
		operations;										\
	}													\
} while(false)

// @param required_level: if DEBUG_LEVEL < required_level, no message will be print;
// @param name: the name of the value that need to print
// @param val: the value that need to print
#define DEBUG_LOG_VAL(required_level, name, val) DEBUG_DO((required_level),		\
	std::cout << "[DEBUG_LOG]\t@" __FILE__ ":" << __LINE__ << '\t'				\
	<< (name) << " = " << (val) << std::endl;		)

// @param required_level:if DEBUG_LEVEL < required_level, no message will be print;
// @param var: a named variable that need to be print
#define DEBUG_LOG_VAR(required_level, var) DEBUG_LOG_VAL((required_level), #var, (var))


#define DEBUG_LOG_MSG(required, msg) DEBUG_DO((required_level),				\
	std::cout << "[DEBUG_LOG]\t@" __FILE__ ":" << __LINE__ << '\t'			\
	<< (msg) << std::end;													\
		)


#else



#define DEBUG_DO(required_level, operate) ;
#define DEBUG_LOG_VAL(required_level, name, var) ;
#define DEBUG_LOG_VAR(required_level, var) ;
#define DEBUG_LOG_MSG(required, name, msg) ;
#define PANIC() ;


#endif // DEBUG_LEVEL
/////////////////////////////////////////////////////////////////////////////////////////


























#endif // !__GLOBAL_CONTROL_H__
