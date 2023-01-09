#include <Corrade/Utility/DebugStl.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Math/Matrix3.h>
#include <vector>
#include <chrono>

import LabyrinthConfig;
import Player;
import GridSystem;
import Wall;

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

    GridSystem grid{ {config::window::screenSize[0], config::window::screenSize[1]}, config::map::gridSystemSize};

    Wall wall{ {TopY(0.5), BottomY(0.2), LeftX(-0.9), RightX(-0.3)}};

    Player player{ {config::player::playerWidth , config::player::playerHeight }, config::player::startMiddlePositionInGridCoord,
                    config::player::startSpeed, grid };
};

GameWindow::GameWindow(const Arguments& arguments,
    const Configuration& config, const GLConfiguration& glConfig) :
    Platform::Application{ arguments, config, glConfig }
{

   GL::Renderer::setClearColor(config::window::backgroundColor);
  //  GL::Renderer::set(config::window::backgroundColor);

    Debug{} << "Hello! This application is running on"
        << GL::Context::current().version() << "using"
        << GL::Context::current().rendererString();

    RectangleHitBox wallHitBox = wall.getHitBox();

    auto wallCoords = wallHitBox.getMagnumCoordinates();
    Debug{} << "wall hitbox = " << "top(" << wallCoords.topY.value << ") bottom(" << wallCoords.bottomY.value
        << ") left(" << wallCoords.leftX.value << ") right(" << wallCoords.rightX.value << ")";

    player.subscribeHitBox(wallHitBox);
}

void GameWindow::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    wall.draw();
    player.draw();

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