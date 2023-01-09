export module TrigonometricAngle;

export{
	struct Sine { float value; };
	struct Cosine { float value; };
	struct TrigonometricAngle { Sine sine; Cosine cosine; };

	TrigonometricAngle trigo0{ Sine(0.f), Cosine(1.f) };
	TrigonometricAngle trigo45{ Sine(0.707106f), Cosine(0.707106f) };
	TrigonometricAngle trigo90{ Sine(1.f), Cosine(0) };
	TrigonometricAngle trigo135{ Sine(0.707106f), Cosine(-0.707106f) };
	TrigonometricAngle trigo180{ Sine(0.f), Cosine(-1.f) };
	TrigonometricAngle trigo225{ Sine(-0.707106f), Cosine(-0.707106f) };
	TrigonometricAngle trigo270{ Sine(-1.f), Cosine(0.f) };
	TrigonometricAngle trigo315{ Sine(-0.707106f), Cosine(0.707106f) };
}
