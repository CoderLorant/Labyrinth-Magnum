#include <Corrade/Utility/DebugStl.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>

#include <vector>
#include <chrono>
import LabyrinthConfig;


using namespace Magnum;
using namespace std;
using namespace Math::Literals;

class GameWindow : public Platform::Application {
public:
    explicit GameWindow(const Arguments& arguments, const Configuration& config, const GLConfiguration& glConfig);

private:
    void drawEvent() override;
    void keyPressEvent(KeyEvent& event) override;

    std::chrono::time_point<std::chrono::system_clock> firstKeyPressDate 
                        = std::chrono::system_clock::now() 
                            - std::chrono::microseconds(10s);
    int exitKeyPressCounter = 0;
    GL::Mesh triangleMesh;
    GL::Mesh squareMesh;
    Shaders::VertexColor2D triangleShader;
    Shaders::Flat2D squareShader;
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

    struct TriangleVertex {
        Vector2 position;
        Color3 color;
    };
    const TriangleVertex meshColoringData[]{
        {{-0.3f, -0.4f}, 0xff0000_rgbf},    // Left vertex, red color (x,y)
        {{ 0.3f, -0.4f}, 0x00ff000_rgbf},    // Right vertex, green color (x,y)
        {{ 0.0f,  0.4f}, 0x0000ff_rgbf}     //Top vertex, blue color (x,y)
    };

    GL::Buffer buffer;
    buffer.setData(meshColoringData);
  //  Trade::MeshData data = …;

  //  GL::Mesh mesh = MeshTools::compile(data);
    squareMesh = MeshTools::compile(Primitives::squareSolid());

    triangleMesh.setCount(3)
        .addVertexBuffer(std::move(buffer), 0,
            Shaders::VertexColor2D::Position{},
            Shaders::VertexColor2D::Color3{});
}

void GameWindow::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
    
    triangleShader.draw(triangleMesh);
    squareShader.setColor(0x329ea8_rgbf).draw(squareMesh);


    swapBuffers();
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
}


int main(int argc, char** argv)
{
    vector<const char*> a{ argv[0], "--magnum-log","verbose" };
    char** argsData = const_cast<char**>(a.data());
    int argsCount = a.size();
    Magnum::Platform::Sdl2Application::Arguments args{ argsCount, argsData };


    Magnum::Platform::Sdl2Application::Configuration config;
    config.setTitle(config::window::title);
 //   config.addWindowFlags(Magnum::Platform::Sdl2Application::Configuration::WindowFlag::FullscreenDesktop);
    config.setSize(config::window::resolution, config::window::dpiScalingForResolution);


    Magnum::Platform::Sdl2Application::GLConfiguration glConfig;
   

    GameWindow app(args, config, glConfig);
 //   app.setCursor(Magnum::Platform::Sdl2Application::Cursor::Wait);
    int n = app.exec();
 
}