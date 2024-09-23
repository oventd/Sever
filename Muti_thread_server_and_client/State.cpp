#include <iostream>
#include <conio.h>  // For _kbhit() and _getch()

// Forward declaration
class KeyDetector;

// State interface
class State {
public:
    virtual ~State() {}
    virtual void DoAction();
};

// KeyDetector class which uses states
class KeyDetector {
private:
    State* currentState;

public:
    KeyDetector(State* initialState) : currentState(initialState) {}
    ~KeyDetector() { delete currentState; }

    void setState(State* newState) {
        if (currentState) {
            delete currentState;
        }
        currentState = newState;
    }

    void processInput(char input) {
        currentState->handleInput(this, input);
    }

    bool isExiting() const {
        return dynamic_cast<ExitState*>(currentState) != nullptr;
    }
};

// Concrete state classes
class WaitForInputState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override;
};

class HandleQState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Key 'q' pressed." << std::endl;
        detector->setState(new WaitForInputState());
    }
};

class HandleAState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Key 'a' pressed." << std::endl;
        detector->setState(new WaitForInputState());
    }
};

class HandleWState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Key 'w' pressed." << std::endl;
        detector->setState(new WaitForInputState());
    }
};

class HandleSState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Key 's' pressed." << std::endl;
        detector->setState(new WaitForInputState());
    }
};

class HandleEState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Key 'e' pressed." << std::endl;
        detector->setState(new WaitForInputState());
    }
};

class HandleDState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Key 'd' pressed." << std::endl;
        detector->setState(new WaitForInputState());
    }
};

class ExitState : public State {
public:
    void handleInput(KeyDetector* detector, char input) override {
        std::cout << "Exiting..." << std::endl;
    }
};

void WaitForInputState::handleInput(KeyDetector* detector, char input) {
    switch (input) {
    case 'q':
        detector->setState(new HandleQState());
        break;
    case 'a':
        detector->setState(new HandleAState());
        break;
    case 'w':
        detector->setState(new HandleWState());
        break;
    case 's':
        detector->setState(new HandleSState());
        break;
    case 'e':
        detector->setState(new HandleEState());
        break;
    case 'd':
        detector->setState(new HandleDState());
        break;
    case 'x':
        detector->setState(new ExitState());
        break;
    default:
        // Ignore other keys
        break;
    }
}
