#include <iostream>
#include <fstream>
#include <memory>

#include "global_control.h"
#include "compile_controller.h"

using std::ifstream;
using std::ofstream;
using std::unique_ptr;
using std::istream;
using std::ostream;


int main(int argc, char* argv[])
{
#ifdef HW_1
	unique_ptr<istream> input_file(new ifstream("testfile.txt"));
	unique_ptr<ostream> output_file(new ofstream("output.txt"));
	lexical_analyze(std::move(input_file), std::move(output_file));
#endif // HW_1

#ifdef HW_2
	unique_ptr<istream> input_file(new ifstream("testfile.txt"));
	unique_ptr<ostream> output_file(new ofstream("output.txt"));
	syntactic_analyze(std::move(input_file), std::move(output_file));
#endif // HW_2

#ifdef HW_3
	unique_ptr<istream> input_file(new ifstream("testfile.txt"));
	unique_ptr<ostream> output_file(new ofstream("error.txt"));
	error_analyze(std::move(input_file), std::move(output_file));
#endif // HW_2

#ifdef HW_4
	unique_ptr<istream> input_file(new ifstream("testfile.txt"));
	ofstream output_file("mips.txt");
	start_compile(std::move(input_file), output_file);

#endif // HW_4


	return 0;
}