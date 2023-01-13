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
#include <MagnumPlugins/DrWavAudioImporter/DrWavImporter.h>

using namespace Magnum;
using namespace Corrade;
using namespace Math::Literals;

import SoundHandler;

SoundHandler::SoundHandler(): playable{ object, &playableGroup } {

    PluginManager::Manager<Audio::AbstractImporter> manager;
    Containers::Pointer<Audio::AbstractImporter> oggImporter = manager.loadAndInstantiate("StbVorbisAudioImporter");
    /*if (!oggImporter)
        std::exit(1);
    if (!oggImporter->openFile("./sound/chimes.ogg"))
        std::exit(2);*/

    Containers::Pointer<Audio::AbstractImporter> wavImporter = manager.loadAndInstantiate("WavAudioImporter");
    if (!wavImporter)
        std::exit(1);
    if (!wavImporter->openFile("./sound/guitar.wav"))
        std::exit(2);

    buffer.setData(wavImporter->format(), wavImporter->data(), wavImporter->frequency());

    playable.source()
        .setBuffer(&buffer)
        .setLooping(true)
        .play();
}
