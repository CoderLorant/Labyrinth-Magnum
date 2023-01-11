import RectangleHitBox;

RectangleHitBox::RectangleHitBox() {}

RectangleHitBox::RectangleHitBox(RectangleCoordinates hitBoxMagnumCoordinates) {
	magnumCoordinates = hitBoxMagnumCoordinates;
}

RectangleCoordinates RectangleHitBox::getMagnumCoordinates() const {
	return magnumCoordinates;
}

void RectangleHitBox::updateMagnumCoordinates(RectangleCoordinates rectangleMagnumCoordinates) {
	magnumCoordinates = rectangleMagnumCoordinates;
}
