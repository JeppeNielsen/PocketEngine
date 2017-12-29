#!/bin/sh

#  Pocket Engine for web using Emscripten
#
#  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.

EMSCRIPTEN_PATH="/Projects/Emscripten/emscripten/1.35.0"
POCKET_PATH="../../Pocket"

#-s ALLOW_MEMORY_GROWTH=1 \

python $EMSCRIPTEN_PATH/emcc -O3 -s FULL_ES2=1 -s ASSERTIONS=1 \
--preload-file resources \
-s DISABLE_EXCEPTION_CATCHING=0 \
PocketEngine.bc PocketEngine_c.bc -o PocketEngine.html -std=c++14 \
\
--memory-init-file 0 \
\
-I $POCKET_PATH/ComponentSystem/ \
-I $POCKET_PATH/ComponentSystem/Meta/ \
-I $POCKET_PATH/Core/ \
-I $POCKET_PATH/Data/ \
-I $POCKET_PATH/Debugging/ \
-I $POCKET_PATH/Libs/FreeType/include/ \
-I $POCKET_PATH/Libs/FreeType/include/freetype/ \
-I $POCKET_PATH/Libs/FreeType/include/freetype/config/ \
-I $POCKET_PATH/Libs/FreeType/include/freetype/internal/ \
-I $POCKET_PATH/Libs/FreeType/include/freetype/internal/services/ \
-I $POCKET_PATH/Libs/FreeType/src/autofit/ \
-I $POCKET_PATH/Libs/FreeType/src/base/ \
-I $POCKET_PATH/Libs/FreeType/src/cff/ \
-I $POCKET_PATH/Libs/FreeType/src/pshinter/ \
-I $POCKET_PATH/Libs/FreeType/src/psnames/ \
-I $POCKET_PATH/Libs/FreeType/src/raster/ \
-I $POCKET_PATH/Libs/FreeType/src/sfnt/ \
-I $POCKET_PATH/Libs/FreeType/src/smooth/ \
-I $POCKET_PATH/Libs/FreeType/src/truetype/ \
-I $POCKET_PATH/Libs/STBImage/ \
-I $POCKET_PATH/Libs/TinyXml/ \
-I $POCKET_PATH/Libs/Zip/ \
-I $POCKET_PATH/Logic/Animation/ \
-I $POCKET_PATH/Logic/Assets/ \
-I $POCKET_PATH/Logic/Cloning/ \
-I $POCKET_PATH/Logic/Common/ \
-I $POCKET_PATH/Logic/Editor/ \
-I $POCKET_PATH/Logic/Effects/ \
-I $POCKET_PATH/Logic/Files/ \
-I $POCKET_PATH/Logic/Gui/ \
-I $POCKET_PATH/Logic/Gui/Layout/ \
-I $POCKET_PATH/Logic/Gui/Menu/ \
-I $POCKET_PATH/Logic/Input/ \
-I $POCKET_PATH/Logic/Interaction/ \
-I $POCKET_PATH/Logic/Movement/ \
-I $POCKET_PATH/Logic/Rendering/ \
-I $POCKET_PATH/Logic/Rendering/Lights/ \
-I $POCKET_PATH/Logic/Selection/ \
-I $POCKET_PATH/Logic/Spatial/ \
-I $POCKET_PATH/Logic/Switching/ \
-I $POCKET_PATH/Math/ \
-I $POCKET_PATH/OpenGL/ \
-I $POCKET_PATH/Packing/ \
-I $POCKET_PATH/Platform/Emscripten/ \
-I $POCKET_PATH/Profiler/ \
-I $POCKET_PATH/Rendering/ \
-I $POCKET_PATH/Serialization/ \
-I $POCKET_PATH/Social/ \
-I $POCKET_PATH/Web/ \
main.cpp \


#missing logic folders, ie audio may take a while
#-I $POCKET_PATH/Logic/Audio \
#-I $POCKET_PATH/Logic/Physics \


