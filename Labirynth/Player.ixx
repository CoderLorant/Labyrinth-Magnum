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
	Player(Magnum::Vector2i playerSizeInPixels, Magnum::Vector2 startPositionInGridCoord, float startSpeed, GridSystem grid);
    void draw();
    void subscribeMovingDirection(MovingDirection direction);
    void unsubscribeMovingDirection(MovingDirection direction);
private: 

    struct Sine { float value; };
    struct Cosine { float value; };
    struct TrigonometricAngle { Sine sine; Cosine cosine; };

    void moveIfInMotion();
    void checkCollisionDetectionWithScreenBorder();
    void validateInMotion();
    void calculateCombinedDirections();
    void calculateDefaultDirection();
    void calculateBorders();
    void moveToEdge(MovingDirection direction);
    GridSystem grid;
    /// <summary>
    /// this is the middle position of the player within -1,1 cartisian coordinate system
    /// </summary>
    Magnum::Vector2 playerMiddleMagnumPosition;
    Magnum::Matrix3 playerScale;
    GL::Mesh squareMesh;
    Shaders::Flat2D squareShader;
    float speed;
    float topY;
    float bottomY;
    float leftX;
    float rightX;
    float magnumHeight;
    float magnumWidth;
    float scaleX;
    float scaleY;
    bool inMotion = false;
    int movingAngle;
    bool rightPressed = false;
    bool leftPressed = false;
    bool upPressed = false;
    bool downPressed = false;

    TrigonometricAngle trigo0{ Sine(0.f), Cosine(1.f) };
    TrigonometricAngle trigo45{ Sine(0.707106f), Cosine(0.707106f) };
    TrigonometricAngle trigo90{ Sine(1.f), Cosine(0) };
    TrigonometricAngle trigo135{ Sine(0.707106f), Cosine(-0.707106f) };
    TrigonometricAngle trigo180{ Sine(0.f), Cosine(-1.f) };
    TrigonometricAngle trigo225{ Sine(-0.707106f), Cosine(-0.707106f) };
    TrigonometricAngle trigo270{ Sine(-1.f), Cosine(0.f) };
    TrigonometricAngle trigo315{ Sine(-0.707106f), Cosine(0.707106f) };
};


