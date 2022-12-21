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
    playerMiddlePosition = startPosition;
    height = scaleY * 2;
    width = scaleX * 2;
    Debug{} << "Scale: " << scaleX << " : " << scaleY;
    Debug{} << "Size: " << height << " : " << width;
    
    topY = playerMiddlePosition.y() + height / 2;
    bottomY = playerMiddlePosition.y() - height / 2;
    leftX = playerMiddlePosition.x() - width / 2;
    rightX = playerMiddlePosition.x() + width / 2;

    Debug{} << "MiddlePosition: " << playerMiddlePosition.x() << " : " << playerMiddlePosition.y();

    Debug{} << "Borders: " << topY << " : " << bottomY << " : " << leftX << " : " << rightX;
    Trade::MeshData squareMeshData = Primitives::squareSolid();
    squareMesh = MeshTools::compile(squareMeshData);

    this->speed = startSpeed;
}

void Player::draw()
{
    squareShader.setTransformationProjectionMatrix(playerScale * Matrix3::translation(playerMiddlePosition));
    squareShader.setColor(0x329ea8_rgbf).draw(squareMesh);
}

void Player::startMove(MovingDirection direction)
{ 
    switch (direction)
    {
    case MovingDirection::RIGHT:
        playerMiddlePosition += (Magnum::Vector2{ 1,0 }) * speed;
        break;
    case MovingDirection::LEFT:
        playerMiddlePosition += (Magnum::Vector2{ -1,0 }) * speed;
        break;
    case MovingDirection::DOWN:
        playerMiddlePosition += (Magnum::Vector2{ 0,-1 }) * speed;
        break;
    case MovingDirection::UP:
        playerMiddlePosition += (Magnum::Vector2{ 0,1 }) * speed; 
        break;
    }
    topY = playerMiddlePosition.y() + height / 2;
    bottomY = playerMiddlePosition.y() - height / 2;
    leftX = playerMiddlePosition.x() - width / 2;
    rightX = playerMiddlePosition.x() + width / 2;
    if (topY * scaleY >= 1.f) {
        playerMiddlePosition.y() = 1 / scaleY -  (height / scaleY)/2;
    //    topY = 1.f;
    }
    if (bottomY * scaleY < -1.f) {
        playerMiddlePosition.y() = -1 / scaleY + (height / scaleY) / 2;
     //   bottomY = -1.f;
    }
    if (leftX * scaleX < -1.f) {
        playerMiddlePosition.x() = -1 / scaleX + (width / scaleX) / 2;
     //   leftX = -1.f;
    }
    if (rightX * scaleX > 1.f) {
        playerMiddlePosition.x() = 1 / scaleX - (width / scaleX) / 2;
     //   rightX = 1.f;
    }
    Debug{} << "MiddlePosition: " << playerMiddlePosition.x() << " : " << playerMiddlePosition.y();
    Debug{} << "Borders: " << topY << " : " << bottomY << " : " << leftX << " : " << rightX;
}
