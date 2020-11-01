
#ifndef __INITIALIZE_LIST_ANALYZE_H__
#define __INITIALIZE_LIST_ANALYZE_H__

#include <memory>
#include "global_control.h"
#include "SyntacticAnalyzeUtil.h"

using std::shared_ptr;

struct InitializeListElement
{
	using elem_ptr = shared_ptr<InitializeListElement>;
	int line_number;
	InitializeListElement(int line_number) : line_number(line_number) { }
	virtual ~InitializeListElement() = default;
	virtual int end_line()
	{
		return line_number;
	}
	virtual int start_line() = 0;
};

struct InitializeListChar : InitializeListElement
{
	char content;
	InitializeListChar(int line_number, char content) : InitializeListElement(line_number), content(content) { }
	virtual ~InitializeListChar() = default;
	virtual int start_line()
	{
		return line_number;
	}
	virtual int end_line()
	{
		return line_number;
	}
};

struct InitializeListInteger : InitializeListElement
{
	int content;
	InitializeListInteger(int line_number, int content) : InitializeListElement(line_number), content(content) { }
	virtual ~InitializeListInteger() = default;
	virtual int start_line()
	{
		return line_number;
	}
	virtual int end_line()
	{
		return line_number;
	}
};

struct InitializeList : InitializeListElement
{
	vector<elem_ptr> list;
	int last_line;
	InitializeList(int line_number) : InitializeListElement(line_number), list(), last_line(line_number) { }
	virtual ~InitializeList() = default;
	virtual int start_line()
	{
		return line_number;
	}
	virtual int end_line()
	{
		return last_line;
	}
};


#endif // !__INITIALIZE_LIST_ANALYZE_H__
