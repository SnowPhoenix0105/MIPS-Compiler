
#ifndef __IDENTIFIER_TABLE_H__
#define __IDENTIFIER_TABLE_H__

#include "global_control.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "IrTable.h"

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
	function = 0b00001, 
	l_array	 = 0b00010,
	d_array	 = 0b00100,
	variable = 0b01000, 
	constant = 0b10000
};

struct IdentifierType
{
	// 仅 extern_type == function 时, 可以为type_void
	BaseType base_type;
	ExternType extern_type;

	bool is_one_from(ExternType type) const
	{
		return (static_cast<unsigned>(extern_type) & static_cast<unsigned>(type)) != 0;
	}

	bool is_one_from(ExternType t1, ExternType t2) const
	{
		return (static_cast<unsigned>(extern_type) & (static_cast<unsigned>(t1) | static_cast<unsigned>(t2))) != 0;
	}
	
	bool is_one_from(ExternType t1, ExternType t2, ExternType t3) const
	{
		return (static_cast<unsigned>(extern_type) & (static_cast<unsigned>(t1) | static_cast<unsigned>(t2) | static_cast<unsigned>(t3))) != 0;
	}


	bool is_one_from(ExternType t1, ExternType t2, ExternType t3, ExternType t4) const
	{
		return (static_cast<unsigned>(extern_type) & (static_cast<unsigned>(t1) | static_cast<unsigned>(t2) | static_cast<unsigned>(t3) | static_cast<unsigned>(t4))) != 0;
	}

	virtual ~IdentifierType() = default;
};

// extern_type == l_array or d_array
struct ArrayIdentifierType : IdentifierType
{
	virtual int total_size() const = 0;
	virtual ~ArrayIdentifierType() = default;
};

// extern_type == l_array
struct LinearArrayIdentifierType : ArrayIdentifierType
{
	unsigned size;
	virtual int total_size() const { return size; }
	virtual ~LinearArrayIdentifierType() = default;
};

// extern_type == d_array
struct DoubleDimensionalArrayIdentifierType : ArrayIdentifierType
{
	unsigned size_1;
	unsigned size_2;
	virtual int total_size() const
	{
		return size_1 * size_2;
	}
	virtual ~DoubleDimensionalArrayIdentifierType() = default;
};

// extern_type == function
struct FuctionIdentifierType : IdentifierType
{
	// 只能是 type_int / type_char
	shared_ptr<const vector<BaseType>> param_type_list;
	virtual ~FuctionIdentifierType() = default;
};

struct IdentifierInfo
{
	shared_ptr<const string> id;
	shared_ptr<const IdentifierType> return_type;
	irelem_t ir_id;
	virtual ~IdentifierInfo() = default;
};

// extern_type == constant
struct ConstantIdentifierInfo : IdentifierInfo
{
	virtual int get_value() const = 0;
	virtual ~ConstantIdentifierInfo() = default;
};

// extern_type == constant && base_type == type_int
struct IntegerIdentifierInfo : ConstantIdentifierInfo
{
	unsigned long value;
	virtual int get_value() const {
		return static_cast<int>(value);
	}
	virtual ~IntegerIdentifierInfo() = default;
};

// extern_type == constant && base_type == type_char
struct CharactorIdentifierInfo : ConstantIdentifierInfo
{
	char value;
	virtual int get_value() const {
		return static_cast<int>(value);
	}
	virtual ~CharactorIdentifierInfo() = default;
};

// extern_type == function
struct FunctionIdentifierInfo : IdentifierInfo
{
	irelem_t mid_label;
	irelem_t end_label;
	virtual ~FunctionIdentifierInfo() = default;
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

	shared_ptr<const IdentifierInfo> get_identifier(const char* id) const
	{
		return table.at(string(id));
	}

	shared_ptr<const IdentifierInfo> get_identifier(shared_ptr<const string> id) const 
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