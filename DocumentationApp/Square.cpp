#include "Square.h"

Square::Square() : Rectangle(1, 1), sideLength(1) {

}

Square::Square(double sideLength) : Rectangle(sideLength, sideLength), sideLength(sideLength) {

}

Square::Square(double sideLength, glm::vec2 position, glm::vec2 vel, glm::vec2 Force, double Mass, ImVec4 colour, bool stat) :
	Rectangle(sideLength, sideLength, position, vel, Force, Mass, colour, stat), sideLength(sideLength) {


}