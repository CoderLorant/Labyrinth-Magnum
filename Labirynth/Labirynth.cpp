#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Complex.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>

#include <functional>
#include <memory>
#include <vector>
#include <chrono>

import GridSystem;
import LabyrinthConfig;
import Player;
import SoundHandler;
import Wall;
import WinHandler;
import WinPoint;

using namespace Magnum;
using namespace std;
using namespace Math::Literals;

class GameWindow : public Platform::Application {
public:
    explicit GameWindow(const Arguments& arguments, const Configuration& config, const GLConfiguration& glConfig);

private:
    void drawEvent() override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;

    std::chrono::time_point<std::chrono::system_clock> firstKeyPressDate 
                        = std::chrono::system_clock::now() 
                            - std::chrono::microseconds(10s);
    int exitKeyPressCounter = 0;

    std::shared_ptr<WinHandler> winHandler = std::make_shared<WinHandler>();
    WinPoint winPoint;
    GridSystem grid{ {config::window::screenSize[0], config::window::screenSize[1]}, config::map::gridSystemSize};
    std::vector<Wall> walls;
    Player player{ {config::player::playerWidth , config::player::playerHeight }, config::player::startMiddlePositionInGridCoord,
                    config::player::startSpeed, grid };

    PluginManager::Manager<Text::AbstractFont> _manager;
    Containers::Pointer<Text::AbstractFont> _font;

    Text::DistanceFieldGlyphCache _cache;
    GL::Mesh _rotatingText{ NoCreate };
    GL::Buffer _vertices, _indices;
    Containers::Pointer<Text::Renderer2D> _dynamicText;
    Shaders::DistanceFieldVector2D _shader;

    Matrix3 _transformationRotatingText,
        _projectionRotatingText,
        _transformationProjectionDynamicText;

    SoundHandler soundHandler;
};

GameWindow::GameWindow(const Arguments& arguments,
    const Configuration& config, const GLConfiguration& glConfig) :
        Platform::Application{ arguments, config, glConfig },
        _cache{ Vector2i{2048}, Vector2i{512}, 22 }
{

   GL::Renderer::setClearColor(config::window::backgroundColor);
  //  GL::Renderer::set(config::window::backgroundColor);

    Debug{} << "Hello! This application is running on"
        << GL::Context::current().version() << "using"
        << GL::Context::current().rendererString();

    /* Loop at 60 Hz max */
    /*setSwapInterval(1);
    setMinimalLoopPeriod(16);*/

    winHandler->initialize();

    // wallCoordinates
    std:vector<RectangleCoordinates> wallGridCoordinates{
        {TopY(20), BottomY(44), LeftX(20), RightX(22)},
        {TopY(20), BottomY(22), LeftX(22), RightX(62)},
        {TopY(42), BottomY(44), LeftX(42), RightX(62)},
        {TopY(20), BottomY(44), LeftX(62), RightX(64)},
        {TopY(64), BottomY(66), LeftX(42), RightX(106)},
        {TopY(64), BottomY(126), LeftX(106), RightX(108)},
        {TopY(126), BottomY(128), LeftX(106), RightX(148)}
        
    };

    for (const auto& wallGridCoords : wallGridCoordinates) {
        auto wallMagnumCoordinates = grid.calculateRectangleCoordsFromGridToMagnum(wallGridCoords);
        Wall wall{ wallMagnumCoordinates };
        RectangleHitBox wallHitBox = wall.getHitBox();
        player.subscribeWallHitBox(std::move(wallHitBox));
        walls.push_back(std::move(wall));
    }

    // winPoint
    RectangleCoordinates winPointGridCoordinates{ TopY(130), BottomY(145), LeftX(133), RightX(148) };
    auto winPointMagnumCoordinates = grid.calculateRectangleCoordsFromGridToMagnum(winPointGridCoordinates);
    winPoint.initialize(winPointMagnumCoordinates);
    auto winPointHitBox = winPoint.getHitBox();

    player.subscribeWinPoint(winPointHitBox, std::bind(&WinHandler::callbackWinFunction, winHandler.get()));
    //player.subscribeWinPoint(winPointHitBox, std::bind([]() {}));

    // text
    
}

void GameWindow::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    int n = walls.size();
    for (int i = 0; i < n; ++i) {
        walls[i].draw();
    }

    winPoint.draw();
    player.draw();
    winHandler->drawAfterWin();

    swapBuffers();
    redraw();
}

void GameWindow::keyPressEvent(KeyEvent& event) {
    if (event.key() == config::secretExit::exitKey) {
        auto keyPressDate = std::chrono:: system_clock::now();
        auto timeDurationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(keyPressDate - firstKeyPressDate).count();
        

        if (timeDurationInMs > config::secretExit::maximumTimeDurationInMs) {
            firstKeyPressDate = keyPressDate;
            exitKeyPressCounter = 1;
        } else {
            ++exitKeyPressCounter;
        }

        if (exitKeyPressCounter == config::secretExit::numberOfKeyPressForApplicationQuit) {
            this->exit();
        }
    }
    else if (event.key() == config::player::moveUpKey) {
        player.subscribeMovingDirection(MovingDirection::UP);
    } else if (event.key() == config::player::moveLeftKey) {
        player.subscribeMovingDirection(MovingDirection::LEFT);
    } else if (event.key() == config::player::moveDownKey) {
        player.subscribeMovingDirection(MovingDirection::DOWN);
    } else if (event.key() == config::player::moveRightKey) {
        player.subscribeMovingDirection(MovingDirection::RIGHT);
    }
}

void GameWindow::keyReleaseEvent(KeyEvent& event) {
     if (event.key() == config::player::moveUpKey) {
        player.unsubscribeMovingDirection(MovingDirection::UP);
     } else if (event.key() == config::player::moveLeftKey) {
        player.unsubscribeMovingDirection(MovingDirection::LEFT);
     } else if (event.key() == config::player::moveDownKey) {
        player.unsubscribeMovingDirection(MovingDirection::DOWN);
     } else if (event.key() == config::player::moveRightKey) {
        player.unsubscribeMovingDirection(MovingDirection::RIGHT);
     }
}


int main(int argc, char** argv)
{
    vector<const char*> a{ argv[0], "--magnum-log","verbose" };
    char** argsData = const_cast<char**>(a.data());
    int argsCount = static_cast<int>(a.size());
    Magnum::Platform::Sdl2Application::Arguments args{ argsCount, argsData };


    Magnum::Platform::Sdl2Application::Configuration config;
    config.setTitle(config::window::title);
 //   config.addWindowFlags(Magnum::Platform::Sdl2Application::Configuration::WindowFlag::FullscreenDesktop);
    config.setSize(config::window::screenSize, config::window::dpiScalingForResolution);

    Magnum::Platform::Sdl2Application::GLConfiguration glConfig;
   
    GameWindow app(args, config, glConfig);
 //   app.setCursor(Magnum::Platform::Sdl2Application::Cursor::Wait);
    int n = app.exec();
 
}