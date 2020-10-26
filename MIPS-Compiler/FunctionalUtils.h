

#ifndef __FUNCTIONAL_UTILS_H__
#define __FUNCTIONAL_UTILS_H__


#include <functional>
#include <unordered_set>
#include "global_control.h"
#include "SyntacticAnalyzeUtil.h"
#include "SymbolType.h"

using std::unordered_set;

struct IsType
{
	IsType(SymbolType type) : type(type) { }
	bool operator()(TokenEnvironment& env) const
	{
		return env.peek() == type;
	}
private:
	SymbolType type;
};

struct TypeInsideSet
{
	template<typename T>
	TypeInsideSet(T&& set) : type_set(std::forward(set)) { }
	bool operator()(TokenEnvironment& env) const
	{
		return type_set.count(env.peek()) != 0;
	}
private:
	unordered_set<SymbolType> type_set;
};

struct OrCondition
{
	OrCondition(const function<bool(TokenEnvironment&)>& c1, const function<bool(TokenEnvironment&)>& c2)
		: c1(c1), c2(c2)
	{ }
	bool operator()(TokenEnvironment& env) const
	{
		return c1(env) || c2(env);
	}
private:
	function<bool(TokenEnvironment&)> c1;
	function<bool(TokenEnvironment&)> c2;
};


inline function<bool(TokenEnvironment&)> wrap_condition(function<bool(TokenEnvironment&)> func)
{
	return func;
}

inline IsType wrap_condition(SymbolType type)
{
	return IsType(type);
}

inline TypeInsideSet wrap_condition(std::initializer_list<SymbolType> type_set)
{
	return TypeInsideSet(type_set);
}


#endif // !__FUNCTIONAL_UTILS_H__
