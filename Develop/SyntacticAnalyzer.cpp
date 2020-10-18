#include "SyntacticAnalyzer.h"

void SyntacticAnalyzer::parse()
{
	ProgramAnalyze()(env);
}


void SyntacticAnalyzer::print_all()
{
	env.print_all(*os);
}