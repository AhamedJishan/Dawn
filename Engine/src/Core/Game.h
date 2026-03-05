#pragma once

namespace Dawn
{
	// Base class for the client game logic.
	// The Engine's Application needs a Game instance to drives its lifecycle. 
	// Inherit from this to define specific game rules and state.
	class Game
	{
	public:
		virtual ~Game() {}

		virtual void Start() {}
		virtual void Update(float deltaTime) {}
		virtual void ImGuiRender() {}
	};
}