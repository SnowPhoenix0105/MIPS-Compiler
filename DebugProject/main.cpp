
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"
#include "../Develop/compile_controller.h"

# define DEBUG_002


int main(int argc, char* argv[])
{
#ifdef DEBUG_001

	string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\Test\\TestResource");
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

#endif // DEBUG_002

}