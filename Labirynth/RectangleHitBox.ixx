export module RectangleHitBox;

export import RectangleCoordinates;

export class RectangleHitBox {
public:
	RectangleHitBox();
	RectangleHitBox(RectangleCoordinates hitBoxMagnumCoordinates);
	RectangleCoordinates getMagnumCoordinates() const;
	void updateMagnumCoordinates(RectangleCoordinates rectangleMagnumCoordinates);

private:
	RectangleCoordinates magnumCoordinates;
};
