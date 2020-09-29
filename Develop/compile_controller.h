#pragma once
#ifndef __COMPILE_CONTROLLER_H__
#define __COMPILE_CONTROLLER_H__


#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"




void start_compile(const std::string& input_file_name, const std::string& output_file_name);

string formated_content(LexicalAnalyzer& analyzer);

std::unique_ptr<std::ostream> lexical_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

#endif // !__COMPILE_CONTROLLER_H__


