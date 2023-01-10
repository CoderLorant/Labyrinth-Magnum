import WinHandler;

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

#include <Corrade/Utility/Debug.h>

void WinHandler::draw() {

}

void WinHandler::callbackWinFunction() {
	Corrade::Utility::Debug{} << "You win";
}