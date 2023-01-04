module; 
#include <string>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Magnum.h>
#include <Windows.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;
using namespace Magnum::Platform;
export module LabyrinthConfig;

//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
const int screenWidth = GetSystemMetrics(0);
const int screenHeight = GetSystemMetrics(1);

export namespace config {

	namespace window {
		const std::string title = "Escape from Labyrinth";
		const Magnum::Vector2i screenSize = Magnum::Vector2i{ screenWidth,screenHeight };
		const Magnum::Vector2 dpiScalingForResolution = Magnum::Vector2{ 1,1 };
		const Magnum::Color4 backgroundColor = 0x1a4a1f_rgbf;
	}

	namespace secretExit {
		const Sdl2Application::KeyEvent::Key exitKey = Sdl2Application::KeyEvent::Key::Esc;
		const long long maximumTimeDurationInMs = 1000;
		const int numberOfKeyPressForApplicationQuit = 2;
	}
	namespace player {
		const int playerWidth = 30;
		const int playerHeight = 30;
		const Sdl2Application::KeyEvent::Key moveUpKey = Sdl2Application::KeyEvent::Key::W;
		const Sdl2Application::KeyEvent::Key moveLeftKey = Sdl2Application::KeyEvent::Key::A;
		const Sdl2Application::KeyEvent::Key moveDownKey = Sdl2Application::KeyEvent::Key::S;
		const Sdl2Application::KeyEvent::Key moveRightKey = Sdl2Application::KeyEvent::Key::D;
		const Magnum::Vector2 startMiddlePosition = { 0,0 };
		const float startSpeed = 0.03f;
	}
}