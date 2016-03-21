//
//  ClangUtil.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ClangUtil.hpp"

#include <iostream>

std::ostream&
operator<<(std::ostream& stream, const LocationInfo& location)
{

    std::cout << location.filePath <<
        " line: " << location.line <<
        ", column: " << location.column <<
        ", offset: " << location.offset;

    return stream;
}

std::string
getClangString(CXString str)
{
    const char* tmp = clang_getCString(str);
    if (tmp == NULL)
    {
        return "";
    }
    else
    {
        std::string translated = std::string(tmp);
        clang_disposeString(str);
        return translated;
    }
}

std::string
toStdString(CXString cxstr)
{
    const char* tmp = clang_getCString(cxstr);
    if (tmp == NULL)
    {
        return "";
    }
    else
    {
        std::string translated = std::string(tmp);
        clang_disposeString(cxstr);
        return translated;
    }
}

std::string toStdString(CXCursor cursor)
{
    return toStdString(clang_getCursorSpelling(cursor));
}

std::string toStdString(CXCursorKind kind)
{
    return toStdString(clang_getCursorKindSpelling(kind));
}

std::string toStdString(CXToken token, CXTranslationUnit tu)
{
    return toStdString(clang_getTokenSpelling(tu, token));
}

std::string toStdString(CXType type)
{
    return toStdString(clang_getTypeSpelling(type));
}



LocationInfo
getLocationInfo(const CXSourceLocation& location)
{
    CXFile file;
    unsigned int line;
    unsigned int column;
    unsigned int offset;

    clang_getFileLocation(location, &file, &line, &column, &offset);

    return LocationInfo(getClangString(clang_getFileName(file)),
        line, column, offset);
}

LocationInfo
getLocationInfo(const CXCursor& cursor)
{
    CXFile file;
    unsigned int line;
    unsigned int column;
    unsigned int offset;

    CXSourceLocation location = clang_getCursorLocation(cursor);

    clang_getFileLocation(location, &file, &line, &column, &offset);

    return LocationInfo(getClangString(clang_getFileName(file)),
        line, column, offset);
}