#include "SyntacticAnalyzer.h"

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
