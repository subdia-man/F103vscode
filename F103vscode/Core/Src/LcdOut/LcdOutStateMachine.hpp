
enum LcdOutState {
	SystemLoad,
	FilesView, 
	MenuView,
	PassOut,
};

class LcdOutStateMachine {

	LcdOutState _currentState = SystemLoad; 

public:

	LcdOutStateMachine() {}
	~LcdOutStateMachine() {}
	
	void SetState(LcdOutState state) {
		this -> _currentState = state;
	}

	LcdOutState GetState() {
		return this -> _currentState;
	}
};