export module RectangleCoordinates;

export {
	struct TopY { float value; };
	struct BottomY { float value; };
	struct LeftX { float value; };
	struct RightX { float value; };

	struct RectangleCoordinates {
		TopY topY; 
		BottomY bottomY; 
		LeftX leftX; 
		RightX rightX;

		float getMiddleX() const { return (rightX.value + leftX.value) / 2; }
		float getMiddleY() const { return (topY.value + bottomY.value) / 2; }
		float getWidth() const { return rightX.value - leftX.value; }
		float getHeight() const { return topY.value - bottomY.value; }
	};
}