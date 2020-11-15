#include "SyntacticAnalyzer.h"
#include "TargetCodeGenerator.h"
#include <unordered_set>

using std::unordered_set;
using std::swap;

void SyntacticAnalyzer::parse()
{
	try
	{
		ProgramAnalyze()(env);
	}
	catch (const reach_end_exception&) { }
}

void SyntacticAnalyzer::print_all_message(ostream& os)
{
	env.print_all_message(os);
}

void SyntacticAnalyzer::print_all_error(ostream& os)
{
	env.print_all_error(os);
}

shared_ptr<IrTable> SyntacticAnalyzer::get_ir_table()
{
	IrTableBuilder& builder = env.code_builder();
	unordered_set<IrHead> end_set = { IrHead::arr, IrHead::init, IrHead::gvar, IrHead::param, IrHead::func };
	for (int i = 0; i < builder.size(); ++i)
	{
		if (builder[i].head == IrHead::arr ||
			builder[i].head == IrHead::init ||
			builder[i].head == IrHead::label && IrType::is_mid(builder[i].elem[0]) && IrType::is_func(builder[i].elem[0]))
		{
			for (int j = i - 1; j >= 0; --j)
			{
				if (end_set.count(builder[j].head) != 0) break;
				// if (builder[j].head == IrHead::label && IrType::is_func(builder[i].elem[0]) && IrType::is_beg(builder[i].elem[0])) break;
				swap(builder[j], builder[j + 1]);
			}
		}
	}
	return builder.build();
}