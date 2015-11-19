#!/bin/sh

#  BuildEmscipten.sh
#  Tetris
#
#  Created by Jeppe Nielsen on 4/6/14.
#  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.

NANO_PATH="../../../../Nano"

#-s ALLOW_MEMORY_GROWTH=1 \

python /Users/Jeppe/Downloads/emsdk_portable_new/emscripten/1.21.0/emcc -O2 -s FULL_ES2=1 -o cubism.html -std=c++11 --preload-file Assets -s ALLOW_MEMORY_GROWTH=1 \
\
-I $NANO_PATH/TinyXml/ \
-I $NANO_PATH/ComponentSystem/ \
-I $NANO_PATH/Core/ \
-I $NANO_PATH/Data/ \
-I $NANO_PATH/Debugging/ \
-I $NANO_PATH/Events/ \
-I $NANO_PATH/Logic/Animation \
-I $NANO_PATH/Logic/Common \
-I $NANO_PATH/Logic/Effects \
-I $NANO_PATH/Logic/Gui \
-I $NANO_PATH/Logic/Gui/Layout \
-I $NANO_PATH/Logic/Gui/Menu \
-I $NANO_PATH/Logic/Interaction \
-I $NANO_PATH/Logic/Movement \
-I $NANO_PATH/Logic/Rendering \
-I $NANO_PATH/Logic/Rendering/Lights \
-I $NANO_PATH/Logic/Rendering/RenderSubSystems \
-I $NANO_PATH/Logic/Selection \
-I $NANO_PATH/Logic/Spatial \
-I $NANO_PATH/Math/ \
-I $NANO_PATH/OpenGL/ \
-I $NANO_PATH/Packing/ \
-I $NANO_PATH/Platform/Emscripten/ \
-I $NANO_PATH/PngLib/ \
-I $NANO_PATH/Profiler/ \
-I $NANO_PATH/Rendering/ \
-I $NANO_PATH/Rendering/Shaders/ \
-I $NANO_PATH/Rendering/Shaders/Lit \
-I $NANO_PATH/Rendering/Shaders/Terrain \
-I $NANO_PATH/Rendering/Shaders/Unlit \
-I $NANO_PATH/Serialization/Json \
\
$NANO_PATH/TinyXml/tinystr.cpp \
$NANO_PATH/TinyXml/tinyxml.cpp \
$NANO_PATH/TinyXml/tinyxmlerror.cpp \
$NANO_PATH/TinyXml/tinyxmlparser.cpp \
\
$NANO_PATH/ComponentSystem/GameComponent.cpp \
$NANO_PATH/ComponentSystem/GameComponentType.cpp \
$NANO_PATH/ComponentSystem/GameFactory.cpp \
$NANO_PATH/ComponentSystem/GameObject.cpp \
$NANO_PATH/ComponentSystem/GameSystem.cpp \
$NANO_PATH/ComponentSystem/GameWorld.cpp \
\
$NANO_PATH/Core/Engine.cpp \
$NANO_PATH/Core/GameManager.cpp \
$NANO_PATH/Core/GameState.cpp \
$NANO_PATH/Core/InputDevice.cpp \
$NANO_PATH/Core/InputManager.cpp \
$NANO_PATH/Core/Timer.cpp \
$NANO_PATH/Core/Window.cpp \
\
$NANO_PATH/Data/Animation.cpp \
$NANO_PATH/Data/BezierCurve1.cpp \
$NANO_PATH/Data/BezierCurve3.cpp \
$NANO_PATH/Data/ICellItem.cpp \
$NANO_PATH/Data/Octree.cpp \
$NANO_PATH/Data/StringHelper.cpp \
$NANO_PATH/Data/Triangulator.cpp \
\
$NANO_PATH/Logic/Animation/Animator.cpp \
$NANO_PATH/Logic/Animation/AnimatorSystem.cpp \
$NANO_PATH/Logic/Animation/TouchAnimator.cpp \
$NANO_PATH/Logic/Animation/TouchAnimatorSystem.cpp \
$NANO_PATH/Logic/Animation/TransformAnimation.cpp \
$NANO_PATH/Logic/Animation/TransformAnimationDatabase.cpp \
$NANO_PATH/Logic/Animation/TransformAnimator.cpp \
$NANO_PATH/Logic/Animation/TransformAnimatorSystem.cpp \
$NANO_PATH/Logic/Common/Hierarchy.cpp \
$NANO_PATH/Logic/Common/HierarchyOrder.cpp \
$NANO_PATH/Logic/Common/Orderable.cpp \
$NANO_PATH/Logic/Effects/ParticleEffect.cpp \
$NANO_PATH/Logic/Effects/ParticleEmitter.cpp \
$NANO_PATH/Logic/Effects/ParticleMeshUpdater.cpp \
$NANO_PATH/Logic/Effects/ParticleUpdaterSystem.cpp \
$NANO_PATH/Logic/Gui/Font.cpp \
$NANO_PATH/Logic/Gui/Gui.cpp \
$NANO_PATH/Logic/Gui/Label.cpp \
$NANO_PATH/Logic/Gui/LabelMeshSystem.cpp \
$NANO_PATH/Logic/Gui/Layout/Layoutable.cpp \
$NANO_PATH/Logic/Gui/Layout/LayoutSystem.cpp \
$NANO_PATH/Logic/Gui/Menu/Menu.cpp \
$NANO_PATH/Logic/Gui/Menu/MenuButton.cpp \
$NANO_PATH/Logic/Gui/Menu/MenuButtonSystem.cpp \
$NANO_PATH/Logic/Gui/Menu/MenuSystem.cpp \
$NANO_PATH/Logic/Gui/Menu/MenuAnimator.cpp \
$NANO_PATH/Logic/Gui/Sizeable.cpp \
$NANO_PATH/Logic/Gui/Sprite.cpp \
$NANO_PATH/Logic/Gui/SpriteMeshSystem.cpp \
$NANO_PATH/Logic/Gui/SpriteTextureSystem.cpp \
$NANO_PATH/Logic/Gui/TextBox.cpp \
$NANO_PATH/Logic/Gui/TextBoxLabelSystem.cpp \
$NANO_PATH/Logic/Gui/TextBoxSystem.cpp \
$NANO_PATH/Logic/Interaction/Touchable.cpp \
$NANO_PATH/Logic/Interaction/TouchSystem.cpp \
$NANO_PATH/Logic/Interaction/TouchableCanceller.cpp \
$NANO_PATH/Logic/Interaction/TouchCancelSystem.cpp \
$NANO_PATH/Logic/Movement/Draggable.cpp \
$NANO_PATH/Logic/Movement/DraggableMotion.cpp \
$NANO_PATH/Logic/Movement/DraggableMotionSystem.cpp \
$NANO_PATH/Logic/Movement/DraggableSystem.cpp \
$NANO_PATH/Logic/Movement/FirstPersonMover.cpp \
$NANO_PATH/Logic/Movement/FirstPersonMoverSystem.cpp \
$NANO_PATH/Logic/Movement/Limitable.cpp \
$NANO_PATH/Logic/Movement/LimitableSystem.cpp \
$NANO_PATH/Logic/Movement/Velocity.cpp \
$NANO_PATH/Logic/Movement/VelocitySystem.cpp \
$NANO_PATH/Logic/Rendering/Atlas.cpp \
$NANO_PATH/Logic/Rendering/Camera.cpp \
$NANO_PATH/Logic/Rendering/Colorable.cpp \
$NANO_PATH/Logic/Rendering/Lights/Light.cpp \
$NANO_PATH/Logic/Rendering/Material.cpp \
$NANO_PATH/Logic/Rendering/Mesh.cpp \
$NANO_PATH/Logic/Rendering/RenderSubSystems/LightingSystem.cpp \
$NANO_PATH/Logic/Rendering/RenderSystem.cpp \
$NANO_PATH/Logic/Rendering/Terrain.cpp \
$NANO_PATH/Logic/Rendering/TerrainSystem.cpp \
$NANO_PATH/Logic/Rendering/TextureComponent.cpp \
$NANO_PATH/Logic/Rendering/VisibleObject.cpp \
$NANO_PATH/Logic/Selection/ClickSelectorSystem.cpp \
$NANO_PATH/Logic/Selection/DragSelector.cpp \
$NANO_PATH/Logic/Selection/ClickSelectorSystem.cpp \
$NANO_PATH/Logic/Selection/SelectableCollection.cpp \
$NANO_PATH/Logic/Selection/SelectableDragSystem.cpp \
$NANO_PATH/Logic/Selection/SelectionVisualizer.cpp \
$NANO_PATH/Logic/Spatial/OctreeSystem.cpp \
$NANO_PATH/Logic/Spatial/Transform.cpp \
$NANO_PATH/Logic/Spatial/TransformHierarchy.cpp \
\
$NANO_PATH/Math/BoundingBox.cpp \
$NANO_PATH/Math/BoundingFrustum.cpp \
$NANO_PATH/Math/Box.cpp \
$NANO_PATH/Math/Interpolator.cpp \
$NANO_PATH/Math/MathHelper.cpp \
$NANO_PATH/Math/Matrix3x3.cpp \
$NANO_PATH/Math/Matrix4x4.cpp \
$NANO_PATH/Math/Plane.cpp \
$NANO_PATH/Math/Point.cpp \
$NANO_PATH/Math/Point3.cpp \
$NANO_PATH/Math/Quaternion.cpp \
$NANO_PATH/Math/Ray.cpp \
$NANO_PATH/Math/TransitionHelper.cpp \
$NANO_PATH/Math/Vector2.cpp \
$NANO_PATH/Math/Vector3.cpp \
\
$NANO_PATH/Packing/RectPacker.cpp \
\
$NANO_PATH/Platform/Emscripten/FileReader.cpp \
$NANO_PATH/Platform/Emscripten/WindowWeb.cpp \
\
$NANO_PATH/PngLib/lodepng.cpp \
\
$NANO_PATH/Profiler/Profiler.cpp \
\
$NANO_PATH/Rendering/Clipper.cpp \
$NANO_PATH/Rendering/Colour.cpp \
$NANO_PATH/Rendering/Shaders/Lit/ShaderLitUncoloredTextured.cpp \
$NANO_PATH/Rendering/Shaders/Lit/ShaderLitUncoloredTexturedBatched.cpp \
$NANO_PATH/Rendering/Shaders/Lit/ShaderTexturedLighting.cpp \
$NANO_PATH/Rendering/Shaders/Shader.cpp \
$NANO_PATH/Rendering/Shaders/ShaderParticleSystem.cpp \
$NANO_PATH/Rendering/Shaders/ShaderProgram.cpp \
$NANO_PATH/Rendering/Shaders/Terrain/ShaderTerrain.cpp \
$NANO_PATH/Rendering/Shaders/Unlit/ShaderUnlitUncolored.cpp \
$NANO_PATH/Rendering/Shaders/Unlit/ShaderUnlitUncoloredTextured.cpp \
$NANO_PATH/Rendering/Texture.cpp \
$NANO_PATH/Rendering/TextureAtlas.cpp \
$NANO_PATH/Rendering/TexturePacker.cpp \
$NANO_PATH/Rendering/Vertex.cpp \
$NANO_PATH/Rendering/VertexRenderer.cpp \
\
-I ../States/ \
-I ../Logic/ \
-I ../Puzzles/ \
../States/Game.cpp \
../Logic/Block.cpp \
../Logic/BlockBlockedSystem.cpp \
../Logic/BlockMeshSystem.cpp \
../Logic/BlockMoverSystem.cpp \
../Logic/BlockParentSystem.cpp \
../Logic/BlockPatchSnapSystem.cpp \
../Logic/BlockRotaterSystem.cpp \
../Logic/BlockTransformer.cpp \
../Logic/Patch.cpp \
../Logic/PatchMeshSystem.cpp \
../Logic/PuzzleCompletionChecker.cpp \
../Logic/PuzzleDestructorSystem.cpp \
../Logic/PuzzleSystem.cpp \
../Puzzles/Puzzle.cpp \
../Puzzles/PuzzleDatabase.cpp \
../Puzzles/PuzzleProgress.cpp \
../States/PlayingState.cpp \
../States/MenuState.cpp \
../States/Game.cpp \
../main.cpp \

#missing logic folders, ie audio may take a while
#-I $NANO_PATH/Logic/Audio \
#-I $NANO_PATH/Logic/Physics \


