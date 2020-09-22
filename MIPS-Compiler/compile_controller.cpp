#include "global_control.h"
#include "compile_controller.h"
#include <iostream>
#include <memory>

using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::cout;
using std::cin;
using std::endl;
using std::istream;
using std::ostream;

void start_compile(const std::string& input_file_name, const std::string& output_file_name)
{
	// TODO: main process of compiling
	cout << "start compile" << endl;
}

void lexical_analyze(unique_ptr<istream> input_file, unique_ptr<ostream> output_file)
{
	LexicalAnalyzer analyzer(std::move(input_file));
	while (analyzer.has_next())
	{
		*output_file << symboltype_output_dictionary.at(analyzer.next())
			<< ' '
			<< analyzer.get_content()
			<< endl;
	}
}

