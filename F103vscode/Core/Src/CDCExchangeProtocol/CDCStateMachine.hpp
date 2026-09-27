
enum CDCState {
	CDCFree,
	CDCCommandAwaiting,
	CDCFileReceiving,
	CDCFileTransmitting
};


class CDCStateMachine {

CDCState currentState = CDCFree; 

public:

	CDCStateMachine() {
		currentState = CDCCommandAwaiting;
	}
	~CDCStateMachine();
	
	void SetState(CDCState state) {
		this -> currentState = state;
	}

	CDCState GetState() {
		return this -> currentState;
	}
};