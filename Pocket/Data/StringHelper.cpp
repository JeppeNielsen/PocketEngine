//
//  StringHelper.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "StringHelper.hpp"
#include <string>
#include <locale>
#include <codecvt>
#include <iostream>
using namespace Pocket;

std::vector<std::string>& StringHelper::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> StringHelper::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void StringHelper::splitToFloats(const std::string &s, char delim, std::vector<float>& floats) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    for (unsigned i=0; i<elems.size(); ++i) {
        double value = ::atof(elems[i].c_str());
        floats.push_back((float)value);
    }
}

void StringHelper::splitToInts(const std::string &s, char delim, std::vector<int>& ints) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    for (unsigned i=0; i<elems.size(); ++i) {
        int value = ::atoi(elems[i].c_str());
        ints.push_back(value);
    }
}

static int cc_wcslen(const unsigned short* str)
{
    int i=0;
    while(*str++) i++;
    return i;
}

/* Code from GLIB gutf8.c starts here. */

#define UTF8_COMPUTE(Char, Mask, Len)        \
if (Char < 128)                \
{                        \
Len = 1;                    \
Mask = 0x7f;                \
}                        \
else if ((Char & 0xe0) == 0xc0)        \
{                        \
Len = 2;                    \
Mask = 0x1f;                \
}                        \
else if ((Char & 0xf0) == 0xe0)        \
{                        \
Len = 3;                    \
Mask = 0x0f;                \
}                        \
else if ((Char & 0xf8) == 0xf0)        \
{                        \
Len = 4;                    \
Mask = 0x07;                \
}                        \
else if ((Char & 0xfc) == 0xf8)        \
{                        \
Len = 5;                    \
Mask = 0x03;                \
}                        \
else if ((Char & 0xfe) == 0xfc)        \
{                        \
Len = 6;                    \
Mask = 0x01;                \
}                        \
else                        \
Len = -1;

#define UTF8_LENGTH(Char)            \
((Char) < 0x80 ? 1 :                \
((Char) < 0x800 ? 2 :            \
((Char) < 0x10000 ? 3 :            \
((Char) < 0x200000 ? 4 :            \
((Char) < 0x4000000 ? 5 : 6)))))


#define UTF8_GET(Result, Chars, Count, Mask, Len)    \
(Result) = (Chars)[0] & (Mask);            \
for ((Count) = 1; (Count) < (Len); ++(Count))        \
{                            \
if (((Chars)[(Count)] & 0xc0) != 0x80)        \
{                        \
(Result) = -1;                \
break;                    \
}                        \
(Result) <<= 6;                    \
(Result) |= ((Chars)[(Count)] & 0x3f);        \
}

#define UNICODE_VALID(Char)            \
((Char) < 0x110000 &&                \
(((Char) & 0xFFFFF800) != 0xD800) &&        \
((Char) < 0xFDD0 || (Char) > 0xFDEF) &&    \
((Char) & 0xFFFE) != 0xFFFE)


static const char utf8_skip_data[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 6, 6, 1, 1
};

static const char *const g_utf8_skip = utf8_skip_data;

#define cc_utf8_next_char(p) (char *)((p) + g_utf8_skip[*(unsigned char *)(p)])

/*
 * @str:    the string to search through.
 * @c:        the character to find.
 *
 * Returns the index of the first occurrence of the character, if found.  Otherwise -1 is returned.
 *
 * Return value: the index of the first occurrence of the character if found or -1 otherwise.
 * */
static unsigned int cc_utf8_find_char(std::vector<unsigned short> str, unsigned short c)
{
    unsigned int len = (unsigned int)str.size();
    
    for (unsigned int i = 0; i < len; ++i)
        if (str[i] == c) return i;
    
    return -1;
}

/*
 * @str:    the string to search through.
 * @c:        the character to not look for.
 *
 * Return value: the index of the last character that is not c.
 * */
static unsigned int cc_utf8_find_last_not_char(std::vector<unsigned short> str, unsigned short c)
{
    int len = (int)str.size();
    
    int i = len - 1;
    for (; i >= 0; --i)
        if (str[i] != c) return i;
    
    return i;
}

/*
 * @str:    the string to trim
 * @index:    the index to start trimming from.
 *
 * Trims str st str=[0, index) after the operation.
 *
 * Return value: the trimmed string.
 * */
static void cc_utf8_trim_from(std::vector<unsigned short>* str, int index)
{
    int size = (int)str->size();
    if (index >= size || index < 0)
        return;
    
    str->erase(str->begin() + index, str->begin() + size);
}

/*
 * @ch is the unicode character whitespace?
 *
 * Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 * Return value: weather the character is a whitespace character.
 * */
static bool isspace_unicode(unsigned short ch)
{
    return  (ch >= 0x0009 && ch <= 0x000D) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
    || (ch >= 0x2000 && ch <= 0x200A) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
    ||  ch == 0x205F || ch == 0x3000;
}

static void cc_utf8_trim_ws(std::vector<unsigned short>* str)
{
    int len = (int)str->size();
    
    if ( len <= 0 )
        return;
    
    int last_index = len - 1;
    
    // Only start trimming if the last character is whitespace..
    if (isspace_unicode((*str)[last_index]))
    {
        for (int i = last_index - 1; i >= 0; --i)
        {
            if (isspace_unicode((*str)[i]))
                last_index = i;
            else
                break;
        }
        
        cc_utf8_trim_from(str, last_index);
    }
}

/*
 * g_utf8_strlen:
 * @p: pointer to the start of a UTF-8 encoded string.
 * @max: the maximum number of bytes to examine. If @max
 *       is less than 0, then the string is assumed to be
 *       null-terminated. If @max is 0, @p will not be examined and
 *       may be %NULL.
 *
 * Returns the length of the string in characters.
 *
 * Return value: the length of the string in characters
 **/
long
cc_utf8_strlen (const char * p, int max)
{
    long len = 0;
    const char *start = p;
    
    if (!(p != NULL || max == 0))
    {
        return 0;
    }
    
    if (max < 0)
    {
        while (*p)
        {
            p = cc_utf8_next_char (p);
            ++len;
        }
    }
    else
    {
        if (max == 0 || !*p)
            return 0;
        
        p = cc_utf8_next_char (p);
        
        while (p - start < max && *p)
        {
            ++len;
            p = cc_utf8_next_char (p);
        }
        
        /* only do the last len increment if we got a complete
         * char (don't count partial chars)
         */
        if (p - start == max)
            ++len;
    }
    
    return len;
}

/*
 * g_utf8_get_char:
 * @p: a pointer to Unicode character encoded as UTF-8
 *
 * Converts a sequence of bytes encoded as UTF-8 to a Unicode character.
 * If @p does not point to a valid UTF-8 encoded character, results are
 * undefined. If you are not sure that the bytes are complete
 * valid Unicode characters, you should use g_utf8_get_char_validated()
 * instead.
 *
 * Return value: the resulting character
 **/
static unsigned int
cc_utf8_get_char (const char * p)
{
    int i, mask = 0, len;
    unsigned int result;
    unsigned char c = (unsigned char) *p;
    
    UTF8_COMPUTE (c, mask, len);
    if (len == -1)
        return (unsigned int) - 1;
    UTF8_GET (result, p, i, mask, len);
    
    return result;
}

/*
 * cc_utf16_from_utf8:
 * @str_old: pointer to the start of a C string.
 *
 * Creates a utf8 string from a cstring.
 *
 * Return value: the newly created utf8 string.
 * */
static unsigned short* cc_utf16_from_utf8(const char* str_old)
{
    int len = (int)cc_utf8_strlen(str_old, -1);
    
    unsigned short* str_new = new unsigned short[len + 1];
    str_new[len] = 0;
    
    for (int i = 0; i < len; ++i)
    {
        str_new[i] = cc_utf8_get_char(str_old);
        str_old = cc_utf8_next_char(str_old);
    }
    
    return str_new;
}

static std::vector<unsigned short> cc_utf16_vec_from_utf16_str(const unsigned short* str)
{
    int len = cc_wcslen(str);
    std::vector<unsigned short> str_new;
    
    for (int i = 0; i < len; ++i)
    {
        str_new.push_back(str[i]);
    }
    return str_new;
}

std::vector<unsigned short> StringHelper::GetUTF(const std::string& s) {
    return cc_utf16_vec_from_utf16_str(cc_utf16_from_utf8(s.c_str()));
}


//std::string StringHelper::ConvertUTF8ToString(std::string utf8) {
    //std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    //std::u16string utf16 = convert.from_bytes(utf8);
    //return convert.to_bytes(utf16);
    /*
  
    // UTF-8 data. The character U+1d10b, musical sign segno, does not fit in UCS2
    std::string utf8 = u8"sp\u00f8rgsm\u00e5l";
    //    spørgsmål
    
    // the UTF-8 / UTF-16 standard conversion facet
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
    std::u16string utf16 = utf16conv.from_bytes(utf8);
    
    //const char* str = (const char*)utf16.c_str();
    
    //std::string test;
    //test.append((char16_t*)utf16.c_str(), utf16.size());
    
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    std::string a = convert.to_bytes(utf16);
    
    std::cout << a;
     */
    
  //  std::vector<unsigned short> utf16line;
   // utf8::utf8to16(utf8.begin(), utf8.end(), back_inserter(utf16line));
    
    
   // std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
   // std::u16string utf16;


//utf16.append(utf16line.begin(), utf16line.end());
    //return convert.to_bytes(utf16);
    
//    std::string utf8line;
    //utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));
 //   utf8line.append(utf16line.begin(), utf16line.end());
    
//    return utf8line;
//∫}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string StringHelper::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}



std::string StringHelper::base64_decode(const std::string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}



