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
    float scaleX = static_cast<float>(playerSizeInPixels.x()) / screenSize.x();
    float scaleY = static_cast<float>(playerSizeInPixels.y()) / screenSize.y();
    Magnum::Vector2 playerScaleVector = { scaleX,scaleY };
    playerScale = Matrix3::scaling(playerScaleVector);

    Trade::MeshData squareMeshData = Primitives::squareSolid();
    squareMesh = MeshTools::compile(squareMeshData);

    this->speed = startSpeed;
}

void Player::draw()
{
    squareShader.setTransformationProjectionMatrix(playerScale * Matrix3::translation(playerPosition));
    squareShader.setColor(0x329ea8_rgbf).draw(squareMesh);
}

void Player::startMove(MovingDirection direction)
{ 
    switch (direction)
    {
    case MovingDirection::RIGHT:
        playerPosition += (Magnum::Vector2{ 1,0 }) * speed;
        break;
    case MovingDirection::LEFT:
        playerPosition += (Magnum::Vector2{ -1,0 }) * speed;
        break;
    case MovingDirection::DOWN:
        playerPosition += (Magnum::Vector2{ 0,-1 }) * speed;
        break;
    case MovingDirection::UP:
        playerPosition += (Magnum::Vector2{ 0,1 }) * speed; 
        break;
    default:
        break;
    }
}
