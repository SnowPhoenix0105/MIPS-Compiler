#pragma once
#ifndef __COMPILE_CONTROLLER_H__
#define __COMPILE_CONTROLLER_H__


#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"
#include "TargetCodeGenerator.h"
#include "SimpleTargetGenerator.h"
#include "GCPTargetGenerator.h"
#include "OptimizerFormat.h"




void start_compile(unique_ptr<istream> input_file, ostream& output_file);

void start_compile_without_optimization(unique_ptr<istream> input_file, ostream& output_file);

void get_ir_and_target(unique_ptr<istream> input_file, ostream& ir_file, ostream& target_file);

void get_ir_fmtir_target(unique_ptr<istream> input_file, ostream& ir_file, ostream& fmtir_file, ostream& target_file);

string formated_content(LexicalAnalyzer& analyzer);

std::unique_ptr<std::ostream> lexical_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

std::unique_ptr<std::ostream> syntactic_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

std::unique_ptr<std::ostream> error_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file);

#endif // !__COMPILE_CONTROLLER_H__


