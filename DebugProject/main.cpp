
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"
#include "../Develop/compile_controller.h"

# define DEBUG_001


int main(int argc, char* argv[])
{
#ifdef DEBUG_001

	string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\Test\\TestResource");
	string source_file = test_resource_path + "\\sample1\\nospace_source.c";
	unique_ptr<istream> input_stream(new std::istringstream(source_file));
	unique_ptr<ostringstream> output_ostream(new std::ostringstream());
	output_ostream.reset(
		dynamic_cast<ostringstream*>(
			syntactic_analyze(std::move(input_stream), std::move(output_ostream)).release()
			)
	);

#endif // DEBUG_001

}