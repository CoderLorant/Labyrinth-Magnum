module; 
#include <string>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Magnum.h>


using namespace Magnum::Math::Literals;
using namespace Magnum::Platform;
export module LabyrinthConfig;

export namespace config {

	namespace window {
		const std::string title = "Escape from Labyrinth";
		const Magnum::Vector2i resolution = Magnum::Vector2i{ 1920,1080 };
		const Magnum::Vector2 dpiScalingForResolution = Magnum::Vector2{ 1,1 };
		const Magnum::Color4 backgroundColor = 0x1a4a1f_rgbf;
	}

	namespace secretExit {
		const Sdl2Application::KeyEvent::Key exitKey = Sdl2Application::KeyEvent::Key::Esc;
		const long long maximumTimeDurationInMs = 1000;
		const int numberOfKeyPressForApplicationQuit = 2;
	}
}