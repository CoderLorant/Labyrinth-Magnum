module;

#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/SceneGraph/MatrixTransformation2D.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Listener.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/PlayableGroup.h>
#include <Magnum/Audio/Source.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cone.h>
#include <Magnum/Primitives/Cylinder.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/Trade/MeshData.h>

export module SoundHandler;

using namespace Magnum;
using namespace Corrade;
using namespace Math::Literals;

typedef SceneGraph::Scene<SceneGraph::MatrixTransformation2D> Scene2D;
typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D> Object2D;

export class SoundHandler {
public:
    SoundHandler();
private:
	Audio::Context ALSettings;
	Scene2D scene;
	Object2D object{ &scene };
	Audio::Buffer buffer;
	Audio::PlayableGroup2D playableGroup;
	Audio::Playable2D playable;
};