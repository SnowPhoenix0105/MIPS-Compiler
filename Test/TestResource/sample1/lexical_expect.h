#ifndef __SAMPLE1_LEXICAL_EXPECT_H__
#define __SAMPLE1_LEXICAL_EXPECT_H__

#include <vector>

const std::vector<SymbolType> sample1_expect_answer = 
{
// const char c_1 = 'o';
    SymbolType::key_const,
    SymbolType::key_char,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::character,
    SymbolType::semicolon,

// const char c_2 = 'p', c_3 = 'a';
    SymbolType::key_const,
    SymbolType::key_char,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::character,
    SymbolType::semicolon,

// char c_4 = 'b';
    SymbolType::key_char,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::character,
    SymbolType::semicolon,

// char c_array[3] = {'c', 'd', 'e'};
    SymbolType::key_char,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::assign,
    SymbolType::left_brance,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::right_brance,
    SymbolType::semicolon,

// char c_matrix[3][3] = 
    SymbolType::key_char,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::assign,

// {
    SymbolType::left_brance,

//    {'f', 'g', 'h'},
    SymbolType::left_brance,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::right_brance,
    SymbolType::comma,

//    {'i', 'j', 'k'},
    SymbolType::left_brance,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::right_brance,
    SymbolType::comma,

//    {'l', 'm', 'n'}
    SymbolType::left_brance,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::comma,
    SymbolType::character,
    SymbolType::right_brance,

// };
    SymbolType::right_brance,
    SymbolType::semicolon,

// void main()
    SymbolType::key_void,
    SymbolType::key_main,
    SymbolType::left_paren,
    SymbolType::right_paren,

// {
    SymbolType::left_brance,

//    const int i_1 = 1;
    SymbolType::key_const,
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    const int i_2 = 2, i_3 = 3;
    SymbolType::key_const,
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    int  i_4 = 4;
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    int i_array[3] = {5, 6, 7};
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::assign,
    SymbolType::left_brance,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::right_brance,
    SymbolType::semicolon,

//    int i_matrix[3][3] = 
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::assign,

//    {
    SymbolType::left_brance,

//        {8 , 9 , 10},
    SymbolType::left_brance,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::right_brance,
    SymbolType::comma,

//        {11, 12, 13},
    SymbolType::left_brance,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::right_brance,
    SymbolType::comma,

//        {14, 15, 16}
    SymbolType::left_brance,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::comma,
    SymbolType::number,
    SymbolType::right_brance,

//    };
    SymbolType::right_brance,
    SymbolType::semicolon,

//    int result;
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::semicolon,

//    int i, j;
    SymbolType::key_int,
    SymbolType::identifier,
    SymbolType::comma,
    SymbolType::identifier,
    SymbolType::semicolon,

//    char result_c;
    SymbolType::key_char,
    SymbolType::identifier,
    SymbolType::semicolon,

//    printf("18231045");
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    result = c_1 + c_2 + c_3 + c_4;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::semicolon,

//    for (i = 0; i < 3; i = i + 1)
    SymbolType::key_for,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,
    SymbolType::identifier,
    SymbolType::less,
    SymbolType::number,
    SymbolType::semicolon,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::number,
    SymbolType::right_paren,

//    {
    SymbolType::left_brance,

//        result = result + c_array[i];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::identifier,
    SymbolType::right_square,
    SymbolType::semicolon,

//        for (j = 2; j >= 0; j = j - 1)
    SymbolType::key_for,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,
    SymbolType::identifier,
    SymbolType::greater_equal,
    SymbolType::number,
    SymbolType::semicolon,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::minus,
    SymbolType::number,
    SymbolType::right_paren,

//        {
    SymbolType::left_brance,

//            result = result + c_matrix[i][j];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::identifier,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::identifier,
    SymbolType::right_square,
    SymbolType::semicolon,

//        }
    SymbolType::right_brance,

//    }
    SymbolType::right_brance,

//    printf("char sum:");
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    printf(result);
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    result = i_matrix[2][2];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::semicolon,

//    printf("i_matric[2][2]=", result);
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::comma,
    SymbolType::identifier,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    result_c = c_matrix[2][2];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::semicolon,

//    printf(result_c);
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    switch (i_matrix[1][1])
    SymbolType::key_switch,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::right_paren,

//    {
    SymbolType::left_brance,

//    case 0:
    SymbolType::key_case,
    SymbolType::number,
    SymbolType::colon,

//        result = 0;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    case 1:
    SymbolType::key_case,
    SymbolType::number,
    SymbolType::colon,

//        result = 1;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    case 2:
    SymbolType::key_case,
    SymbolType::number,
    SymbolType::colon,

//        result = 2;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    case 3:
    SymbolType::key_case,
    SymbolType::number,
    SymbolType::colon,

//        result = 3;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    case 12:
    SymbolType::key_case,
    SymbolType::number,
    SymbolType::colon,

//        result = 4;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    case 100:
    SymbolType::key_case,
    SymbolType::number,
    SymbolType::colon,

//        { }
    SymbolType::left_brance,
    SymbolType::right_brance,

//    default:
    SymbolType::key_default,
    SymbolType::colon,

//        result = 5;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    }
    SymbolType::right_brance,

//    printf("switch to case", result);
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::comma,
    SymbolType::identifier,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    if (result <= 4)
    SymbolType::key_if,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::less_equal,
    SymbolType::number,
    SymbolType::right_paren,

//    {
    SymbolType::left_brance,

//        printf("if success!");
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    }
    SymbolType::right_brance,

//    if (result > 4)
    SymbolType::key_if,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::greater,
    SymbolType::number,
    SymbolType::right_paren,

//    {
    SymbolType::left_brance,

//        result = 0;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    }
    SymbolType::right_brance,

//    else
    SymbolType::key_else,

//    {
    SymbolType::left_brance,

//        result = 1;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//    }
    SymbolType::right_brance,

//    printf("else success: ", result);
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::comma,
    SymbolType::identifier,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    result = i_array[0];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::semicolon,

//    i_array[0] = i_matrix[0][0];
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::semicolon,

//    i_matrix[0][0] = result;
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::number,
    SymbolType::right_square,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::semicolon,

//    result = i_1 + i_2 + i_3 + i_4;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::semicolon,

//    for (i = 0; i != 3; i = i + 1)
    SymbolType::key_for,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,
    SymbolType::identifier,
    SymbolType::not_equal,
    SymbolType::number,
    SymbolType::semicolon,
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::number,
    SymbolType::right_paren,

//    {
    SymbolType::left_brance,

//        result = result + i_array[i];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::identifier,
    SymbolType::right_square,
    SymbolType::semicolon,

//        j = 2;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::number,
    SymbolType::semicolon,

//        while (j >= 0)
    SymbolType::key_while,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::greater_equal,
    SymbolType::number,
    SymbolType::right_paren,

//        {
    SymbolType::left_brance,

//            result = result + i_matrix[i][j];
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::plus,
    SymbolType::identifier,
    SymbolType::left_square,
    SymbolType::identifier,
    SymbolType::right_square,
    SymbolType::left_square,
    SymbolType::identifier,
    SymbolType::right_square,
    SymbolType::semicolon,

//            j = j - 1;
    SymbolType::identifier,
    SymbolType::assign,
    SymbolType::identifier,
    SymbolType::minus,
    SymbolType::number,
    SymbolType::semicolon,

//        }
    SymbolType::right_brance,

//    }
    SymbolType::right_brance,

//    printf("int sum:");
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::string,
    SymbolType::right_paren,
    SymbolType::semicolon,

//    printf(result);
    SymbolType::key_printf,
    SymbolType::left_paren,
    SymbolType::identifier,
    SymbolType::right_paren,
    SymbolType::semicolon,

// }
    SymbolType::right_brance
};

#endif //!__SAMPLE1_LEXICAL_EXPECT_H__
