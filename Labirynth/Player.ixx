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

import GridSystem;

export enum class MovingDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
export class Player {
public:
	Player(Magnum::Vector2i playerSizeInPixels, Magnum::Vector2 startPosition, float startSpeed, Magnum::Vector2i screenSize, 
            GridSystem grid);
    void draw();
    void subscribeMovingDirection(MovingDirection direction);
    void unsubscribeMovingDirection(MovingDirection direction);
private: 
    void moveIfInMotion();
    void checkCollisionDetectionWithScreenBorder();
    void validateInMotion();
    void calculateCombinedDirections();
    void calculateDefaultDirection();
    GridSystem grid;
    /// <summary>
    /// this is the middle position of the player within -1,1 cartisian coordinate system
    /// </summary>
    Magnum::Vector2 unscaledPlayerMiddlePosition;
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
    bool inMotion = false;
    int movingAngle;
    bool rightPressed = false;
    bool leftPressed = false;
    bool upPressed = false;
    bool downPressed = false;
    float sin0 = 0.f;
    float cos0 = 1.f;
    float sin45 = 0.707106f;
    float cos45 = 0.707106f;
    float sin90 = 1.f;
    float cos90 = 0.f;
    float sin135 = 0.707106f;
    float cos135 = -0.707106f;
    float sin180 = 0.f;
    float cos180 = -1.f;
    float sin225 = -0.707106f;
    float cos225 = -0.707106f;
    float sin270 = -1.f;
    float cos270 = 0.f;
    float sin315 = -0.707106f;
    float cos315 = 0.707106f;
};


