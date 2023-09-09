#include "EventQueue.h"
#include "Event.h"
#include "Listener.h"

EventQueue::EventQueue() :
	events_(),
	listeners_()
{}

EventQueue::~EventQueue()
{
	while (!events_.empty())
	{
		delete events_.front();
		events_.pop();
	}
}

EventQueue &EventQueue::getInstance()
{
	static EventQueue instance;
	return instance;
}

void EventQueue::dispatch()
{
	while (!events_.empty())
	{
		const Event &ev = *events_.front();

		for (auto it = listeners_.begin(); it != listeners_.end(); ++it)
		{
			if (!(*it)->accepts(ev.getType()))
			{
				continue;
			}

			(*it)->onEvent(ev);
		}

		delete events_.front();
		events_.pop();
	}
}

void EventQueue::send(Event *ev)
{
	events_.push(ev);
}

void EventQueue::attach(Listener *listener)
{
	listeners_.emplace(listener);
}

void EventQueue::detach(Listener *listener)
{
	listeners_.erase(listener);
}