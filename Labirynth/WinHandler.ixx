module;

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Complex.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>

export module WinHandler;

export class WinHandler {
public:
	void callbackWinFunction();
	void draw();
private:
};