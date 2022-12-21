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

using namespace Magnum;
using namespace Magnum::Math::Literals;
using namespace Magnum::Platform;

export module Player;

export enum class MovingDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
export class Player {
public:
	Player(Magnum::Vector2i playerSizeInPixels, Magnum::Vector2 startPosition, float startSpeed, Magnum::Vector2i screenSize);
    void draw();
    void startMove(MovingDirection direction);
private: 
    Magnum::Vector2 playerMiddlePosition = { -1,1 };
    Magnum::Matrix3 playerScale;
    GL::Mesh squareMesh;
    Shaders::Flat2D squareShader;
    float speed;
    float topY;
    float bottomY;
    float leftX;
    float rightX;
    float height;
    float width;
    float scaleX;
    float scaleY;

};


