LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

TARGET_PLATFORM := 17
LOCAL_MODULE    := PocketAndroid

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue

LOCAL_CPPFLAGS := -DANDROID -frtti

POCKET := ../../../../Pocket

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/$(POCKET)/ComponentSystem/ \
$(LOCAL_PATH)/$(POCKET)/Core/ \
$(LOCAL_PATH)/$(POCKET)/Data/ \
$(LOCAL_PATH)/$(POCKET)/Debugging/ \
$(LOCAL_PATH)/$(POCKET)/Debugging/ \
$(LOCAL_PATH)/$(POCKET)/Events/ \
$(LOCAL_PATH)/$(POCKET)/Logic/ \
$(LOCAL_PATH)/$(POCKET)/Logic/Animation \
$(LOCAL_PATH)/$(POCKET)/Logic/Animation/Spine \
$(LOCAL_PATH)/$(POCKET)/Logic/Common \
$(LOCAL_PATH)/$(POCKET)/Logic/Effects \
$(LOCAL_PATH)/$(POCKET)/Logic/Gui \
$(LOCAL_PATH)/$(POCKET)/Logic/Gui/Layout \
$(LOCAL_PATH)/$(POCKET)/Logic/Gui/Menu \
$(LOCAL_PATH)/$(POCKET)/Logic/Interaction \
$(LOCAL_PATH)/$(POCKET)/Logic/Movement \
$(LOCAL_PATH)/$(POCKET)/Logic/Rendering \
$(LOCAL_PATH)/$(POCKET)/Logic/Rendering/Lights \
$(LOCAL_PATH)/$(POCKET)/Logic/Selection \
$(LOCAL_PATH)/$(POCKET)/Logic/Spatial \
$(LOCAL_PATH)/$(POCKET)/Math \
$(LOCAL_PATH)/$(POCKET)/OpenGL \
$(LOCAL_PATH)/$(POCKET)/Platform/Android \
$(LOCAL_PATH)/$(POCKET)/PngLib \
$(LOCAL_PATH)/$(POCKET)/Profiler \
$(LOCAL_PATH)/$(POCKET)/Rendering \
$(LOCAL_PATH)/$(POCKET)/TinyXml \
$(LOCAL_PATH)/$(POCKET)/Serialization/Json \
/Users/Jeppe/Downloads/android-ndk-r10e/sources/cxx-stl/gnu-libstdc++/4.8/include/bits \

LOCAL_SRC_FILES := \
$(POCKET)/ComponentSystem/GameComponent.cpp \
$(POCKET)/ComponentSystem/GameComponentType.cpp \
$(POCKET)/ComponentSystem/GameFactory.cpp \
$(POCKET)/ComponentSystem/GameObject.cpp \
$(POCKET)/ComponentSystem/GameSystem.cpp \
$(POCKET)/ComponentSystem/GameWorld.cpp \
$(POCKET)/ComponentSystem/SerializedProperty.cpp \
\
$(POCKET)/Core/Engine.cpp \
$(POCKET)/Core/GameManager.cpp \
$(POCKET)/Core/GameState.cpp \
$(POCKET)/Core/InputDevice.cpp \
$(POCKET)/Core/InputManager.cpp \
$(POCKET)/Core/Timer.cpp \
$(POCKET)/Core/Window.cpp \
\
$(POCKET)/Data/BezierCurve1.cpp \
$(POCKET)/Data/BezierCurve3.cpp \
$(POCKET)/Data/ICellItem.cpp \
$(POCKET)/Data/IPointable.cpp \
$(POCKET)/Data/Octree.cpp \
$(POCKET)/Data/Pointer.cpp \
$(POCKET)/Data/QuadTree.cpp \
$(POCKET)/Data/StringHelper.cpp \
$(POCKET)/Data/Triangulator.cpp \
\
$(POCKET)/Debugging/TimeMeasurer.cpp \
$(POCKET)/Debugging/UnitTest.cpp \
\
$(POCKET)/Logic/Animation/TouchAnimator.cpp \
$(POCKET)/Logic/Animation/TouchAnimatorSystem.cpp \
$(POCKET)/Logic/Animation/TransformAnimation.cpp \
$(POCKET)/Logic/Animation/TransformAnimationDatabase.cpp \
$(POCKET)/Logic/Animation/TransformAnimator.cpp \
$(POCKET)/Logic/Animation/TransformAnimatorSystem.cpp \
\
$(POCKET)/Logic/Animation/Spine/SpineAnimation.cpp \
$(POCKET)/Logic/Animation/Spine/SpineAnimator.cpp \
$(POCKET)/Logic/Animation/Spine/SpineAnimatorSystem.cpp \
$(POCKET)/Logic/Animation/Spine/SpineFactory.cpp \
$(POCKET)/Logic/Animation/Spine/SpineModel.cpp \
$(POCKET)/Logic/Animation/Spine/SpineSkeleton.cpp \
\
$(POCKET)/Logic/Common/HierarchyOrder.cpp \
$(POCKET)/Logic/Common/Orderable.cpp \
\
$(POCKET)/Logic/Effects/ParticleEffect.cpp \
$(POCKET)/Logic/Effects/ParticleEmitter.cpp \
$(POCKET)/Logic/Effects/ParticleMeshUpdater.cpp \
$(POCKET)/Logic/Effects/ParticleUpdaterSystem.cpp \
\
$(POCKET)/Logic/Gui/Font.cpp \
$(POCKET)/Logic/Gui/Gui.cpp \
$(POCKET)/Logic/Gui/Label.cpp \
$(POCKET)/Logic/Gui/LabelMeshSystem.cpp \
$(POCKET)/Logic/Gui/Sizeable.cpp \
$(POCKET)/Logic/Gui/Sprite.cpp \
$(POCKET)/Logic/Gui/SpriteMeshSystem.cpp \
$(POCKET)/Logic/Gui/SpriteTextureSystem.cpp \
$(POCKET)/Logic/Gui/TextBox.cpp \
$(POCKET)/Logic/Gui/TextBoxLabelSystem.cpp \
$(POCKET)/Logic/Gui/TextBoxSystem.cpp \
$(POCKET)/Logic/Gui/Layout/Layoutable.cpp \
$(POCKET)/Logic/Gui/Layout/LayoutSystem.cpp \
$(POCKET)/Logic/Gui/Menu/Menu.cpp \
$(POCKET)/Logic/Gui/Menu/MenuAnimator.cpp \
$(POCKET)/Logic/Gui/Menu/MenuButton.cpp \
$(POCKET)/Logic/Gui/Menu/MenuButtonSystem.cpp \
$(POCKET)/Logic/Gui/Menu/MenuSystem.cpp \
\
$(POCKET)/Logic/Interaction/Touchable.cpp \
$(POCKET)/Logic/Interaction/TouchableCanceller.cpp \
$(POCKET)/Logic/Interaction/TouchCancelSystem.cpp \
$(POCKET)/Logic/Interaction/TouchSystem.cpp \
\
$(POCKET)/Logic/Movement/Draggable.cpp \
$(POCKET)/Logic/Movement/DraggableMotion.cpp \
$(POCKET)/Logic/Movement/DraggableMotionSystem.cpp \
$(POCKET)/Logic/Movement/DraggableSystem.cpp \
$(POCKET)/Logic/Movement/FirstPersonMover.cpp \
$(POCKET)/Logic/Movement/FirstPersonMoverSystem.cpp \
$(POCKET)/Logic/Movement/Limitable.cpp \
$(POCKET)/Logic/Movement/LimitableSystem.cpp \
$(POCKET)/Logic/Movement/Velocity.cpp \
$(POCKET)/Logic/Movement/VelocitySystem.cpp \
\
$(POCKET)/Logic/Rendering/Atlas.cpp \
$(POCKET)/Logic/Rendering/Camera.cpp \
$(POCKET)/Logic/Rendering/CameraSystem.cpp \
$(POCKET)/Logic/Rendering/Colorable.cpp \
$(POCKET)/Logic/Rendering/ColorSystem.cpp \
$(POCKET)/Logic/Rendering/RenderSystem.cpp \
$(POCKET)/Logic/Rendering/Terrain.cpp \
$(POCKET)/Logic/Rendering/TerrainSystem.cpp \
$(POCKET)/Logic/Rendering/TextureComponent.cpp \
\
$(POCKET)/Logic/Rendering/Lights/Light.cpp \
\
$(POCKET)/Logic/Selection/ClickSelectorSystem.cpp \
$(POCKET)/Logic/Selection/DragSelector.cpp \
$(POCKET)/Logic/Selection/Selectable.cpp \
$(POCKET)/Logic/Selection/SelectableCollection.cpp \
$(POCKET)/Logic/Selection/SelectableDragSystem.cpp \
$(POCKET)/Logic/Selection/SelectionVisualizer.cpp \
\
$(POCKET)/Logic/Spatial/OctreeSystem.cpp \
$(POCKET)/Logic/Spatial/Transform.cpp \
$(POCKET)/Logic/Spatial/TransformHierarchy.cpp \
\
$(POCKET)/Math/BoundingBox.cpp \
$(POCKET)/Math/BoundingFrustum.cpp \
$(POCKET)/Math/Box.cpp \
$(POCKET)/Math/Interpolator.cpp \
$(POCKET)/Math/MathHelper.cpp \
$(POCKET)/Math/Matrix3x3.cpp \
$(POCKET)/Math/Matrix4x4.cpp \
$(POCKET)/Math/Plane.cpp \
$(POCKET)/Math/Point.cpp \
$(POCKET)/Math/Point3.cpp \
$(POCKET)/Math/Quaternion.cpp \
$(POCKET)/Math/Ray.cpp \
$(POCKET)/Math/TransitionHelper.cpp \
$(POCKET)/Math/Vector2.cpp \
$(POCKET)/Math/Vector3.cpp \
\
$(POCKET)/Platform/Android/WindowAndroid.cpp \
\
$(POCKET)/PngLib/jpeg_decoder.cpp \
$(POCKET)/PngLib/lodepng.cpp \
\
$(POCKET)/Profiler/Profiler.cpp \
\
$(POCKET)/Rendering/Clipper.cpp \
$(POCKET)/Rendering/Colour.cpp \
$(POCKET)/Rendering/DeferredBuffers.cpp \
$(POCKET)/Rendering/Shader.cpp \
$(POCKET)/Rendering/Texture.cpp \
$(POCKET)/Rendering/TextureAtlas.cpp \
$(POCKET)/Rendering/TexturePacker.cpp \
$(POCKET)/Rendering/Vertex.cpp \
\
$(POCKET)/Profiler/TinyXml/tinystr.cpp \
$(POCKET)/Profiler/TinyXml/tinyxml.cpp \
$(POCKET)/Profiler/TinyXml/tinyxmlerror.cpp \
$(POCKET)/Profiler/TinyXml/tinyxmlparser.cpp \



include $(BUILD_SHARED_LIBRARY) 

$(call import-module,android/native_app_glue) 
