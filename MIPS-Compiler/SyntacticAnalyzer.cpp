#include "SyntacticAnalyzer.h"

void SyntacticAnalyzer::parse()
{
	ProgramAnalyze()(env);
}

void SyntacticAnalyzer::print_all(ostream& os)
{
	env.print_all(os);
}
