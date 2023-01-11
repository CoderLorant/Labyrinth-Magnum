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

#include <functional>
#include <vector>

using namespace Magnum;
using namespace Magnum::Math::Literals;
using namespace Magnum::Platform;

export module Player;

import GridSystem;
import RectangleHitBox;

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
    void subscribeWinPoint(RectangleHitBox rectHitBox, std::function<void()> winFunction);
    void subscribeWallHitBox(RectangleHitBox rectHitBox);
    void subscribeMovingDirection(MovingDirection direction);
    void unsubscribeMovingDirection(MovingDirection direction);
    void pause();
private: 
    void moveIfInMotion();
    void checkCollisionDetectionWithScreenBorder();
    void checkCollisionDetectionWithWalls();
    void checkCollisionDetectionWithWinPoint();
    void validateInMotion();
    void calculateCombinedDirections();
    void calculateDefaultDirection();
    void calculateBorders();
    void moveToEdgeOfLine(float magnumCoordinate, MovingDirection direction);
    void moveToEdgeOfWallByPrediction(const RectangleHitBox& wallHitbox);
    GridSystem grid;
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
    std::vector<RectangleHitBox> rectHitboxes;
    RectangleHitBox playerHitBox;
    RectangleHitBox winPointHitBox;
    std::function<void()> winFunction;
    bool inPause = false;
};


