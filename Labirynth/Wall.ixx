module;

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

export module Wall;

using namespace Magnum;
using namespace Magnum::Math::Literals;
using namespace Magnum::Platform;

export import RectangleCoordinates;
export import RectangleHitBox;

export class Wall {
public:
	Wall(RectangleCoordinates rectMagnumCoordinates);
	RectangleHitBox getHitBox();
	void draw();
private:
	GL::Mesh rectangleMesh;
	Shaders::Flat2D rectangleShader;
	Matrix3 transformationMatrix;
	RectangleCoordinates magnumCoords;
	RectangleHitBox hitBox;
};