#include "SyntacticAnalyzer.h"

void SyntacticAnalyzer::parse()
{
	ProgramAnalyze()(env);
}

void SyntacticAnalyzer::print_all_message(ostream& os)
{
	env.print_all_message(os);
}

void SyntacticAnalyzer::print_all_error(ostream& os)
{
	env.print_all_error(os);
}
