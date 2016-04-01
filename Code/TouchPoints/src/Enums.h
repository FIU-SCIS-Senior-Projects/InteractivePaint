#ifndef ENUMS_H
#define ENUMS_H
#define COLOR_ZERO "White.png"
#define COLOR_ONE "Red.png"
#define COLOR_TWO "Yellow.png"
#define COLOR_THREE "Green.png"
#define COLOR_FOUR "Torquise.png"
#define COLOR_FIVE "Blue.png"
#define COLOR_SIX "Purple.png"
#define COLOR_SEVEN "Orange.png"

#define SHAPE_LINE "LineIcon.png"
#define SHAPE_Circle "Circle.png"
#define SHAPE_Rectangle "Rectangle.png"
#define SHAPE_Triangle "Triangle.png"
#define SHAPE_Filled_Circle "FilledCircle.png"
#define SHAPE_Filled_Rectangle "FilledRectangle.png"
#define SHAPE_Filled_Triangle "FilledTriangle.png"
namespace Shape{
	enum Shape { Line, Circle, Rectangle, Triangle };
}
namespace ourColors{
	enum ourColors{ White, Red, Yellow, Green, Aqua, Blue, Purple, Orange };
}
#endif