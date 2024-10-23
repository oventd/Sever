#include "Render.cpp"

class Fall {
protected:
	int position_x;   // X 좌표
	int position_y;   // Y 좌표 (떨어지는 위치)
	int cursor_x;     // 커서 X 좌표
	int cursor_y;     // 커서 Y 좌표
	int game_score;   // 게임 점수

	const char* shape;

	Render render;

public:
	Fall(int cx, int cy, int score) : cursor_x(cx), cursor_y(cy), game_score(score) {
		position_x = rand() % 19 + 2;
		position_y = 1;
	}

	virtual void fallStep() = 0; // 자식 클래스에서 구현할 함수

	virtual void resetPosition() {
		clearPreviousPosition();
		BLUE_GREEN render.printAt("웃", cursor_x, cursor_y);
		position_y = 1;
	}

	void clearPreviousPosition() {
		if (position_y != 1) {
			render.setClear(position_x, position_y - 1);
		}
	}

	void updatePosition() {
		if (position_y < 19) {
			clearPreviousPosition();
			render.printAt(shape, position_x, position_y);
			position_y++;
		}
		else {
			resetPosition();
		}

	}



	int getPositionX() const { return position_x; }
	int getPositionY() const { return position_y; }
	int& getGameScore() { return game_score; }
};
