




#!/bin/sh

#  Pocket Engine for web using Emscripten
#
#  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.

CLANG_PATH="/Users/Jeppe/Downloads/clang+llvm-3.7.0-x86_64-apple-darwin/bin/clang++"
POCKET_PATH="../../../Pocket"

#$CLANG_PATH -std=c++11 -stdlib=libc++ -g -DOSX -o game \


$CLANG_PATH -std=c++11 -stdlib=libc++ -DOSX -o PocketEngine.a \
\
\
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
$POCKET_PATH/Math/BoundingBox.cpp \
$POCKET_PATH/Math/BoundingFrustum.cpp \
$POCKET_PATH/Math/Box.cpp \
$POCKET_PATH/Math/Interpolator.cpp \
$POCKET_PATH/Math/MathHelper.cpp \
$POCKET_PATH/Math/Matrix3x3.cpp \
$POCKET_PATH/Math/Matrix4x4.cpp \
$POCKET_PATH/Math/Plane.cpp \
$POCKET_PATH/Math/Point.cpp \
$POCKET_PATH/Math/Point3.cpp \
$POCKET_PATH/Math/Quaternion.cpp \
$POCKET_PATH/Math/Ray.cpp \
$POCKET_PATH/Math/Rect.cpp \
$POCKET_PATH/Math/TransitionHelper.cpp \
$POCKET_PATH/Math/Vector2.cpp \
$POCKET_PATH/Math/Vector3.cpp \
\
$POCKET_PATH/Data/BezierCurve1.cpp \
$POCKET_PATH/Data/BezierCurve3.cpp \
$POCKET_PATH/Data/Bitset.cpp \
$POCKET_PATH/Data/ICellItem.cpp \
$POCKET_PATH/Data/Octree.cpp \
$POCKET_PATH/Data/Pointer.cpp \
$POCKET_PATH/Data/QuadTree.cpp \
$POCKET_PATH/Data/StringHelper.cpp \
$POCKET_PATH/Data/Triangulator.cpp \
\
$POCKET_PATH/Core/Engine.cpp \
$POCKET_PATH/Core/EngineContext.cpp \
$POCKET_PATH/Core/GameState.cpp \
$POCKET_PATH/Core/InputDevice.cpp \
$POCKET_PATH/Core/InputManager.cpp \
$POCKET_PATH/Core/Timer.cpp \
$POCKET_PATH/Core/Window.cpp \
\
$POCKET_PATH/Platform/OSX/AppDelegate.mm \
$POCKET_PATH/Platform/OSX/File.mm \
$POCKET_PATH/Platform/OSX/FileReader.mm \
$POCKET_PATH/Platform/OSX/GamePadManager.mm \
$POCKET_PATH/Platform/OSX/OSXView.mm \
$POCKET_PATH/Platform/OSX/OSXWindow.mm \
$POCKET_PATH/Platform/OSX/OSXWindowCreator.mm \
$POCKET_PATH/Platform/OSX/WindowOSX.mm \
\
#main.cpp \
\


