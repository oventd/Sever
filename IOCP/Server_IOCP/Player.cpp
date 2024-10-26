#include "header.h"
#include "Render.cpp"

class Player {
private:
	int x;           // Player's current X position
	int y;           // Player's current Y position
	const char* shape;   // Player's shape for rendering
	const char* hitShape = "¡Ø";
	uint16_t color;      // Player's color
	uint16_t displayColor;      // Player's color
	int life;       // Player's remaining lives
	bool isDead;     // Player's dead status


public:
	Render* render1;


	Player(int startX, int startY, const char* playerShape, uint16_t playerColor, int startLife)
		: x(startX), y(startY), shape(playerShape), color(playerColor), life(startLife), isDead(false) {
		render1 = new Render();
		displayColor = color;
	}

	// Method to render the player at their current position
	void render() const {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		render1->printAt(shape, x, y);
	}

	// Move the player based on key input
	void move() {
		render1->setClear(x, y);
		register int direction;
		direction = _getch();

		switch (direction) {
		case LEFT:
			if (x > 2)
				x -= 2;
			break;
		case RIGHT:
			if (x < 19)
				x += 2;
			break;
		}

	}

	// Check if the player is dead
	bool isHit(int fallingObjX, int fallingObjY) {
		// If the player's position matches the falling object's position, they are dead
		if (x == fallingObjX && y == fallingObjY) {
			life--;
			if (life <= 0) {
				isDead = true;
			}
			return true;
		}
		return false;
	}

	void heal() {
		life++;
		GREEN render1->printAt(shape, x, y);
	}

	void damage() {
		life--;
		RED render1->printAt(hitShape, x, y);
	}

	// Getters
	int getX() const { return x; }
	int getY() const { return y; }
	bool getIsDead() const { return isDead; }
	int getLife() const { return life; }

	// Setters
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setLives(int newLives) { life = newLives; }
};
