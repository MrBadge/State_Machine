#include "stdafx.h"
#include "StateMachineClass.h"
#include <locale>


StateMachineClass::StateMachineClass()
	:Words_(0), 
	Numbers_(0), 
	Charecters_(0), 
	Spaces_(0), 
	CurrentState_(State_start)
{
	AddTransition(State_start, State_start, Event_Unknown);
	AddTransition(State_start, State_start, Event_Space, &StateMachineClass::FoundSpace);
	AddTransition(State_start, State_start, Event_Charecter, &StateMachineClass::FoundCharecter);
	AddTransition(State_start, State_Word, Event_Letter);
	AddTransition(State_start, State_Number, Event_Digit);

	AddTransition(State_Word, State_start, Event_Space, &StateMachineClass::FoundSpace, &StateMachineClass::FoundWord);
	AddTransition(State_Word, State_start, Event_Charecter, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundWord);
	AddTransition(State_Word, State_start, Event_Unknown, &StateMachineClass::FoundWord);
	AddTransition(State_Word, State_Number, Event_Digit, &StateMachineClass::FoundWord);
	AddTransition(State_Word, State_Word, Event_Letter);

	AddTransition(State_Number, State_start, Event_Space, &StateMachineClass::FoundSpace, &StateMachineClass::FoundNumber);
	AddTransition(State_Number, State_start, Event_Charecter, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundNumber);
	AddTransition(State_Number, State_start, Event_Unknown, &StateMachineClass::FoundNumber);
	AddTransition(State_Number, State_MaybeNotIntNumber, Event_Comma);
	AddTransition(State_Number, State_Number, Event_Digit);
	AddTransition(State_Number, State_Word, Event_Letter, &StateMachineClass::FoundNumber);

	AddTransition(State_MaybeNotIntNumber, State_start, Event_Space, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundSpace);
	AddTransition(State_MaybeNotIntNumber, State_start, Event_Charecter, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundCharecter);
	AddTransition(State_MaybeNotIntNumber, State_start, Event_Unknown, &StateMachineClass::FoundCharecter);
	AddTransition(State_MaybeNotIntNumber, State_NotIntNumber, Event_Digit);
	AddTransition(State_MaybeNotIntNumber, State_Word, Event_Letter, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundNumber);

	AddTransition(State_NotIntNumber, State_start, Event_Space, &StateMachineClass::FoundSpace, &StateMachineClass::FoundNumber);
	AddTransition(State_NotIntNumber, State_start, Event_Charecter, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundNumber);
	AddTransition(State_NotIntNumber, State_start, Event_Unknown, &StateMachineClass::FoundNumber);
	AddTransition(State_NotIntNumber, State_NotIntNumber, Event_Digit);
	AddTransition(State_NotIntNumber, State_Word, Event_Letter, &StateMachineClass::FoundCharecter, &StateMachineClass::FoundNumber);
}

void StateMachineClass::ProcessEvent(Events event)
{
	for (TransitionsTable::const_iterator transition = Transitions_.begin(), end = Transitions_.end();
		transition != end; 
		++transition)
	{
		if (transition->beginState_ == CurrentState_ && transition->event_ == event)
		{
			CurrentState_ = transition->goToState_;
			if (transition->action_ != NULL)
				(this->*(transition->action_))();
			if (transition->AddAction_ != NULL)
				(this->*(transition->AddAction_))();
			return;
		}
	}
}

void StateMachineClass::Process(const std::string text)
{
	const size_t length = text.length();
	for (size_t i = 0; i != length; ++i)
	{
		const char current = text[i];

		if (isspace(current))
		{
			ProcessEvent(Event_Space);
		}
		else if (isdigit(current))
		{
			ProcessEvent(Event_Digit);
		}
		else if (isalpha(current))
		{
			ProcessEvent(Event_Letter);
		}
		else if (current == ',' && CurrentState_ == State_Number)
		{
			ProcessEvent(Event_Comma);
		}
		else if (isprint(current))
		{
			ProcessEvent(Event_Charecter);
		}
		else
		{
			ProcessEvent(Event_Unknown);
		}
	}
	ProcessEvent(Event_Unknown);
}

/*void StateMachineClass::AddTransition(States fromState, States toState, Events event)
{
	Transitions_.push_back(Transition(fromState, event, toState, NULL)); //&StateMachineClass::NothingToDoHere
}*/

void StateMachineClass::AddTransition(States fromState, States toState, Events event, Action action, Action addAction)
{
	Transitions_.push_back(Transition(fromState, event, toState, action, addAction));
}

StateMachineClass::Transition::Transition(States baseState, Events event, States targetState, Action action, Action AdditionalAction)
	:beginState_(baseState), 
	event_(event), 
	goToState_(targetState), 
	action_(action),
	AddAction_(AdditionalAction)
{
}

size_t StateMachineClass::Words() const
{
	return Words_;
}

size_t StateMachineClass::Numbers() const
{
	return Numbers_;
}

size_t StateMachineClass::Charecters() const
{
	return Charecters_;
}

size_t StateMachineClass::Spaces() const
{
	return Spaces_;
}

void StateMachineClass::FoundWord()
{
	++Words_;
}

void StateMachineClass::FoundNumber()
{
	++Numbers_;
}

void StateMachineClass::FoundCharecter()
{
	++Charecters_;
}

void StateMachineClass::FoundSpace()
{
	++Spaces_;
}

void StateMachineClass::NothingToDoHere()
{

}