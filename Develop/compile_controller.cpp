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

unique_ptr<std::ostream> lexical_analyze(unique_ptr<istream> input_file, unique_ptr<ostream> output_file)
{
	LexicalAnalyzer analyzer(std::move(input_file));
	while (analyzer.has_next())
	{
		SymbolType sym = analyzer.next();
		string content;
		const string& origin = analyzer.get_content();
		switch (sym)
		{
		case SymbolType::character:
			content = origin[1];
			break;
		case SymbolType::string:
			content = origin.substr(1, origin.size() - 1);
			break;
		default:
			content = origin;
		}
		*output_file << symboltype_output_dictionary.at(sym)
			<< ' '
			<< content
			<< endl;
	}
	return output_file;
}

