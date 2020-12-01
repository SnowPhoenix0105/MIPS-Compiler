
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"
#include "../Develop/compile_controller.h"
#include "../Develop/GCPTargetGenerator.h"
#include "../Develop/OptimizerFormat.h"

#define DEBUG_006

using std::ostringstream;
using std::ofstream;
using std::ifstream;


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


#ifdef DEBUG_003
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


#endif // DEBUG_003


#ifdef DEBUG_004
	string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
	string source_file = test_resource_path + "\\blackbox_simple_test\\sample4\\source.c";
	std::ifstream input_stream_0(source_file);
	string line;
	while (std::getline(input_stream_0, line))
	{
		std::cout << line << endl;
	}
	unique_ptr<istream> input_stream(new std::ifstream(source_file));
	ostringstream target_ostream;
	ofstream ofs(test_resource_path + "\\blackbox_simple_test\\sample4\\mem");
	auto save = std::cout.rdbuf();
	std::cout.rdbuf(ofs.rdbuf());

	get_ir_and_target(std::move(input_stream), std::cout, target_ostream);

	std::cout.rdbuf(save);

	// std::cout << target_ostream.str() << std::endl;


#endif // DEBUG_004

#ifdef DEBUG_005

	unique_ptr<istream> input_file(new ifstream("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug\\source.c"));
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	syntactic_analyzer.parse();
	ostringstream oss;
	syntactic_analyzer.print_all_error(oss);
	string err_msg = oss.str();
	if (err_msg.size() != 0)
	{
		std::cout << err_msg << std::endl;
		return 0;
	}
	shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
	shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();
	ofstream origin_ir_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug\\origin.ir");
	ofstream transed_ir_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug\\transed.ir");
	origin_ir_os << ir_table_ptr->to_string(*allocator_ptr);
	GCPRegisterAllocator gcp_registre_allocator(allocator_ptr, ir_table_ptr);
	shared_ptr<IrTable> transed_table = gcp_registre_allocator.build();
	transed_ir_os << transed_table->to_string(*allocator_ptr);

#endif // DEBUG_005


#ifdef DEBUG_006
	unique_ptr<istream> input_file(new ifstream("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug2\\source.c"));
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	syntactic_analyzer.parse();
	ostringstream oss;
	syntactic_analyzer.print_all_error(oss);
	string err_msg = oss.str();
	if (err_msg.size() != 0)
	{
		std::cout << err_msg << std::endl;
		return 0;
	}
	shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
	shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();

	ofstream origin_ir_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug2\\origin.ir");
	ofstream format_ir_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug2\\formatted.ir");
	ofstream transed_ir_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug2\\transed.ir");
	origin_ir_os << ir_table_ptr->to_string(*allocator_ptr);

	shared_ptr<IrTable> formatted_ir_table = OptimizerFormat().parse(*ir_table_ptr, allocator_ptr);

	format_ir_os << formatted_ir_table->to_string(*allocator_ptr);

	GCPRegisterAllocator gcp_registre_allocator(allocator_ptr, formatted_ir_table);
	shared_ptr<IrTable> transed_table = gcp_registre_allocator.build();
	transed_ir_os << transed_table->to_string(*allocator_ptr);

	ofstream simple_asm_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug2\\simple.asm");
	ofstream gcp_asm_os("D:\\Projects\\C++\\MIPS-Compiler\\DebugProject\\Resource\\GCPDebug2\\gcp.asm");

	SimpleCodeGenerator simple_generator(allocator_ptr, ir_table_ptr);
	GCPTargetGenerator gcp_generator(allocator_ptr, transed_table);

	simple_generator.translate(simple_asm_os);
	gcp_generator.translate(gcp_asm_os);
#endif // DEBUG_006
}