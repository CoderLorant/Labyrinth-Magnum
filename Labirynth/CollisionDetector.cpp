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

IntrusionDirection predictIntrusionDirection(RectangleHitBox hitBoxCollidedWith, RectangleHitBox hitBoxCollidedFrom) {
	const auto collidedWithCoord = hitBoxCollidedWith.getMagnumCoordinates();
	const auto collidedFromCoord = hitBoxCollidedFrom.getMagnumCoordinates();
	const float intrusionFromTop = collidedWithCoord.topY.value - collidedFromCoord.bottomY.value;
	const float intrusionFromBottom = collidedFromCoord.topY.value - collidedWithCoord.bottomY.value;
	const float intrusionFromRight = collidedWithCoord.rightX.value - collidedFromCoord.leftX.value;
	const float intrusionFromLeft = collidedFromCoord.rightX.value - collidedWithCoord.leftX.value;

	float minimumIntrusion = intrusionFromTop;
	IntrusionDirection minimumIntrusionFrom = IntrusionDirection::UP;

	if (intrusionFromBottom < minimumIntrusion) {
		minimumIntrusion = intrusionFromBottom;
		minimumIntrusionFrom = IntrusionDirection::DOWN;
	}
	if (intrusionFromRight < minimumIntrusion) {
		minimumIntrusion = intrusionFromRight;
		minimumIntrusionFrom = IntrusionDirection::RIGHT;
	}
	if (intrusionFromLeft < minimumIntrusion) {
		minimumIntrusion = intrusionFromLeft;
		minimumIntrusionFrom = IntrusionDirection::LEFT;
	}

	return minimumIntrusionFrom;
}
