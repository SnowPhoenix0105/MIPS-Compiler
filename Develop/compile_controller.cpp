#include "global_control.h"
#include "compile_controller.h"
#include "ReplaceByMov.h"
#include "OptimizerFormat.h"
#include "OptimizerRemoveEmptyFunction.h"
#include "OptimizerRemoveNearbyCopy.h"
#include "OptimizerCalculateStrengthReduction.h"
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

vector<shared_ptr<IOptimizer>> optimizers =
{
	make_shared<OptimizerCalculateStrengthReduction>(),
	make_shared<OptimizerRemoveEmptyFunction>(),
	make_shared<OptimizerFormat>(),
	make_shared<OptimizerRemoveNearbyCopy>(),
	make_shared<OptimizerFormat>()
};

void start_compile(unique_ptr<istream> input_file, ostream& output_file)
{
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	try
	{
		syntactic_analyzer.parse();
		shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
		shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();
#ifdef DEBUG_LEVEL
		std::cout << ir_table_ptr->to_string(*allocator_ptr) << endl;
#endif // DEBUG_LEVEL

#ifdef ENABLE_OPTIMIZE

		for (size_t i = 0; i != optimizers.size(); ++i)
		{
			ir_table_ptr = optimizers[i]->parse(*ir_table_ptr, allocator_ptr);
		}

		shared_ptr<IrTable> moved_ir = ReplaceByMov().parse(*ir_table_ptr, allocator_ptr);
		shared_ptr<IrTable> registered_ir = GCPRegisterAllocator(allocator_ptr, moved_ir).build();
		unique_ptr<ITargetCodeGenerator> target_code_generator(new GCPTargetGenerator(allocator_ptr, registered_ir));
		target_code_generator->translate(output_file);

#else
		unique_ptr<ITargetCodeGenerator> target_code_generator(new SimpleCodeGenerator(allocator_ptr, ir_table_ptr));
		target_code_generator->translate(output_file);

#endif // ENABLE_OPTIMIZE


	}
	catch (const std::exception& e)
	{
		// *output_file << "1 " << e.what() << endl;
	}
	catch (...)
	{

	}
}


void start_compile_without_optimization(unique_ptr<istream> input_file, ostream& output_file)
{
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	try
	{
		syntactic_analyzer.parse();
		shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
		shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();
#ifdef DEBUG_LEVEL
		std::cout << ir_table_ptr->to_string(*allocator_ptr) << endl;
#endif // DEBUG_LEVEL

		unique_ptr<ITargetCodeGenerator> target_code_generator(new SimpleCodeGenerator(allocator_ptr, ir_table_ptr));
		target_code_generator->translate(output_file);
	}
	catch (const std::exception& e)
	{
		// *output_file << "1 " << e.what() << endl;
	}
	catch (...)
	{

	}
}



void get_ir_and_target(unique_ptr<istream> input_file, ostream& ir_file, ostream& target_file)
{
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	try
	{
		syntactic_analyzer.parse();
		shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
		shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();


		/*unique_ptr<ITargetCodeGenerator> target_code_generator(new SimpleCodeGenerator(allocator_ptr, ir_table_ptr));
		target_code_generator->translate(target_file);*/
#ifdef ENABLE_OPTIMIZE

		ir_file << ir_table_ptr->to_string(*allocator_ptr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		for (size_t i = 0; i != optimizers.size(); ++i)
		{
			ir_table_ptr = optimizers[i]->parse(*ir_table_ptr, allocator_ptr);
			ir_file << ir_table_ptr->to_string(*allocator_ptr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		}


		shared_ptr<IrTable> moved_ir = ReplaceByMov().parse(*ir_table_ptr, allocator_ptr);
		ir_file << moved_ir->to_string(*allocator_ptr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		shared_ptr<IrTable> registered_ir = GCPRegisterAllocator(allocator_ptr, moved_ir).build();
		ir_file << registered_ir->to_string(*allocator_ptr) << endl;
		unique_ptr<ITargetCodeGenerator> target_code_generator(new GCPTargetGenerator(allocator_ptr, registered_ir));
		target_code_generator->translate(target_file);
#else
		ir_file << ir_table_ptr->to_string(*allocator_ptr) << endl;
		unique_ptr<ITargetCodeGenerator> target_code_generator(new SimpleCodeGenerator(allocator_ptr, ir_table_ptr));
		target_code_generator->translate(target_file);

#endif // DEABLE_OPTIMIZE

	}
	catch (const std::exception& e)
	{
		// *output_file << "1 " << e.what() << endl;
		std::cout << "WRONG" << e.what() << endl;
		throw;
	}
	catch (...)
	{
		throw;
	}
}

void get_ir_fmtir_target(unique_ptr<istream> input_file, ostream& ir_file, ostream& fmtir_file, ostream& target_file)
{
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	try
	{
		syntactic_analyzer.parse();
		shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
		shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();
		ir_file << ir_table_ptr->to_string(*allocator_ptr) << endl;


		/*unique_ptr<ITargetCodeGenerator> target_code_generator(new SimpleCodeGenerator(allocator_ptr, ir_table_ptr));
		target_code_generator->translate(target_file);*/

#ifdef ENABLE_OPTIMIZE

		ir_file << ir_table_ptr->to_string(*allocator_ptr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		for (size_t i = 0; i != optimizers.size(); ++i)
		{
			ir_table_ptr = optimizers[i]->parse(*ir_table_ptr, allocator_ptr);
			ir_file << ir_table_ptr->to_string(*allocator_ptr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		}


		shared_ptr<IrTable> moved_ir = ReplaceByMov().parse(*ir_table_ptr, allocator_ptr);
		ir_file << moved_ir->to_string(*allocator_ptr) << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		shared_ptr<IrTable> registered_ir = GCPRegisterAllocator(allocator_ptr, moved_ir).build();
		fmtir_file << registered_ir->to_string(*allocator_ptr) << endl;

		unique_ptr<ITargetCodeGenerator> target_code_generator(new GCPTargetGenerator(allocator_ptr, registered_ir));
		target_code_generator->translate(target_file);

#else
		unique_ptr<ITargetCodeGenerator> target_code_generator(new SimpleCodeGenerator(allocator_ptr, ir_table_ptr));
		target_code_generator->translate(target_file);
#endif // ENABLE_OPTIMIZE

	}
	catch (const std::exception& e)
	{
		// *output_file << "1 " << e.what() << endl;
		std::cout << "WRONG" << e.what() << endl;
		throw;
	}
	catch (...)
	{
		throw;
	}
}

string formated_content(LexicalAnalyzer& analyzer)
{
	string content;
	shared_ptr<const string> origin = analyzer.get_content();
	switch (analyzer.get_symbol())
	{
	case SymbolType::character:
		content = (*origin)[1];
		break;
	case SymbolType::string:
		content = origin->substr(1, origin->size() - 2);
		break;
	default:
		content = *origin;
	}
	return content;
}

unique_ptr<std::ostream> lexical_analyze(unique_ptr<istream> input_file, unique_ptr<ostream> output_file)
{
	LexicalAnalyzer analyzer(std::move(input_file));
	while (analyzer.has_next())
	{
		//SymbolType sym = analyzer.next();
		//*output_file << symboltype_output_dictionary.at(sym)
		//	<< ' '
		//	<< formated_content(analyzer)
		//	<< endl;
		analyzer.next();
		*output_file << analyzer.get_token()->to_print_string() << endl;
	}
	return output_file;
}

std::unique_ptr<std::ostream> syntactic_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file)
{
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	syntactic_analyzer.parse();
	syntactic_analyzer.print_all_message(*output_file);
	return output_file;
}

std::unique_ptr<std::ostream> error_analyze(std::unique_ptr<std::istream> input_file, std::unique_ptr<std::ostream> output_file)
{
	unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
	SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
	try
	{
		syntactic_analyzer.parse();
		syntactic_analyzer.print_all_error(*output_file);
	}
	catch (const std::exception& e)
	{
		*output_file << "1 " << e.what() << endl;
	}
	catch (...)
	{

	}
	return output_file;
}


