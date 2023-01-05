module;

#include <Magnum/Math/Vector2.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix3.h>

#include <utility>

export module GridSystem;

using namespace Magnum;

export class GridSystem {

public:
	GridSystem();
	GridSystem(std::pair<int, int> screenSize, std::pair<int, int> gridSystemSize);
	std::pair<float, float> calculateXYByPixelCoord(std::pair<int, int> pixelCoords);
	std::pair<float, float> calculateXYBy1x1Coord(std::pair<float, float> Coords1x1);
	std::pair<float, float> getScaleFromPixelSizeTo1x1(std::pair<int, int> pixelSize);
	std::pair<float, float> calculateSizeFromPixel(std::pair<int, int> pixelSize);

private:
	int xAxisSize;
	int yAxisSize;
	std::pair<int, int> screenSize;
};