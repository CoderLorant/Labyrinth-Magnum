export module CollisionDetector;

export import RectangleHitBox;

export {
    enum class IntrusionDirection {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

	bool hitBoxesCollide(RectangleHitBox h1, RectangleHitBox h2);
    IntrusionDirection predictIntrusionDirection(RectangleHitBox hitBoxCollidedWith, RectangleHitBox hitBoxCollidedFrom);
}
