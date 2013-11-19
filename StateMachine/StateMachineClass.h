#pragma once

#include <string>
#include <vector>

class StateMachineClass
{
public:
	StateMachineClass();	

	void Process(std::string text);
	size_t Words() const;
	size_t Numbers() const;
	size_t Charecters() const;
	size_t Spaces() const;

	void FoundWord();
	void FoundNumber();
	void FoundCharecter();
	void FoundSpace();
	void FoundCharecterAndWord();
	void FoundSpaceAndWord();
	void FoundCharecterAndNumber();
	void FoundSpaceAndNumber();
	void FoundDoubleCharecter();
	void FoundCharecterAndSpace();	

	enum States
	{
		State_start,
		State_Word,
		State_Number,
		State_NotIntNumber,
		//State_Charecter,
		State_Unknown,
		State_MaybeNotIntNumber
	};

	enum Events
	{
		Event_Space,
		Event_Charecter,
		Event_Digit,
		Event_Letter,
		Event_Comma,
		Event_Unknown
	};

private:
	size_t Words_;
	size_t Numbers_;
	size_t Charecters_;
	size_t Spaces_;

	void ProcessEvent(Events event);
	void NothingToDoHere();

	typedef void (StateMachineClass::*Action)();

	struct Transition
	{
		Transition(States beginState, Events event, States goToState, Action action);

		States beginState_;
		Events event_;
		States goToState_;
		Action action_;
	};

	void AddTransition(States fromState, States toState, Events event);
	void AddTransition(States fromState, States toState, Events event, Action action);

	typedef std::vector<Transition> TransitionsTable;
	TransitionsTable Transitions_;
	States CurrentState_;
};

