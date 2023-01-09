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

import Player;
import GridSystem;
import TrigonometricAngle;
import CollisionDetector;

Player::Player(Magnum::Vector2i playerSizeInPixels, Magnum::Vector2 startPositionInGridCoord, float startSpeed, GridSystem grid) {

    this->grid = grid;

    // Scale
    auto scale = grid.getSizeScaleFromPixelToMagnum({ playerSizeInPixels[0], playerSizeInPixels[1] });
    scaleX = scale.first;
    scaleY = scale.second;
    Magnum::Vector2 playerScaleVector = { scaleX,scaleY };
    Debug{} << "Scale player with " << scaleX << ":" << scaleY;
    playerScale = Matrix3::scaling(playerScaleVector);
    
    // Middle Position
    auto startMiddlePositionInMagnum = grid.calculateXYFromGridToMagnum({ startPositionInGridCoord[0], startPositionInGridCoord[1] });
    playerMiddleMagnumPosition = { startMiddlePositionInMagnum.first, startMiddlePositionInMagnum.second};
    Debug{} << "Player start position = " << startMiddlePositionInMagnum.first << " : " << startMiddlePositionInMagnum.second;

    // Size
    auto playerMagnumSize = grid.calculateSizeFromPixelToMagnum({ playerSizeInPixels[0], playerSizeInPixels[1] });
    magnumWidth = playerMagnumSize.first;
    magnumHeight = playerMagnumSize.second;
    Debug{} << "Magnum player size: " << magnumHeight << ":" << magnumWidth;
    
    // Sarkok
    calculateBorders();
    Debug{} << "Middle position: " << playerMiddleMagnumPosition.x() << " : " << playerMiddleMagnumPosition.y();
    Debug{} << "Player borders: " << topY << " : " << bottomY << " : " << leftX << " : " << rightX;
    Trade::MeshData squareMeshData = Primitives::squareSolid();
    squareMesh = MeshTools::compile(squareMeshData);
    this->speed = startSpeed;

    playerHitBox = RectangleHitBox{{ TopY(topY), BottomY(bottomY), LeftX(leftX), RightX(rightX) }};

    auto hitBoxCoord = playerHitBox.getMagnumCoordinates();
    Debug{} << "Player hitbox = " << "top(" << hitBoxCoord.topY.value << ") bottom(" << hitBoxCoord.bottomY.value
            << ") left(" << hitBoxCoord.leftX.value << ") right(" << hitBoxCoord.rightX.value << ")";
}

void Player::draw()
{
    moveIfInMotion();
    squareShader.setTransformationProjectionMatrix(Matrix3::translation(playerMiddleMagnumPosition) * playerScale)
                .setColor(0x329ea8_rgbf)
                .draw(squareMesh);
}

void Player::moveIfInMotion()
{
    if (!inMotion) {
        return;
    }
    TrigonometricAngle sinCos = trigo0;
    if (movingAngle == 0) {
        sinCos = trigo0;
    } else if (movingAngle == 45) {
        sinCos = trigo45;
    } else if (movingAngle == 90) {
        sinCos = trigo90;
    } else if (movingAngle == 135) {
        sinCos = trigo135;
    } else if (movingAngle == 180) {
        sinCos = trigo180;
    } else if (movingAngle == 225) {
        sinCos = trigo225;
    } else if (movingAngle == 270) {
        sinCos = trigo270;
    } else if (movingAngle == 315) {
        sinCos = trigo315;
    }
    playerMiddleMagnumPosition += (Magnum::Vector2{ sinCos.cosine.value, sinCos.sine.value }) * speed;
    calculateBorders();
    playerHitBox.updateMagnumCoordinates({TopY(topY), BottomY(bottomY), LeftX(leftX), RightX(rightX) });

    checkCollisionDetectionWithScreenBorder();
    checkCollisionDetectionWithWalls();
}

void Player::checkCollisionDetectionWithScreenBorder()
{
    if (topY > 1.f) {
        moveToEdgeOfLine(1, MovingDirection::DOWN);
    }
    if (bottomY < -1.f) {
        moveToEdgeOfLine(-1, MovingDirection::UP);
    }
    if (leftX < -1.f) {
        moveToEdgeOfLine(-1, MovingDirection::RIGHT);
    }
    if (rightX > 1.f) {
        moveToEdgeOfLine(1, MovingDirection::LEFT);
    }
}

void Player::checkCollisionDetectionWithWalls() {
    auto wallHitbox = rectHitboxes[0];
    bool collisionWithWall = hitBoxesCollide(playerHitBox, wallHitbox);
    if (!collisionWithWall) {
        return;
    }

    const auto wallCoord = wallHitbox.getMagnumCoordinates();

    const float intrusionFromTop = wallCoord.topY.value - bottomY;
    const float intrusionFromBottom = topY - wallCoord.bottomY.value;
    const float intrusionFromRight = wallCoord.rightX.value - leftX;
    const float intrusionFromLeft = rightX - wallCoord.leftX.value;

    MovingDirection minimumIntrusionFrom = MovingDirection::UP;
    float minimumIntrusion = intrusionFromTop;

    if (intrusionFromBottom < minimumIntrusion) {
        minimumIntrusion = intrusionFromBottom;
        minimumIntrusionFrom = MovingDirection::DOWN;
    }
    if (intrusionFromRight < minimumIntrusion) {
        minimumIntrusion = intrusionFromRight;
        minimumIntrusionFrom = MovingDirection::RIGHT;
    }
    if (intrusionFromLeft < minimumIntrusion) {
        minimumIntrusion = intrusionFromLeft;
        minimumIntrusionFrom = MovingDirection::LEFT;
    }

    if (minimumIntrusionFrom == MovingDirection::DOWN) {
        moveToEdgeOfLine(wallCoord.bottomY.value - 0.00001f, MovingDirection::DOWN);
    } else if (minimumIntrusionFrom == MovingDirection::UP) {
        moveToEdgeOfLine(wallCoord.topY.value + 0.00001f, MovingDirection::UP);
    }else if (minimumIntrusionFrom == MovingDirection::RIGHT) {
        moveToEdgeOfLine(wallCoord.rightX.value + 0.00001f, MovingDirection::RIGHT);
    } else if (minimumIntrusionFrom == MovingDirection::LEFT) {
        moveToEdgeOfLine(wallCoord.leftX.value - 0.00001f, MovingDirection::LEFT);
    }

    calculateBorders();
    playerHitBox.updateMagnumCoordinates({ TopY(topY), BottomY(bottomY), LeftX(leftX), RightX(rightX) });
}

void Player::validateInMotion()
{
    // moving direction
    if (upPressed && downPressed && leftPressed && rightPressed) {
        inMotion = false;
        return;
    }
    if (upPressed && downPressed && !leftPressed && !rightPressed) {
        inMotion = false;
    }else if (leftPressed && rightPressed && !upPressed && !downPressed) {
        inMotion = false;
    }else if (!leftPressed && !rightPressed && !upPressed && !downPressed) {
        inMotion = false;
    }else {
        inMotion = true;
    }
}

void Player::calculateCombinedDirections()
{
    if (upPressed && rightPressed && !leftPressed && !downPressed) {
        movingAngle = 45;
    }
    else if (downPressed && rightPressed && !leftPressed && !upPressed) {
        movingAngle = 315;
    }
    else if (upPressed && leftPressed && !rightPressed && !downPressed) {
        movingAngle = 135;
    }
    else if (downPressed && leftPressed && !rightPressed && !upPressed) {
        movingAngle = 225;
    }
}

void Player::calculateDefaultDirection()
{
    if (rightPressed && upPressed == downPressed) {
        movingAngle = 0;
    } else if (upPressed && leftPressed == rightPressed) {
        movingAngle = 90;
    } else if (leftPressed && upPressed == downPressed) {
        movingAngle = 180;
    } else if (downPressed && leftPressed == rightPressed) {
        movingAngle = 270;
    }
}

void Player::calculateBorders()
{
    topY = playerMiddleMagnumPosition.y() + magnumHeight / 2;
    bottomY = playerMiddleMagnumPosition.y() - magnumHeight / 2;
    leftX = playerMiddleMagnumPosition.x() - magnumWidth / 2;
    rightX = playerMiddleMagnumPosition.x() + magnumWidth / 2;
}

void Player::moveToEdgeOfLine(float magnumCoordinate, MovingDirection direction)
{
    switch (direction) {
        case MovingDirection::LEFT:
            playerMiddleMagnumPosition.x() = magnumCoordinate - magnumWidth / 2; break;
        case MovingDirection::RIGHT:
            playerMiddleMagnumPosition.x() = magnumCoordinate + magnumWidth / 2; break;
        case MovingDirection::UP:
            playerMiddleMagnumPosition.y() = magnumCoordinate + magnumHeight / 2; break;
        case MovingDirection::DOWN:
            playerMiddleMagnumPosition.y() = magnumCoordinate - magnumHeight / 2; break;
        default:
            break;
    }
}

void Player::subscribeHitBox(RectangleHitBox rectHitBox) {
    rectHitboxes.push_back(rectHitBox);
    auto wallCoord = rectHitBox.getMagnumCoordinates();
}

void Player::subscribeMovingDirection(MovingDirection direction) {
    if (direction == MovingDirection::RIGHT) {
        rightPressed = true;
    } else if (direction == MovingDirection::UP) {
        upPressed = true;
    } else if (direction == MovingDirection::LEFT) {
        leftPressed = true;
    } else if (direction == MovingDirection::DOWN) {
        downPressed = true;
    }
    validateInMotion();
    if (inMotion) {
        calculateDefaultDirection();
        calculateCombinedDirections();
    }
}

void Player::unsubscribeMovingDirection(MovingDirection direction) {
    if (direction == MovingDirection::RIGHT) {
        rightPressed = false;
    } else if (direction == MovingDirection::UP) {
        upPressed = false;
    } else if (direction == MovingDirection::LEFT) {
        leftPressed = false;
    } else if (direction == MovingDirection::DOWN) {
        downPressed = false;
    }
    validateInMotion();
    calculateDefaultDirection();
    calculateCombinedDirections();
}
