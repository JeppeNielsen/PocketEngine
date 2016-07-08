




#!/bin/sh

#  Pocket Engine for web using Emscripten
#
#  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.

CLANG_PATH="ar"
POCKET_PATH="../../../Pocket"

#$CLANG_PATH -std=c++11 -stdlib=libc++ -g -DOSX -o game \


$CLANG_PATH -rcs libPocketEngine.a \
\
BoundingBox.o \
BoundingFrustum.o \
Box.o \
Interpolator.o \
MathHelper.o \
Matrix3x3.o \
Matrix4x4.o \
Plane.o \
Point.o \
Point3.o \
Quaternion.o \
Ray.o \
Rect.o \
TransitionHelper.o \
Vector2.o \
Vector3.o \
\
BezierCurve1.o \
BezierCurve3.o \
Bitset.o \
ICellItem.o \
Octree.o \
Pointer.o \
QuadTree.o \
StringHelper.o \
Triangulator.o \
\
Engine.o \
EngineContext.o \
GameState.o \
InputDevice.o \
InputManager.o \
Timer.o \
Window.o \
\
AppDelegate.o \
File.o \
FileReader.o \
GamePadManager.o \
OSXView.o \
OSXWindow.o \
OSXWindowCreator.o \
WindowOSX.o \
\
#main.o \
\


