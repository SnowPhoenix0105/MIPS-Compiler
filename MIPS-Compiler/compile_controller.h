#ifndef __COMPILE_CONTROLLER_H__
#define __COMPILE_CONTROLLER_H__


#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"
#include "TargetCodeGenerator.h"
#include "SimpleTargetGenerator.h"




unique_ptr<ostream> start_compile(unique_ptr<istream> input_file, unique_ptr<ostream> output_file);

string formated_content(LexicalAnalyzer& analyzer);

std::unique_ptr<std::ostream> lexical_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

std::unique_ptr<std::ostream> syntactic_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

std::unique_ptr<std::ostream> error_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

#endif // !__COMPILE_CONTROLLER_H__


