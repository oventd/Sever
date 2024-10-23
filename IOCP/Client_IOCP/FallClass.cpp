#include "Render.cpp"

class Fall {
protected:
	int position_x;   // X ��ǥ
	int position_y;   // Y ��ǥ (�������� ��ġ)
	int cursor_x;     // Ŀ�� X ��ǥ
	int cursor_y;     // Ŀ�� Y ��ǥ
	int game_score;   // ���� ����

	const char* shape;

	Render render;

public:
	Fall(int cx, int cy, int score) : cursor_x(cx), cursor_y(cy), game_score(score) {
		position_x = rand() % 19 + 2;
		position_y = 1;
	}

	virtual void fallStep() = 0; // �ڽ� Ŭ�������� ������ �Լ�

	virtual void resetPosition() {
		clearPreviousPosition();
		BLUE_GREEN render.printAt("��", cursor_x, cursor_y);
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
