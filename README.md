# Bombermanigin

Bombermanigin is c++ game engine based on Minigin made for the programming 4 course at Digital Arts & Entertainment.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Features/Choices

Input happens through 'actions'. Register an action in the inputmanager by giving it a gamepad button or keyboard key to watch, when to trigger it, and a command class that gets executed when triggered.

Using the event manager, you can send events to other components or gameobjects. To do this you need to:
1) Make the class where you want to receive the event also an "observer" and override its notify function.
2) Register the (observer) instance to the eventmanager with an event ID. If an event with that ID gets pushed to the event manager, all instances that are subscribed to it will receive it.

Every event ID obviously has its own event type. The engine uses int for these, but making an enum is recommended to more easily differentiate between event types.

To push an event, you make an event object. The event class can be inherited from to allow for passing parameters in/with the event.

Each gameobject has a list of components that it possesses, but every gameobject also has a "transform" component by default. This is stored seperatle from the other components and is accesible through the "GetTransform" function.
