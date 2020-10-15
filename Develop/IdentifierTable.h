#pragma once

#ifndef __IDENTIFIER_TABLE_H__
#define __IDENTIFIER_TABLE_H__

#include "global_control.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;
using std::make_shared;
using std::make_pair;

enum class BaseType
{
	type_int, type_char, type_void
};

enum class ExternType : unsigned
{
	function = 0b0001, 
	array	 = 0b0010, 
	variable = 0b0100, 
	constant = 0b1000
};

struct IdentifierType
{
	BaseType base_type;
	ExternType extern_type;

	bool is(ExternType type) const
	{
		return (static_cast<unsigned>(extern_type) & static_cast<unsigned>(type)) != 0;
	}

	bool is(ExternType t1, ExternType t2) const
	{
		return (static_cast<unsigned>(extern_type) & (static_cast<unsigned>(t1) | static_cast<unsigned>(t2))) != 0;
	}
	
	bool is(ExternType t1, ExternType t2, ExternType t3) const
	{
		return (static_cast<unsigned>(extern_type) & (static_cast<unsigned>(t1) | static_cast<unsigned>(t2) | static_cast<unsigned>(t3))) != 0;
	}
};

struct IdentifierInfo
{
	shared_ptr<const string> id;
	shared_ptr<const IdentifierType> return_type;
	virtual ~IdentifierInfo() = default;
};

struct ConstantIdentifierInfo : IdentifierInfo
{
	virtual ~ConstantIdentifierInfo() = default;
};

struct IntegerIdentifierInfo : ConstantIdentifierInfo
{
	unsigned long value;
	virtual ~IntegerIdentifierInfo() = default;
};

struct CharactorIdentifierInfo : ConstantIdentifierInfo
{
	char value;
	virtual ~CharactorIdentifierInfo() = default;
};

struct LinearArrayIdentifierInfo : IdentifierInfo
{
	int size;
	virtual ~LinearArrayIdentifierInfo() = default;
};

struct DoubleDimensionalArrayIdentifierInfo : IdentifierInfo
{
	int size_1;
	int size_2;
	int total_size()
	{
		return size_1 * size_2;
	}
	virtual ~DoubleDimensionalArrayIdentifierInfo() = default;
};


class IdentifierTable
{
private:
	unordered_map<string, shared_ptr<const IdentifierInfo>> table;
public:
	void clear()
	{
		table.clear();
	}

	void remove_identifier(shared_ptr<const IdentifierInfo> id)
	{
		auto it = table.find(*id->id);
		if (it != table.end() && it->second == id)
		{
			table.erase(it);
		}
	}

	void insert_identifier(shared_ptr<const IdentifierInfo> id)
	{
		table.insert(make_pair(*id->id, id));
	}

	shared_ptr<const IdentifierInfo> get_identifier(shared_ptr<string> id) const 
	{
		return table.at(*id);
	}

	shared_ptr<const IdentifierInfo> get_identifier(const string& id) const 
	{
		return table.at(id);
	}

	shared_ptr<const IdentifierInfo> get_identifier(string&& id) const
	{
		return table.at(id);
	}
};



#endif // !__IDENTIFIER_TABLE_H__