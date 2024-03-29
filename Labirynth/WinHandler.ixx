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

using namespace Corrade;
using namespace Magnum;

export class WinHandler {
public:
    void initialize();
	void callbackWinFunction();
	void drawAfterWin();
private:
	bool gameWon = false;
    float textSmoothness = 0.025f;
    bool addTextSmothness = true;

	PluginManager::Manager<Text::AbstractFont> _manager;
	Containers::Pointer<Text::AbstractFont> _font;
    Text::DistanceFieldGlyphCache _cache{ Vector2i{2048}, Vector2i{512}, 22 };
    GL::Mesh _rotatingText{ NoCreate };
    GL::Buffer _vertices, _indices;
    Shaders::DistanceFieldVector2D _shader;

    Matrix3 _transformationRotatingText,
        _projectionRotatingText;
};