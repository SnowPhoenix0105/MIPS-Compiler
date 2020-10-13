

#ifndef __LEXICAL_ANALYZER_H__
#define __LEXICAL_ANALYZER_H__

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "SymbolType.h"

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::istream;
using std::ostringstream;

class TryToGetUnexistNextException : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

class TryToGetUnexistIdentException : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

/// <summary>
/// 词法分析器, 从给定的流中读取源程序并进行词法分析;
/// 每次调用next()将分析并返回一个词(符号)的类型, 可以通过get_content()方法获得本次获得的符号的内容.
/// </summary>
class LexicalAnalyzer
{
private:
	unique_ptr<istream> input_stream;
	SymbolType last_symbol;
	shared_ptr<string> last_content;
	shared_ptr<string> lower_ident;
	int line_number = 1;
	char last_ch = ' ';

	/// <summary>
	/// 不断进行读取, 直到 last_ch 不为 ' ', '\n', '\t', '\r', '\v' 中的任何一个.
	/// </summary>
	void read_until_not_space();

	/// <summary>
	/// 读取下一个字符, 检查关系运算符, 并设置 last_content 与 last_symbol;
	/// 如果是 '=', 将其写入 last_content, 并将 last_symbol 置为 long_type;
	/// 否则仅将last_symbol 置为 short_type.
	/// </summary>
	/// <param name="long_type">下一个字符为 '=' 时 last_symbol 被设置的值</param>
	/// <param name="short_type">下一个字符不为 '=' 时 last_symbol 被设置的值</param>
	void check_compare_symbol(SymbolType long_type, SymbolType short_type);

	/// <summary>
	/// 检查字符字面量.
	/// </summary>
	void check_character_symbol();

	/// <summary>
	/// 检查字符串字面量.
	/// </summary>
	void check_string_symbol();

	/// <summary>
	/// 检查数字字面量.
	/// </summary>
	void check_number_symbol();

	/// <summary>
	/// 检查关键字, 以及变量/常量名.
	/// </summary>
	void check_name_symbol();

public:
	/////////////////////////// copy controls //////////////////////

	/// <summary>
	/// 移动构造函数.
	/// </summary>
	/// <param name="mov"></param>
	LexicalAnalyzer(LexicalAnalyzer&& mov) noexcept;

	~LexicalAnalyzer() = default;

	/// <summary>
	/// 移动赋值函数.
	/// </summary>
	/// <param name="mov"></param>
	/// <returns></returns>
	LexicalAnalyzer& operator=(LexicalAnalyzer&& mov) noexcept;

	friend void swap(LexicalAnalyzer& a, LexicalAnalyzer& b) noexcept;



	/////////////////////////// constructors //////////////////////

	/// <summary>
	/// 用打开指定的文件并以此为输入进行词法分析.
	/// </summary>
	/// <param name="input_file_name"></param>
	LexicalAnalyzer(const string& input_file_name);

	/// <summary>
	/// 用指定的input_file作为词法分析器的输入;
	/// 将移交input_file的所有权.
	/// </summary>
	/// <param name="input_file"></param>
	LexicalAnalyzer(unique_ptr<istream>&& input_file);



	/////////////////////// main methods /////////////////////////

	/// <summary>
	/// 检查是否可以继续调用 next();
	/// 返回false意味着已经读取完毕;
	/// </summary>
	/// <returns></returns>
	bool has_next()
	{
		return input_stream->good();
	}

	/// <summary>
	/// 请先通过 has_next() 方法检查是否可以继续调用;
	/// 读取指定流, 分析一个词(符号), 返回该词的类型;
	/// 再调用 get_content() 方法获得词的内容；
	/// 只有本次读取的词本身不完整时才会返回 SymbolType::wrong, 即遇到eof时, 除非词的结构不完整, 否则不会返回 wrong.
	/// </summary>
	/// <returns>本次分析的词(符号)的类型</returns>
	SymbolType next();

	/// <summary>
	/// 获取本次调用 next() 分析的词(符号)的原内容.
	/// </summary>
	/// <returns></returns>
	shared_ptr<const string> get_content() const
	{
		return last_content;
	}

	/// <summary>
	/// 获取本次调用 next() 分析的标识符的小写版本
	/// </summary>
	/// <returns></returns>
	shared_ptr<const string> get_lower_ident() const
	{
		if (last_symbol != SymbolType::identifier && keyword_symbol_set.find(last_symbol) == keyword_symbol_set.end())
		{
			throw TryToGetUnexistIdentException(
				"try to call get_lower_ident() at a LexicalAnalyzer with get_symbol() != identifier "
				"and get_symbol() not in keyword_symbol_set");
		}
		return lower_ident;
	}

	/// <summary>
	/// 获取本次调用 next() 分析的词(符号)的内类型, 与本次调用 next() 返回的结果相同.
	/// </summary>
	/// <returns></returns>
	SymbolType get_symbol() const
	{
		return last_symbol;
	}

	/// <summary>
	/// 获取本次调用 next() 分析的词(符号)所在的行数.
	/// </summary>
	/// <returns></returns>
	int get_line_number() const
	{
		return line_number;
	}

	unique_ptr<string> formated_content() const;
};



#endif // !__LEXICAL_ANALYZER_H__
