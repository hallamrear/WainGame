#include <HalTec/GameStates.h>
#include <HalTec\StateDirector.h>
#include <HalTec\Game.h>
#include "World.h"
#include "PostState.h"
#include "Credit.h"

int main(int argc, char** argv)
{
	WindowDetails details;
	details.dimensions = Vector2f(1280.0f, 720.0f);
	details.position = Vector2f(200.0f, 200.0f);
	details.title = "Wain Game!";
	
	Game game;

	game.Initialise(argc, argv, details);

	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_PLAYING, new World());
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_1, new PostState());
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_2, new Credit());
	StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYING);

	game.Start();

}