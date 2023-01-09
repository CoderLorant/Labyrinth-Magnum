import CollisionDetector;

bool hitBoxesCollide(RectangleHitBox h1, RectangleHitBox h2)
{
	const auto coord1 = h1.getMagnumCoordinates();
	const auto coord2 = h2.getMagnumCoordinates();

	if (coord1.rightX.value < coord2.leftX.value
		|| coord2.rightX.value < coord1.leftX.value) {
		return false;
	}

	if (coord1.bottomY.value > coord2.topY.value
		|| coord2.bottomY.value > coord1.topY.value) {
		return false;
	}

	return true;
}