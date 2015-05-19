#!/bin/sh

#  Pocket Engine for web using Emscripten
#
#  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.

EMSCRIPTEN_PATH="/Users/Jeppe/Downloads/emsdk_portable/emscripten/1.29.0"
POCKET_PATH="../../Pocket"

#-s ALLOW_MEMORY_GROWTH=1 \

python $EMSCRIPTEN_PATH/emcc -O3 -s FULL_ES2=1 -o PocketEngine.html -std=c++11 \
\
--memory-init-file 0 \
\
-I $POCKET_PATH/ComponentSystem/ \
-I $POCKET_PATH/Core/ \
-I $POCKET_PATH/Data/ \
-I $POCKET_PATH/Debugging/ \
-I $POCKET_PATH/Events/ \
-I $POCKET_PATH/Logic/Animation \
-I $POCKET_PATH/Logic/Common \
-I $POCKET_PATH/Logic/Effects \
-I $POCKET_PATH/Logic/Gui \
-I $POCKET_PATH/Logic/Gui/Layout \
-I $POCKET_PATH/Logic/Gui/Menu \
-I $POCKET_PATH/Logic/Interaction \
-I $POCKET_PATH/Logic/Movement \
-I $POCKET_PATH/Logic/Rendering \
-I $POCKET_PATH/Logic/Rendering/Lights \
-I $POCKET_PATH/Logic/Selection \
-I $POCKET_PATH/Logic/Spatial \
-I $POCKET_PATH/Math/ \
-I $POCKET_PATH/OpenGL/ \
-I $POCKET_PATH/Packing/ \
-I $POCKET_PATH/Platform/Emscripten/ \
-I $POCKET_PATH/PngLib/ \
-I $POCKET_PATH/Profiler/ \
-I $POCKET_PATH/Rendering/ \
-I $POCKET_PATH/Rendering/Shaders/ \
-I $POCKET_PATH/Rendering/Shaders/Lit \
-I $POCKET_PATH/Rendering/Shaders/Terrain \
-I $POCKET_PATH/Rendering/Shaders/Unlit \
-I $POCKET_PATH/TinyXml/ \
\
$POCKET_PATH/TinyXml/tinystr.cpp \
$POCKET_PATH/TinyXml/tinyxml.cpp \
$POCKET_PATH/TinyXml/tinyxmlerror.cpp \
$POCKET_PATH/TinyXml/tinyxmlparser.cpp \
\
$POCKET_PATH/ComponentSystem/GameComponent.cpp \
$POCKET_PATH/ComponentSystem/GameComponentType.cpp \
$POCKET_PATH/ComponentSystem/GameFactory.cpp \
$POCKET_PATH/ComponentSystem/GameObject.cpp \
$POCKET_PATH/ComponentSystem/GameSystem.cpp \
$POCKET_PATH/ComponentSystem/GameWorld.cpp \
$POCKET_PATH/ComponentSystem/SerializedProperty.cpp \
\
$POCKET_PATH/Core/Engine.cpp \
$POCKET_PATH/Core/GameManager.cpp \
$POCKET_PATH/Core/GameState.cpp \
$POCKET_PATH/Core/InputDevice.cpp \
$POCKET_PATH/Core/InputManager.cpp \
$POCKET_PATH/Core/Timer.cpp \
$POCKET_PATH/Core/Window.cpp \
\
$POCKET_PATH/Data/BezierCurve1.cpp \
$POCKET_PATH/Data/BezierCurve3.cpp \
$POCKET_PATH/Data/ICellItem.cpp \
$POCKET_PATH/Data/IPointable.cpp \
$POCKET_PATH/Data/Octree.cpp \
$POCKET_PATH/Data/Pointer.cpp \
$POCKET_PATH/Data/StringHelper.cpp \
$POCKET_PATH/Data/Triangulator.cpp \
\
$POCKET_PATH/Logic/Animation/TouchAnimator.cpp \
$POCKET_PATH/Logic/Animation/TouchAnimatorSystem.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimation.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimationDatabase.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimator.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimatorSystem.cpp \
\
$POCKET_PATH/Logic/Common/HierarchyOrder.cpp \
$POCKET_PATH/Logic/Common/Orderable.cpp \
$POCKET_PATH/Logic/Effects/ParticleEffect.cpp \
$POCKET_PATH/Logic/Effects/ParticleEmitter.cpp \
$POCKET_PATH/Logic/Effects/ParticleMeshUpdater.cpp \
$POCKET_PATH/Logic/Effects/ParticleUpdaterSystem.cpp \
$POCKET_PATH/Logic/Gui/Font.cpp \
$POCKET_PATH/Logic/Gui/Gui.cpp \
$POCKET_PATH/Logic/Gui/Label.cpp \
$POCKET_PATH/Logic/Gui/LabelMeshSystem.cpp \
$POCKET_PATH/Logic/Gui/Layout/Layoutable.cpp \
$POCKET_PATH/Logic/Gui/Layout/LayoutSystem.cpp \
$POCKET_PATH/Logic/Gui/Menu/Menu.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuAnimator.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuButton.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuButtonSystem.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuSystem.cpp \
$POCKET_PATH/Logic/Gui/Sizeable.cpp \
$POCKET_PATH/Logic/Gui/Sprite.cpp \
$POCKET_PATH/Logic/Gui/SpriteMeshSystem.cpp \
$POCKET_PATH/Logic/Gui/SpriteTextureSystem.cpp \
$POCKET_PATH/Logic/Interaction/Touchable.cpp \
$POCKET_PATH/Logic/Interaction/TouchSystem.cpp \
$POCKET_PATH/Logic/Movement/Draggable.cpp \
$POCKET_PATH/Logic/Movement/DraggableSystem.cpp \
$POCKET_PATH/Logic/Movement/FirstPersonMover.cpp \
$POCKET_PATH/Logic/Movement/FirstPersonMoverSystem.cpp \
$POCKET_PATH/Logic/Rendering/Atlas.cpp \
$POCKET_PATH/Logic/Rendering/Camera.cpp \
$POCKET_PATH/Logic/Rendering/CameraSystem.cpp \
$POCKET_PATH/Logic/Rendering/Colorable.cpp \
$POCKET_PATH/Logic/Rendering/ColorSystem.cpp \
$POCKET_PATH/Logic/Rendering/Lights/Light.cpp \
$POCKET_PATH/Logic/Rendering/Material.cpp \
$POCKET_PATH/Logic/Rendering/Mesh.cpp \
$POCKET_PATH/Logic/Rendering/RenderSystem.cpp \
$POCKET_PATH/Logic/Rendering/Terrain.cpp \
$POCKET_PATH/Logic/Rendering/TerrainSystem.cpp \
$POCKET_PATH/Logic/Rendering/TextureComponent.cpp \
$POCKET_PATH/Logic/Rendering/TextureSystem.cpp \
$POCKET_PATH/Logic/Rendering/VisibleObject.cpp \
$POCKET_PATH/Logic/Selection/ClickSelectorSystem.cpp \
$POCKET_PATH/Logic/Selection/DragSelector.cpp \
$POCKET_PATH/Logic/Selection/Selectable.cpp \
$POCKET_PATH/Logic/Selection/SelectableCollection.cpp \
$POCKET_PATH/Logic/Selection/SelectableDragSystem.cpp \
$POCKET_PATH/Logic/Selection/SelectionVisualizer.cpp \
$POCKET_PATH/Logic/Spatial/OctreeSystem.cpp \
$POCKET_PATH/Logic/Spatial/Transform.cpp \
$POCKET_PATH/Logic/Spatial/TransformHierarchy.cpp \
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
$POCKET_PATH/Math/Quaternion.cpp \
$POCKET_PATH/Math/Ray.cpp \
$POCKET_PATH/Math/TransitionHelper.cpp \
$POCKET_PATH/Math/Vector2.cpp \
$POCKET_PATH/Math/Vector3.cpp \
\
$POCKET_PATH/Packing/RectPacker.cpp \
\
$POCKET_PATH/Platform/Emscripten/FileReader.cpp \
$POCKET_PATH/Platform/Emscripten/WindowWeb.cpp \
$POCKET_PATH/Platform/Emscripten/GamePadManager.cpp \
\
$POCKET_PATH/PngLib/lodepng.cpp \
\
$POCKET_PATH/Profiler/Profiler.cpp \
\
$POCKET_PATH/Rendering/Clipper.cpp \
$POCKET_PATH/Rendering/Colour.cpp \
$POCKET_PATH/Rendering/Shaders/Lit/ShaderLitUncoloredTextured.cpp \
$POCKET_PATH/Rendering/Shaders/Lit/ShaderLitUncoloredTexturedBatched.cpp \
$POCKET_PATH/Rendering/Shaders/Lit/ShaderTexturedLighting.cpp \
$POCKET_PATH/Rendering/Shaders/Shader.cpp \
$POCKET_PATH/Rendering/Shaders/ShaderParticleSystem.cpp \
$POCKET_PATH/Rendering/Shaders/ShaderProgram.cpp \
$POCKET_PATH/Rendering/Shaders/Terrain/ShaderTerrain.cpp \
$POCKET_PATH/Rendering/Shaders/Unlit/ShaderUnlitUncolored.cpp \
$POCKET_PATH/Rendering/Shaders/Unlit/ShaderUnlitUncoloredTextured.cpp \
$POCKET_PATH/Rendering/Texture.cpp \
$POCKET_PATH/Rendering/TextureAtlas.cpp \
$POCKET_PATH/Rendering/TexturePacker.cpp \
$POCKET_PATH/Rendering/Vertex.cpp \
$POCKET_PATH/Rendering/VertexRenderer.cpp \
\
main.cpp \
\
\

#missing logic folders, ie audio may take a while
#-I $POCKET_PATH/Logic/Audio \
#-I $POCKET_PATH/Logic/Physics \


