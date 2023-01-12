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

import LabyrinthConfig;

using namespace Magnum;
using namespace Corrade;
using namespace Math::Literals;

void WinHandler::drawAfterWin() {
	if (!gameWon) { return; }
    _shader.bindVectorTexture(_cache.texture());

    _shader
        .setTransformationProjectionMatrix(
            _projectionRotatingText * _transformationRotatingText)
        .setColor(0x2f83cc_rgbf)
        .setOutlineColor(0xdcdcdc_rgbf)
        .setOutlineRange(0.45f, 0.35f)
        .setSmoothness(textSmoothness / _transformationRotatingText.uniformScaling())
        .draw(_rotatingText);

    if (addTextSmothness) {
        textSmoothness += 0.0001;
    } else {
        textSmoothness -= 0.0001;
    }

    if (textSmoothness > 0.2) {
        addTextSmothness = false;
    } else if (textSmoothness < 0.002) {
        addTextSmothness = true;
    }
}

void WinHandler::initialize() {
#pragma execution_character_set("utf-8")
    /* Load a TrueTypeFont plugin and open the font */
    _font = _manager.loadAndInstantiate("TrueTypeFont");
    if (!_font || !_font->openFile("./NOTMK.ttf", 180.0f))
        Fatal{} << "Cannot open font file";

    /* Glyphs we need to render everything */
    _font->fillGlyphCache(_cache,
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789:-+,.! ");

    std::tie(_rotatingText, std::ignore) = Text::Renderer2D::render(*_font, _cache, 0.2f,
        "You win!",
        _vertices, _indices, GL::BufferUsage::StaticDraw, Text::Alignment::MiddleCenter);
    _transformationRotatingText = Matrix3::rotation(-10.0_degf);

    _projectionRotatingText = Matrix3::projection(
        Vector2::xScale(static_cast<float>(config::window::screenSize[0]) / config::window::screenSize[1]));

    /* Set up premultiplied alpha blending to avoid overlapping text characters
       to cut into each other */
    GL::Renderer::enable(GL::Renderer::Feature::Blending);
    GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One, GL::Renderer::BlendFunction::OneMinusSourceAlpha);
    GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
}

void WinHandler::callbackWinFunction() {
	Corrade::Utility::Debug{} << "You win";
	gameWon = true;
}