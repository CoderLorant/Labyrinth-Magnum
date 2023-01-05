#include <Magnum/Math/Vector2.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix3.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

import GridSystem;

GridSystem::GridSystem() {}

GridSystem::GridSystem(std::pair<int, int> screenSize, std::pair<int, int> gridSystemSize) {
	this->screenSize = screenSize;
	xAxisSize = gridSystemSize.first;
	yAxisSize = gridSystemSize.second;

	Debug{} << "Grid system " << xAxisSize << ":" << yAxisSize;
}

std::pair<float, float> GridSystem::calculateXYByPixelCoord(std::pair<int, int> pixelCoords) {
	return { (pixelCoords.first * xAxisSize) / static_cast<float>(screenSize.first) ,
			 (pixelCoords.second * yAxisSize) / static_cast<float>(screenSize.second) };
}

std::pair<float, float> GridSystem::calculateXYBy1x1Coord(std::pair<float, float> Coords1x1) {
	return { ((Coords1x1.first + 1) * xAxisSize) / 2 ,
			 ((Coords1x1.second + 1) * yAxisSize) / 2 };
}

std::pair<float, float> GridSystem::getScaleFromPixelSizeTo1x1(std::pair<int, int> pixelSize) {
	return { pixelSize.first / static_cast<float>(screenSize.first) ,
			 pixelSize.second / static_cast<float>(screenSize.second) };
}

std::pair<float, float> GridSystem::calculateSizeFromPixel(std::pair<int, int> pixelSize) {
	return calculateXYByPixelCoord(pixelSize);
}
