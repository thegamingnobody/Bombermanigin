#include "EventManager.h"

void dae::EventManager::Init()
{
	m_Subject = std::make_unique<Subject>();
}

void dae::EventManager::ProcessEvent(std::unique_ptr<Event> event)
{
   m_Subject->NotifyObservers(*event);
}

void dae::EventManager::ProcessQueue()
{
   while (!m_EventQueue.empty())
   {
       auto event = std::move(m_EventQueue.front());
       m_EventQueue.pop();
       ProcessEvent(std::move(event));
   }
}
//todo: take damage event broadcasting
void dae::EventManager::BroadcastEvent(std::unique_ptr<Event> event)
{
	m_EventQueue.push(std::move(event));
}

void dae::EventManager::AddObserver(Observer& observer, int eventType)
{
	m_Subject->AddObserver(observer, eventType);
}
void dae::EventManager::RemoveObserver(Observer& observer)
{
	m_Subject->RemoveObserver(observer);
}