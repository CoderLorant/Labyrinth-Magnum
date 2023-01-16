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
        {TopY(0), BottomY(3), LeftX(2), RightX(3)},
        {TopY(5), BottomY(6), LeftX(2), RightX(6)},
        {TopY(6), BottomY(12), LeftX(4), RightX(5)},
        {TopY(8), BottomY(9), LeftX(0), RightX(2)},
        {TopY(11), BottomY(12), LeftX(2), RightX(4)},
        {TopY(12), BottomY(18), LeftX(2), RightX(3)},
        {TopY(17), BottomY(18), LeftX(3), RightX(7)},
        {TopY(18), BottomY(33), LeftX(6), RightX(7)},
        {TopY(20), BottomY(21), LeftX(2), RightX(9)},
        {TopY(23), BottomY(24), LeftX(0), RightX(4)},
        {TopY(24), BottomY(30), LeftX(3), RightX(4)},
        {TopY(24), BottomY(33), LeftX(0), RightX(1)},
        {TopY(32), BottomY(33), LeftX(1), RightX(7)},
        {TopY(35), BottomY(38), LeftX(2), RightX(3)},
        {TopY(35), BottomY(36), LeftX(3), RightX(10)},
        {TopY(38), BottomY(40), LeftX(5), RightX(6)},
        {TopY(38), BottomY(40), LeftX(8), RightX(9)},
        {TopY(38), BottomY(40), LeftX(11), RightX(12)},
        {TopY(26), BottomY(27), LeftX(7), RightX(9)},
        {TopY(2), BottomY(3), LeftX(5), RightX(13)},
        {TopY(0), BottomY(2), LeftX(9), RightX(10)},
        {TopY(0), BottomY(15), LeftX(18), RightX(19)},
        {TopY(2), BottomY(3), LeftX(15), RightX(18)},
        {TopY(5), BottomY(6), LeftX(10), RightX(18)},
        {TopY(8), BottomY(15), LeftX(7), RightX(8)},
        {TopY(8), BottomY(9), LeftX(10), RightX(16)},
        {TopY(11), BottomY(12), LeftX(8), RightX(13)},
        {TopY(14), BottomY(15), LeftX(5), RightX(16)},
        {TopY(9), BottomY(24), LeftX(15), RightX(16)},
        {TopY(17), BottomY(18), LeftX(9), RightX(25)},
        {TopY(20), BottomY(21), LeftX(11), RightX(15)},
        {TopY(21), BottomY(30), LeftX(11), RightX(12)},
        {TopY(23), BottomY(24), LeftX(9), RightX(11)},
        {TopY(29), BottomY(30), LeftX(9), RightX(11)},
        {TopY(30), BottomY(35), LeftX(9), RightX(10)},
        {TopY(23), BottomY(24), LeftX(16), RightX(32)},
        {TopY(20), BottomY(21), LeftX(18), RightX(25)},
        {TopY(18), BottomY(20), LeftX(24), RightX(25)},
        {TopY(14), BottomY(15), LeftX(19), RightX(23)},
        {TopY(11), BottomY(12), LeftX(21), RightX(23)},
        {TopY(11), BottomY(12), LeftX(25), RightX(27)},
        {TopY(14), BottomY(15), LeftX(25), RightX(31)},
        {TopY(17), BottomY(18), LeftX(27), RightX(28)},
        {TopY(17), BottomY(18), LeftX(28), RightX(34)},
        {TopY(17), BottomY(18), LeftX(36), RightX(38)},
        {TopY(20), BottomY(21), LeftX(28), RightX(41)},
        {TopY(20), BottomY(24), LeftX(31), RightX(32)},
        {TopY(14), BottomY(21), LeftX(40), RightX(41)},
        {TopY(0), BottomY(3), LeftX(21), RightX(22)},
        {TopY(5), BottomY(12), LeftX(21), RightX(22)},
        {TopY(2), BottomY(3), LeftX(22), RightX(26)},
        {TopY(5), BottomY(6), LeftX(22), RightX(31)},
        {TopY(0), BottomY(5), LeftX(28), RightX(29)},
        {TopY(6), BottomY(9), LeftX(24), RightX(25)},
        {TopY(8), BottomY(9), LeftX(25), RightX(26)},
        {TopY(11), BottomY(12), LeftX(27), RightX(29)},
        {TopY(8), BottomY(11), LeftX(28), RightX(29)},
        {TopY(8), BottomY(9), LeftX(29), RightX(36)},
        {TopY(9), BottomY(10), LeftX(31), RightX(32)},
        {TopY(2), BottomY(3), LeftX(29), RightX(31)},
        {TopY(14), BottomY(15), LeftX(31), RightX(36)},
        {TopY(12), BottomY(14), LeftX(31), RightX(32)},
        {TopY(11), BottomY(12), LeftX(34), RightX(44)},
        {TopY(12), BottomY(14), LeftX(34), RightX(35)},
        {TopY(2), BottomY(6), LeftX(33), RightX(34)},
        {TopY(2), BottomY(3), LeftX(34), RightX(39)},
        {TopY(5), BottomY(6), LeftX(34), RightX(36)},
        {TopY(6), BottomY(8), LeftX(35), RightX(36)},
        {TopY(3), BottomY(5), LeftX(38), RightX(39)},
        {TopY(7), BottomY(8), LeftX(36), RightX(42)},
        {TopY(8), BottomY(9), LeftX(37), RightX(38)},
        {TopY(8), BottomY(9), LeftX(41), RightX(42)},
        {TopY(12), BottomY(17), LeftX(37), RightX(38)},
        {TopY(10), BottomY(11), LeftX(39), RightX(40)},
        {TopY(10), BottomY(19), LeftX(43), RightX(44)},
        {TopY(20), BottomY(21), LeftX(41), RightX(47)},
        {TopY(17), BottomY(20), LeftX(46), RightX(47)},
        {TopY(0), BottomY(3), LeftX(41), RightX(42)},
        {TopY(0), BottomY(3), LeftX(44), RightX(45)},
        {TopY(5), BottomY(7), LeftX(41), RightX(42)},
        {TopY(5), BottomY(6), LeftX(42), RightX(58)},
        {TopY(2), BottomY(5), LeftX(47), RightX(48)},
        {TopY(8), BottomY(10), LeftX(44), RightX(45)},
        {TopY(8), BottomY(9), LeftX(45), RightX(53)},
        {TopY(6), BottomY(12), LeftX(48), RightX(49)},
        {TopY(0), BottomY(3), LeftX(50), RightX(51)},
        {TopY(2), BottomY(3), LeftX(51), RightX(58)},
        {TopY(8), BottomY(9), LeftX(55), RightX(60)},
        {TopY(9), BottomY(13), LeftX(55), RightX(56)},
        {TopY(12), BottomY(13), LeftX(56), RightX(58)},
        {TopY(11), BottomY(12), LeftX(57), RightX(58)},
        {TopY(11), BottomY(12), LeftX(46), RightX(52)},
        {TopY(12), BottomY(15), LeftX(46), RightX(47)},
        {TopY(14), BottomY(15), LeftX(47), RightX(52)},
        {TopY(15), BottomY(16), LeftX(51), RightX(56)},
        {TopY(16), BottomY(18), LeftX(51), RightX(52)},
        {TopY(17), BottomY(18), LeftX(47), RightX(51)},
        {TopY(23), BottomY(24), LeftX(32), RightX(41)},
        {TopY(23), BottomY(24), LeftX(43), RightX(58)},
        {TopY(20), BottomY(21), LeftX(49), RightX(53)},
        {TopY(21), BottomY(23), LeftX(52), RightX(53)},
        {TopY(20), BottomY(21), LeftX(55), RightX(60)},
        {TopY(21), BottomY(23), LeftX(55), RightX(56)},
        {TopY(16), BottomY(18), LeftX(55), RightX(56)},
        {TopY(17), BottomY(18), LeftX(56), RightX(60)},
        {TopY(24), BottomY(35), LeftX(37), RightX(38)},
        {TopY(29), BottomY(30), LeftX(38), RightX(45)},
        {TopY(30), BottomY(32), LeftX(40), RightX(41)},
        {TopY(30), BottomY(32), LeftX(42), RightX(43)},
        {TopY(30), BottomY(32), LeftX(44), RightX(45)},
        {TopY(24), BottomY(27), LeftX(40), RightX(41)},
        {TopY(26), BottomY(27), LeftX(41), RightX(48)},
        {TopY(24), BottomY(29), LeftX(50), RightX(51)},
        {TopY(28), BottomY(29), LeftX(51), RightX(54)},
        {TopY(26), BottomY(28), LeftX(53), RightX(54)},
        {TopY(26), BottomY(27), LeftX(54), RightX(56)},
        {TopY(26), BottomY(27), LeftX(58), RightX(60)},
        {TopY(29), BottomY(30), LeftX(56), RightX(60)},
        {TopY(30), BottomY(34), LeftX(56), RightX(57)},
        { TopY(31), BottomY(32), LeftX(57), RightX(58) },
        { TopY(33), BottomY(34), LeftX(57), RightX(58) },
        { TopY(36), BottomY(38), LeftX(58), RightX(60) },
        { TopY(27), BottomY(34), LeftX(47), RightX(48) },
        { TopY(31), BottomY(32), LeftX(48), RightX(54) },
        { TopY(32), BottomY(33), LeftX(53), RightX(54) },
        { TopY(36), BottomY(37), LeftX(44), RightX(56) },
        { TopY(34), BottomY(36), LeftX(50), RightX(51) },
        { TopY(35), BottomY(36), LeftX(53), RightX(54) },
        { TopY(36), BottomY(37), LeftX(55), RightX(56) },
        { TopY(39), BottomY(40), LeftX(52), RightX(53) },
        { TopY(37), BottomY(38), LeftX(49), RightX(50) },
        { TopY(39), BottomY(40), LeftX(46), RightX(47) },
        { TopY(34), BottomY(36), LeftX(44), RightX(45) },
        { TopY(34), BottomY(35), LeftX(38), RightX(44) },
        { TopY(39), BottomY(40), LeftX(42), RightX(44) },
        { TopY(37), BottomY(39), LeftX(41), RightX(42) },
        { TopY(37), BottomY(38), LeftX(37), RightX(41) },
        { TopY(37), BottomY(38), LeftX(30), RightX(35) },
        { TopY(28), BottomY(35), LeftX(12), RightX(13) },
        { TopY(28), BottomY(29), LeftX(13), RightX(15) },
        { TopY(31), BottomY(32), LeftX(13), RightX(16) },
        { TopY(23), BottomY(26), LeftX(14), RightX(15) },
        { TopY(37), BottomY(40), LeftX(14), RightX(15) },
        { TopY(37), BottomY(38), LeftX(15), RightX(19) },
        { TopY(26), BottomY(37), LeftX(18), RightX(19) },
        { TopY(26), BottomY(27), LeftX(17), RightX(18) },
        { TopY(28), BottomY(29), LeftX(17), RightX(18) },
        { TopY(34), BottomY(35), LeftX(15), RightX(18) },
        { TopY(31), BottomY(32), LeftX(19), RightX(25) },
        { TopY(34), BottomY(35), LeftX(21), RightX(31) },
        { TopY(35), BottomY(40), LeftX(21), RightX(22) },
        { TopY(37), BottomY(38), LeftX(24), RightX(28) },
        { TopY(38), BottomY(40), LeftX(27), RightX(28) },
        { TopY(35), BottomY(37), LeftX(30), RightX(31) },
        { TopY(31), BottomY(32), LeftX(27), RightX(37) },
        { TopY(32), BottomY(35), LeftX(33), RightX(34) },
        { TopY(33), BottomY(34), LeftX(34), RightX(35) },
        { TopY(26), BottomY(27), LeftX(21), RightX(26) },
        { TopY(27), BottomY(29), LeftX(21), RightX(22) },
        { TopY(27), BottomY(29), LeftX(25), RightX(26) },
        { TopY(28), BottomY(29), LeftX(24), RightX(25) },
        { TopY(24), BottomY(26), LeftX(23), RightX(24) },
        { TopY(24), BottomY(26), LeftX(32), RightX(33) },
        { TopY(26), BottomY(27), LeftX(28), RightX(35) },
        { TopY(27), BottomY(29), LeftX(28), RightX(29) },
        { TopY(27), BottomY(29), LeftX(31), RightX(32) },
        { TopY(27), BottomY(29), LeftX(34), RightX(35) }
        
    };

    for (const auto& wallGridCoords : wallGridCoordinates) {
        auto wallMagnumCoordinates = grid.calculateRectangleCoordsFromGridToMagnum(wallGridCoords);
        Wall wall{ wallMagnumCoordinates };
        RectangleHitBox wallHitBox = wall.getHitBox();
        player.subscribeWallHitBox(std::move(wallHitBox));
        walls.push_back(std::move(wall));
    }

    // winPoint
    RectangleCoordinates winPointGridCoordinates{ TopY(21), BottomY(23), LeftX(32), RightX(34) };
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