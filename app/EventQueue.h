#pragma once

class Event;
class Listener;

#include <queue>
#include <unordered_set>

class EventQueue
{
public:
	EventQueue(const EventQueue &) = delete;
	EventQueue(EventQueue &&) = delete;

	EventQueue &operator=(const EventQueue &) = delete;
	EventQueue &operator=(EventQueue &&) = delete;

	static EventQueue &getInstance();

	void dispatch();
	void send(Event *ev);

	void attach(Listener *listener);
	void detach(Listener *listener);
private:
	EventQueue();
	~EventQueue();

	std::queue<Event *> events_;
	std::unordered_set<Listener *> listeners_;
};