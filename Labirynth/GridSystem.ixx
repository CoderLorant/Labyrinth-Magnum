module;

#include <Magnum/Math/Vector2.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix3.h>

#include <utility>

export module GridSystem;

export import RectangleCoordinates;

using namespace Magnum;

export class GridSystem {

public:
	GridSystem();
	GridSystem(std::pair<int, int> screenSize, std::pair<int, int> gridSystemSize);

	float calculateXFromGridToMagnum(float gridCoordX);
	float calculateYFromGridToMagnum(float gridCoordY);

	std::pair<float, float> calculateXYFromPixelToGrid(std::pair<int, int> pixelCoords);
	std::pair<float, float> calculateXYFromPixelToMagnum(std::pair<int, int> pixelCoords);
	std::pair<float, float> calculateXYFromMagnumToGrid(std::pair<float, float> Coords1x1);
	std::pair<float, float> calculateXYFromGridToMagnum(std::pair<float, float> gridCoords);

	RectangleCoordinates calculateRectangleCoordsFromGridToMagnum(const RectangleCoordinates& gridCoords);

	std::pair<float, float> getSizeScaleFromPixelToMagnum(std::pair<int, int> pixelSize);

	std::pair<float, float> calculateSizeFromPixelToGrid(std::pair<int, int> pixelSize);
	std::pair<float, float> calculateSizeFromPixelToMagnum(std::pair<int, int> pixelSize);

private:
	int xAxisSize;
	int yAxisSize;
	std::pair<int, int> screenSize;
};