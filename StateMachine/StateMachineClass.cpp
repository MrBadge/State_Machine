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

	AddTransition(State_Word, State_start, Event_Space, &StateMachineClass::FoundSpaceAndWord);
	AddTransition(State_Word, State_start, Event_Charecter, &StateMachineClass::FoundCharecterAndWord);
	AddTransition(State_Word, State_start, Event_Unknown, &StateMachineClass::FoundWord);
	AddTransition(State_Word, State_Number, Event_Digit, &StateMachineClass::FoundWord);
	AddTransition(State_Word, State_Word, Event_Letter);

	AddTransition(State_Number, State_start, Event_Space, &StateMachineClass::FoundSpaceAndNumber);
	AddTransition(State_Number, State_start, Event_Charecter, &StateMachineClass::FoundCharecterAndNumber);
	AddTransition(State_Number, State_start, Event_Unknown, &StateMachineClass::FoundNumber);
	AddTransition(State_Number, State_MaybeNotIntNumber, Event_Comma);
	AddTransition(State_Number, State_Number, Event_Digit);

	AddTransition(State_MaybeNotIntNumber, State_start, Event_Space, &StateMachineClass::FoundCharecterAndSpace);
	AddTransition(State_MaybeNotIntNumber, State_start, Event_Charecter, &StateMachineClass::FoundDoubleCharecter);
	AddTransition(State_MaybeNotIntNumber, State_start, Event_Unknown, &StateMachineClass::FoundCharecter);
	AddTransition(State_MaybeNotIntNumber, State_NotIntNumber, Event_Digit);

	AddTransition(State_NotIntNumber, State_start, Event_Space, &StateMachineClass::FoundSpaceAndNumber);
	AddTransition(State_NotIntNumber, State_start, Event_Charecter, &StateMachineClass::FoundCharecterAndNumber);
	AddTransition(State_NotIntNumber, State_start, Event_Unknown, &StateMachineClass::FoundNumber);
	AddTransition(State_NotIntNumber, State_NotIntNumber, Event_Digit);
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
			(this->*(transition->action_))();
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

void StateMachineClass::AddTransition(States fromState, States toState, Events event)
{
	Transitions_.push_back(Transition(fromState, event, toState, &StateMachineClass::NothingToDoHere));
}

void StateMachineClass::AddTransition(States fromState, States toState, Events event, Action action)
{
	Transitions_.push_back(Transition(fromState, event, toState, action));
}

StateMachineClass::Transition::Transition(States baseState, Events event, States targetState, Action action)
	:beginState_(baseState), 
	event_(event), 
	goToState_(targetState), 
	action_(action)
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

void StateMachineClass::FoundCharecterAndNumber()
{
	++Charecters_;
	++Numbers_;
}

void StateMachineClass::FoundCharecterAndSpace()
{
	++Charecters_;
	++Spaces_;
}

void StateMachineClass::FoundCharecterAndWord()
{
	++Charecters_;
	++Words_;
}

void StateMachineClass::FoundDoubleCharecter()
{
	Charecters_ += 2;
}

void StateMachineClass::FoundSpaceAndNumber()
{
	++Spaces_;
	++Numbers_;
}

void StateMachineClass::FoundSpaceAndWord()
{
	++Spaces_;
	++Words_;
}

void StateMachineClass::NothingToDoHere()
{

}