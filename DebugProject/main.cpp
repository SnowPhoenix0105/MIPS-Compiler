
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"
#include "../Develop/compile_controller.h"

# define DEBUG_003

using std::ostringstream;


int main(int argc, char* argv[])
{
#ifdef DEBUG_001

	string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
	string source_file = test_resource_path + "\\sample1\\source.c";
	std::ifstream input_stream_0(source_file);
	string line;
	while (std::getline(input_stream_0, line))
	{
		std::cout << line << endl;
	}
	unique_ptr<istream> input_stream(new std::ifstream(source_file));
	unique_ptr<ostringstream> output_ostream(new std::ostringstream());

	syntactic_analyze(std::move(input_stream), std::move(output_ostream));

#endif // DEBUG_001

#ifdef DEBUG_002

	string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
	string source_file = test_resource_path + "\\error_dealing\\source9.c";
	std::ifstream input_stream_0(source_file);
	string line;
	while (std::getline(input_stream_0, line))
	{
		std::cout << line << endl;
	}
	unique_ptr<istream> input_stream(new std::ifstream(source_file));
	unique_ptr<ostringstream> output_ostream(new std::ostringstream());

	error_analyze(std::move(input_stream), std::move(output_ostream));

#endif // DEBUG_002
	string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
	string source_file = test_resource_path + "\\blackbox_simple_test\\sample1\\source.c";
	std::ifstream input_stream_0(source_file);
	string line;
	while (std::getline(input_stream_0, line))
	{
		std::cout << line << endl;
	}
	unique_ptr<istream> input_stream(new std::ifstream(source_file));
	ostringstream target_ostream;

	get_ir_and_target(std::move(input_stream), std::cout, target_ostream);

	std::cout << target_ostream.str() << std::endl;


#ifdef DEBUG_003


#endif // DEBUG_003


}