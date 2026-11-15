
enum LcdOutState {
	Deinitted,
	SystemLoad,
	FilesView,
	FileDeleteModalView, 
	MenuView,
	PassOut,
};

class LcdOutStateMachine {

	LcdOutState _currentState = Deinitted; 

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