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

Player::Player(Magnum::Vector2i playerSizeInPixels, Magnum::Vector2 startPosition, float startSpeed, Magnum::Vector2i screenSize){
    scaleX = static_cast<float>(playerSizeInPixels.x()) / screenSize.x();
    scaleY = static_cast<float>(playerSizeInPixels.y()) / screenSize.y();
    Magnum::Vector2 playerScaleVector = { scaleX,scaleY };
    playerScale = Matrix3::scaling(playerScaleVector);
    unscaledPlayerMiddlePosition = startPosition;
    height = scaleY * 2;
    width = scaleX * 2;
    Debug{} << "Scale: " << scaleX << " : " << scaleY;
    Debug{} << "Size: " << height << " : " << width;
    
    topY = unscaledPlayerMiddlePosition.y() + height / 2;
    bottomY = unscaledPlayerMiddlePosition.y() - height / 2;
    leftX = unscaledPlayerMiddlePosition.x() - width / 2;
    rightX = unscaledPlayerMiddlePosition.x() + width / 2;

    Debug{} << "MiddlePosition: " << unscaledPlayerMiddlePosition.x() << " : " << unscaledPlayerMiddlePosition.y();

    Debug{} << "Borders: " << topY << " : " << bottomY << " : " << leftX << " : " << rightX;
    Trade::MeshData squareMeshData = Primitives::squareSolid();
    squareMesh = MeshTools::compile(squareMeshData);

    this->speed = startSpeed;
}

void Player::draw()
{
    moveIfInMotion();
    squareShader.setTransformationProjectionMatrix(playerScale * Matrix3::translation(unscaledPlayerMiddlePosition))
                .setColor(0x329ea8_rgbf)
                .draw(squareMesh);
}

void Player::moveIfInMotion()
{
    if (!inMotion) {
        return;
    }
    float x = 0, y = 0;

    if (movingAngle == 0) {
        x = cos0; y = sin0;
    }
    else if (movingAngle == 45) {
        x = cos45; y = sin45;
    }
    else if (movingAngle == 90) {
        x = cos90; y = sin90;
    }
    else if (movingAngle == 135) {
        x = cos135; y = sin135;
    }
    else if (movingAngle == 180) {
        x = cos180; y = sin180;
    }
    else if (movingAngle == 225) {
        x = cos225; y = sin225;
    }
    else if (movingAngle == 270) {
        x = cos270; y = sin270;
    }
    else if (movingAngle == 315) {
        x = cos315; y = sin315;
    }
    unscaledPlayerMiddlePosition += (Magnum::Vector2{ x,y }) * speed;
    checkCollisionDetectionWithScreenBorder();
}

void Player::checkCollisionDetectionWithScreenBorder()
{
    topY = unscaledPlayerMiddlePosition.y() + (height / scaleY) / 2;
    bottomY = unscaledPlayerMiddlePosition.y() - (height / scaleY) / 2;
    leftX = unscaledPlayerMiddlePosition.x() - (width / scaleX) / 2;
    rightX = unscaledPlayerMiddlePosition.x() + (width / scaleX) / 2;
    if (topY * scaleY > 1.f) {
        unscaledPlayerMiddlePosition.y() = 1 / scaleY - (height / scaleY) / 2;
    }
    if (bottomY * scaleY < -1.f) {
        unscaledPlayerMiddlePosition.y() = -1 / scaleY + (height / scaleY) / 2;
    }
    if (leftX * scaleX < -1.f) {
        unscaledPlayerMiddlePosition.x() = -1 / scaleX + (width / scaleX) / 2;
    }
    if (rightX * scaleX > 1.f) {
        unscaledPlayerMiddlePosition.x() = 1 / scaleX - (width / scaleX) / 2;
    }
}

void Player::validateInMotion()
{
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
    if (upPressed && rightPressed) {
        movingAngle = 45;
    }
    else if (downPressed && rightPressed) {
        movingAngle = 315;
    }
    else if (upPressed && leftPressed) {
        movingAngle = 135;
    }
    else if (downPressed && leftPressed) {
        movingAngle = 225;
    }
}

void Player::calculateDefaultDirection()
{
    if (rightPressed) {
        movingAngle = 0;
    }else if (upPressed) {
        movingAngle = 90;
    }else if (leftPressed) {
        movingAngle = 180;
    }else if (downPressed) {
        movingAngle = 270;
    }

}

void Player::subscribeMovingDirection(MovingDirection direction)
{
    if (direction == MovingDirection::RIGHT) {
        rightPressed = true;
    }else if (direction == MovingDirection::UP) {
        upPressed = true;
    }else if (direction == MovingDirection::LEFT) {
        leftPressed = true;
    }else if (direction == MovingDirection::DOWN) {
        downPressed = true;
    }
    validateInMotion();
    calculateDefaultDirection();
    calculateCombinedDirections();
}

void Player::unsubscribeMovingDirection(MovingDirection direction)
{
    if (direction == MovingDirection::RIGHT) {
        rightPressed = false;
    }
    else if (direction == MovingDirection::UP) {
        upPressed = false;
    }
    else if (direction == MovingDirection::LEFT) {
        leftPressed = false;
    }
    else if (direction == MovingDirection::DOWN) {
        downPressed = false;
    }
    validateInMotion();
    calculateDefaultDirection();
    calculateCombinedDirections();
}
