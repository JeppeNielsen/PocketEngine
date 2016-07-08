




#!/bin/sh

#  Pocket Engine for web using Emscripten
#
#  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.

CLANG_PATH="/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++"
POCKET_PATH="../../../Pocket"

#$CLANG_PATH -std=c++11 -stdlib=libc++ -g -DOSX -o game \
#add -g for dSYM file

# -framework Foundation \
# -framework OpenGL \
# -framework IOKit \
# -framework Cocoa \

$CLANG_PATH -std=c++11 -stdlib=libc++ -DOSX libPocketEngine.a -o game -O3 -x objective-c++ -arch x86_64 -lobjc \
\
 -framework Foundation \
 -framework OpenGL \
 -framework IOKit \
 -framework Cocoa \
\
-I $POCKET_PATH/OpenGL/ \
-I $POCKET_PATH/Math/ \
-I $POCKET_PATH/Data/ \
-I $POCKET_PATH/Core/ \
-I $POCKET_PATH/Platform/OSX \
\
main.cpp \
\


