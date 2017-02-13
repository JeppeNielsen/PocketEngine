#!/bin/sh

#  Pocket Engine for web using Emscripten
#
#  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.

EMSCRIPTEN_PATH="/Projects/Emscripten/emscripten/1.35.0"
POCKET_PATH="../../Pocket"

#-s ALLOW_MEMORY_GROWTH=1 \

python $EMSCRIPTEN_PATH/emcc -O3 -s FULL_ES2=1 -s ASSERTIONS=1 -o PocketEngine.html -std=gnu++1y -stdlib=libc++ \
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
$POCKET_PATH/ComponentSystem/FileWorld.cpp \
$POCKET_PATH/ComponentSystem/GameIDHelper.cpp \
$POCKET_PATH/ComponentSystem/GameObject.cpp \
$POCKET_PATH/ComponentSystem/GameObjectHandle.cpp \
$POCKET_PATH/ComponentSystem/GameScene.cpp \
$POCKET_PATH/ComponentSystem/GameSystem.cpp \
$POCKET_PATH/ComponentSystem/GameWorld.cpp \
$POCKET_PATH/Core/Engine.cpp \
$POCKET_PATH/Core/EngineContext.cpp \
$POCKET_PATH/Core/GameState.cpp \
$POCKET_PATH/Core/InputDevice.cpp \
$POCKET_PATH/Core/InputManager.cpp \
$POCKET_PATH/Core/Timer.cpp \
$POCKET_PATH/Core/Window.cpp \
$POCKET_PATH/Data/BezierCurve1.cpp \
$POCKET_PATH/Data/BezierCurve3.cpp \
$POCKET_PATH/Data/Bitset.cpp \
$POCKET_PATH/Data/FileArchive.cpp \
$POCKET_PATH/Data/FileHelper.cpp \
$POCKET_PATH/Data/Guid.cpp \
$POCKET_PATH/Data/ICellItem.cpp \
$POCKET_PATH/Data/Octree.cpp \
$POCKET_PATH/Data/Pointer.cpp \
$POCKET_PATH/Data/QuadTree.cpp \
$POCKET_PATH/Data/StringHelper.cpp \
$POCKET_PATH/Data/Triangulator.cpp \
$POCKET_PATH/Debugging/TimeMeasurer.cpp \
$POCKET_PATH/Debugging/UnitTest.cpp \
$POCKET_PATH/Libs/TinyXml/tinystr.cpp \
$POCKET_PATH/Libs/TinyXml/tinyxml.cpp \
$POCKET_PATH/Libs/TinyXml/tinyxmlerror.cpp \
$POCKET_PATH/Libs/TinyXml/tinyxmlparser.cpp \
$POCKET_PATH/Logic/Animation/TouchAnimatorSystem.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimation.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimationDatabase.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimator.cpp \
$POCKET_PATH/Logic/Animation/TransformAnimatorSystem.cpp \
$POCKET_PATH/Logic/Assets/AssetImporter.cpp \
$POCKET_PATH/Logic/Assets/AssetImporterSystem.cpp \
$POCKET_PATH/Logic/Assets/AssetLoader.cpp \
$POCKET_PATH/Logic/Assets/AssetLoaderSystem.cpp \
$POCKET_PATH/Logic/Assets/AssetManager.cpp \
$POCKET_PATH/Logic/Cloning/CloneVariable.cpp \
$POCKET_PATH/Logic/Cloning/Cloner.cpp \
$POCKET_PATH/Logic/Cloning/ClonerSystem.cpp \
$POCKET_PATH/Logic/Common/HierarchyOrder.cpp \
$POCKET_PATH/Logic/Editor/FieldEditor.cpp \
$POCKET_PATH/Logic/Editor/FieldEditorSystem.cpp \
$POCKET_PATH/Logic/Editor/GameObjectEditor.cpp \
$POCKET_PATH/Logic/Editor/GameObjectEditorSystem.cpp \
$POCKET_PATH/Logic/Editor/GuiFieldEditors.cpp \
$POCKET_PATH/Logic/Editor/HierarchyEditor.cpp \
$POCKET_PATH/Logic/Editor/HierarchyEditorSystem.cpp \
$POCKET_PATH/Logic/Editor/VirtualTreeList.cpp \
$POCKET_PATH/Logic/Editor/VirtualTreeListSpawner.cpp \
$POCKET_PATH/Logic/Editor/VirtualTreeListSpawnerSystem.cpp \
$POCKET_PATH/Logic/Editor/VirtualTreeListSystem.cpp \
$POCKET_PATH/Logic/Effects/ParticleEffect.cpp \
$POCKET_PATH/Logic/Effects/ParticleEmitter.cpp \
$POCKET_PATH/Logic/Effects/ParticleMeshUpdater.cpp \
$POCKET_PATH/Logic/Effects/ParticleUpdaterSystem.cpp \
$POCKET_PATH/Logic/Files/FilePath.cpp \
$POCKET_PATH/Logic/Files/FileSystemListenerSystem.cpp \
$POCKET_PATH/Logic/Gui/DroppableSystem.cpp \
$POCKET_PATH/Logic/Gui/Font.cpp \
$POCKET_PATH/Logic/Gui/FontTextureSystem.cpp \
$POCKET_PATH/Logic/Gui/Gui.cpp \
$POCKET_PATH/Logic/Gui/LabelMeshSystem.cpp \
$POCKET_PATH/Logic/Gui/Layout/LayoutSystem.cpp \
$POCKET_PATH/Logic/Gui/Menu/Menu.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuButton.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuButtonSystem.cpp \
$POCKET_PATH/Logic/Gui/Menu/MenuSystem.cpp \
$POCKET_PATH/Logic/Gui/SlicedQuadMeshSystem.cpp \
$POCKET_PATH/Logic/Gui/SpriteMeshSystem.cpp \
$POCKET_PATH/Logic/Gui/SpriteTextureSystem.cpp \
$POCKET_PATH/Logic/Gui/TextBoxLabelSystem.cpp \
$POCKET_PATH/Logic/Gui/TextBoxSystem.cpp \
$POCKET_PATH/Logic/Input/InputController.cpp \
$POCKET_PATH/Logic/Input/InputMapper.cpp \
$POCKET_PATH/Logic/Input/InputMapperSystem.cpp \
$POCKET_PATH/Logic/Interaction/TouchCancelSystem.cpp \
$POCKET_PATH/Logic/Interaction/TouchSystem.cpp \
$POCKET_PATH/Logic/Interaction/Touchable.cpp \
$POCKET_PATH/Logic/Movement/DraggableMotion.cpp \
$POCKET_PATH/Logic/Movement/DraggableMotionSystem.cpp \
$POCKET_PATH/Logic/Movement/DraggableSystem.cpp \
$POCKET_PATH/Logic/Movement/FirstPersonMover.cpp \
$POCKET_PATH/Logic/Movement/FirstPersonMoverSystem.cpp \
$POCKET_PATH/Logic/Movement/Limitable.cpp \
$POCKET_PATH/Logic/Movement/LimitableSystem.cpp \
$POCKET_PATH/Logic/Movement/VelocitySystem.cpp \
$POCKET_PATH/Logic/Rendering/Atlas.cpp \
$POCKET_PATH/Logic/Rendering/Camera.cpp \
$POCKET_PATH/Logic/Rendering/ColorSystem.cpp \
$POCKET_PATH/Logic/Rendering/Colorable.cpp \
$POCKET_PATH/Logic/Rendering/Lights/Light.cpp \
$POCKET_PATH/Logic/Rendering/RenderSystem.cpp \
$POCKET_PATH/Logic/Rendering/ShaderComponent.cpp \
$POCKET_PATH/Logic/Rendering/SlicedTexture.cpp \
$POCKET_PATH/Logic/Rendering/TextureComponent.cpp \
$POCKET_PATH/Logic/Selection/ClickSelectorSystem.cpp \
$POCKET_PATH/Logic/Selection/DragSelector.cpp \
$POCKET_PATH/Logic/Selection/Selectable.cpp \
$POCKET_PATH/Logic/Selection/SelectableCollection.cpp \
$POCKET_PATH/Logic/Selection/SelectableDragSystem.cpp \
$POCKET_PATH/Logic/Selection/SelectedColorer.cpp \
$POCKET_PATH/Logic/Selection/SelectedColorerSystem.cpp \
$POCKET_PATH/Logic/Selection/SelectionVisualizer.cpp \
$POCKET_PATH/Logic/Spatial/Transform.cpp \
$POCKET_PATH/Logic/Spatial/TransformHierarchy.cpp \
$POCKET_PATH/Logic/Switching/Switch.cpp \
$POCKET_PATH/Logic/Switching/SwitchEnabler.cpp \
$POCKET_PATH/Logic/Switching/SwitchEnablerSystem.cpp \
$POCKET_PATH/Logic/Switching/SwitchSystem.cpp \
$POCKET_PATH/Logic/Switching/TouchSwitchSystem.cpp \
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
$POCKET_PATH/Packing/RectPacker.cpp \
$POCKET_PATH/Platform/Emscripten/AppMenu.cpp \
$POCKET_PATH/Platform/Emscripten/File.cpp \
$POCKET_PATH/Platform/Emscripten/FileReader.cpp \
$POCKET_PATH/Platform/Emscripten/GamePadManager.cpp \
$POCKET_PATH/Platform/Emscripten/WindowWeb.cpp \
$POCKET_PATH/Profiler/Profiler.cpp \
$POCKET_PATH/Rendering/Clipper.cpp \
$POCKET_PATH/Rendering/Colour.cpp \
$POCKET_PATH/Rendering/ImageLoader.cpp \
$POCKET_PATH/Rendering/Shader.cpp \
$POCKET_PATH/Rendering/ShaderCollection.cpp \
$POCKET_PATH/Rendering/Texture.cpp \
$POCKET_PATH/Rendering/TextureAtlas.cpp \
$POCKET_PATH/Rendering/Vertex.cpp \
$POCKET_PATH/Serialization/TypeIndexList.cpp \
$POCKET_PATH/Social/FacebookClient.cpp \
$POCKET_PATH/Social/FacebookUser.cpp \
$POCKET_PATH/Web/FileDownloader.cpp \
$POCKET_PATH/Web/HttpRequest.cpp \
$POCKET_PATH/Libs/Zip/miniz.c \
\
main.cpp \
\
\

#missing logic folders, ie audio may take a while
#-I $POCKET_PATH/Logic/Audio \
#-I $POCKET_PATH/Logic/Physics \


