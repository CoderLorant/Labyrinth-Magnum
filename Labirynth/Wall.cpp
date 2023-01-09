#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;
using namespace Magnum::Platform;

import Wall;

Wall::Wall(RectangleCoordinates rectMagnumCoordinates) {
	magnumCoords = rectMagnumCoordinates;
	hitBox = magnumCoords;
	Vector2 middlePos{ rectMagnumCoordinates.getMiddleX(), rectMagnumCoordinates.getMiddleY() };
	float width = rectMagnumCoordinates.getWidth();
	float height = rectMagnumCoordinates.getHeight();
	Magnum::Vector2 scaleVector = { width/2, height/2 };
	transformationMatrix = Matrix3::translation(middlePos) * Matrix3::scaling(scaleVector);

	rectangleMesh = MeshTools::compile(Primitives::squareSolid());
}

RectangleHitBox Wall::getHitBox() {
	return hitBox;
}

void Wall::draw() {
	rectangleShader.setTransformationProjectionMatrix(transformationMatrix)
		.setColor(0xff9900_rgbf)
		.draw(rectangleMesh);
}
