

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
/// �ʷ�������, �Ӹ��������ж�ȡԴ���򲢽��дʷ�����;
/// ÿ�ε���next()������������һ����(����)������, ����ͨ��get_content()������ñ��λ�õķ��ŵ�����.
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
	/// ���Ͻ��ж�ȡ, ֱ�� last_ch ��Ϊ ' ', '\n', '\t', '\r', '\v' �е��κ�һ��.
	/// </summary>
	void read_until_not_space();

	/// <summary>
	/// ��ȡ��һ���ַ�, ����ϵ�����, ������ last_content �� last_symbol;
	/// ����� '=', ����д�� last_content, ���� last_symbol ��Ϊ long_type;
	/// �������last_symbol ��Ϊ short_type.
	/// </summary>
	/// <param name="long_type">��һ���ַ�Ϊ '=' ʱ last_symbol �����õ�ֵ</param>
	/// <param name="short_type">��һ���ַ���Ϊ '=' ʱ last_symbol �����õ�ֵ</param>
	void check_compare_symbol(SymbolType long_type, SymbolType short_type);

	/// <summary>
	/// ����ַ�������.
	/// </summary>
	void check_character_symbol();

	/// <summary>
	/// ����ַ���������.
	/// </summary>
	void check_string_symbol();

	/// <summary>
	/// �������������.
	/// </summary>
	void check_number_symbol();

	/// <summary>
	/// ���ؼ���, �Լ�����/������.
	/// </summary>
	void check_name_symbol();

public:
	/////////////////////////// copy controls //////////////////////

	/// <summary>
	/// �ƶ����캯��.
	/// </summary>
	/// <param name="mov"></param>
	LexicalAnalyzer(LexicalAnalyzer&& mov) noexcept;

	~LexicalAnalyzer() = default;

	/// <summary>
	/// �ƶ���ֵ����.
	/// </summary>
	/// <param name="mov"></param>
	/// <returns></returns>
	LexicalAnalyzer& operator=(LexicalAnalyzer&& mov) noexcept;

	friend void swap(LexicalAnalyzer& a, LexicalAnalyzer& b) noexcept;



	/////////////////////////// constructors //////////////////////

	/// <summary>
	/// �ô�ָ�����ļ����Դ�Ϊ������дʷ�����.
	/// </summary>
	/// <param name="input_file_name"></param>
	LexicalAnalyzer(const string& input_file_name);

	/// <summary>
	/// ��ָ����input_file��Ϊ�ʷ�������������;
	/// ���ƽ�input_file������Ȩ.
	/// </summary>
	/// <param name="input_file"></param>
	LexicalAnalyzer(unique_ptr<istream>&& input_file);



	/////////////////////// main methods /////////////////////////

	/// <summary>
	/// ����Ƿ���Լ������� next();
	/// ����false��ζ���Ѿ���ȡ���;
	/// </summary>
	/// <returns></returns>
	bool has_next()
	{
		return input_stream->good();
	}

	/// <summary>
	/// ����ͨ�� has_next() ��������Ƿ���Լ�������;
	/// ��ȡָ����, ����һ����(����), ���ظôʵ�����;
	/// �ٵ��� get_content() ������ôʵ����ݣ�
	/// ֻ�б��ζ�ȡ�Ĵʱ�������ʱ�Ż᷵�� SymbolType::wrong, ������eofʱ, ���ǴʵĽṹ������, ���򲻻᷵�� wrong.
	/// </summary>
	/// <returns>���η����Ĵ�(����)������</returns>
	SymbolType next();

	/// <summary>
	/// ��ȡ���ε��� next() �����Ĵ�(����)��ԭ����.
	/// </summary>
	/// <returns></returns>
	shared_ptr<const string> get_content() const
	{
		return last_content;
	}

	/// <summary>
	/// ��ȡ���ε��� next() �����ı�ʶ����Сд�汾
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
	/// ��ȡ���ε��� next() �����Ĵ�(����)��������, �뱾�ε��� next() ���صĽ����ͬ.
	/// </summary>
	/// <returns></returns>
	SymbolType get_symbol() const
	{
		return last_symbol;
	}

	/// <summary>
	/// ��ȡ���ε��� next() �����Ĵ�(����)���ڵ�����.
	/// </summary>
	/// <returns></returns>
	int get_line_number() const
	{
		return line_number;
	}

	unique_ptr<string> formated_content() const;
};



#endif // !__LEXICAL_ANALYZER_H__
