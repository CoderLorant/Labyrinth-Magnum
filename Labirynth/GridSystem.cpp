#include <Magnum/Math/Vector2.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix3.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

import GridSystem;

// TODO delete this row
import RectangleCoordinates;

GridSystem::GridSystem() {}

GridSystem::GridSystem(std::pair<int, int> screenSize, std::pair<int, int> gridSystemSize) {
	this->screenSize = screenSize;
	xAxisSize = gridSystemSize.first;
	yAxisSize = gridSystemSize.second;

	Debug{} << "Grid system " << xAxisSize << ":" << yAxisSize;
}

float GridSystem::calculateXFromGridToMagnum(float gridCoordX) {
	return ((gridCoordX * 2) / static_cast<float>(xAxisSize)) - 1;
}

float GridSystem::calculateYFromGridToMagnum(float gridCoordY) {
	return ((gridCoordY * -2) / static_cast<float>(yAxisSize)) + 1;
}

std::pair<float, float> GridSystem::calculateXYFromPixelToGrid(std::pair<int, int> pixelCoords) {
	return { (pixelCoords.first * xAxisSize) / static_cast<float>(screenSize.first) ,
			 (pixelCoords.second * yAxisSize) / static_cast<float>(screenSize.second) };
}

std::pair<float, float> GridSystem::calculateXYFromPixelToMagnum(std::pair<int, int> pixelCoords)
{
	return { ((pixelCoords.first * 2) / static_cast<float>(screenSize.first)) -1,
			 ((pixelCoords.second * -2) / static_cast<float>(screenSize.second)) + 1};
}

std::pair<float, float> GridSystem::calculateXYFromMagnumToGrid(std::pair<float, float> Coords1x1) {
	return { ((Coords1x1.first + 1) * xAxisSize) / 2 ,
			 ((Coords1x1.second - 1) * yAxisSize) / -2 };
}

std::pair<float, float> GridSystem::calculateXYFromGridToMagnum(std::pair<float, float> gridCoords) {
	return { calculateXFromGridToMagnum(gridCoords.first), calculateYFromGridToMagnum(gridCoords.second)};
}

RectangleCoordinates GridSystem::calculateRectangleCoordsFromGridToMagnum(const RectangleCoordinates& gridCoords) {
	return RectangleCoordinates({TopY(calculateYFromGridToMagnum(gridCoords.topY.value)), 
								 BottomY(calculateYFromGridToMagnum(gridCoords.bottomY.value)),
								 LeftX(calculateXFromGridToMagnum(gridCoords.leftX.value)),
								 RightX(calculateXFromGridToMagnum(gridCoords.rightX.value))
	});
}

std::pair<float, float> GridSystem::getSizeScaleFromPixelToMagnum(std::pair<int, int> pixelSize) {
	return { pixelSize.first / static_cast<float>(screenSize.first) ,
			 pixelSize.second / static_cast<float>(screenSize.second) };
}

std::pair<float, float> GridSystem::calculateSizeFromPixelToGrid(std::pair<int, int> pixelSize) {
	return calculateXYFromPixelToGrid(pixelSize);
}

std::pair<float, float> GridSystem::calculateSizeFromPixelToMagnum(std::pair<int, int> pixelSize)
{
	return { (pixelSize.first * 2) / static_cast<float>(screenSize.first),
			 (pixelSize.second * 2) / static_cast<float>(screenSize.second) };
}
