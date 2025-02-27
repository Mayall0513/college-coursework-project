#include "stdafx.h"

#include "qjson.h"
#include "GameResources.h"
#include "Scene.h"
#include "MainMenuScene.h"
#include "WindowsFunctions.h"
#include "SFML\GpuPreference.hpp"

#include <filesystem>
#include <fstream>

SFML_DEFINE_DISCRETE_GPU_PREFERENCE; //Tell the graphics driver we want to use a discete graphics card if there is one.

constexpr int EXITCODE_SUCCESS = 0;
constexpr int EXTICODE_FAILURE_NOSHADERS = 1;
constexpr int EXITCODE_FAILURE_NOVERTEXBUFFERS = 2;
constexpr int EXITCODE_FAILURE_COMMONINITIALIZATION = 3;

constexpr unsigned int ENGINE_TICKSPERSECOND = 64U;

int main() {
	//Shaders and vertex buffers are required to render the game, if we can't use them we can't continue.
	if (!sf::Shader::isAvailable()) {
		win32::createErrorBox("ERROR", "Shaders are not available when they are required! Closing...");
		return EXTICODE_FAILURE_NOSHADERS;
	}

	if (!sf::VertexBuffer::isAvailable()) {
		win32::createErrorBox("ERROR", "Vertex Buffers are not available when they are required! Closing...");
		return EXITCODE_FAILURE_NOVERTEXBUFFERS;
	}

	//Default settings file configuration.
	unsigned int viewWidth = 1280U;
	unsigned int viewHeight = 720U;
	unsigned int antialiasingLevel = 2U;
	bool windowFullscreen = false;

	//Check if A settings file exists.
	if (std::filesystem::exists(resources::PATH_SETTINGSFILE)) {
		std::ifstream fileStream(resources::PATH_SETTINGSFILE); //Open the file

		if (fileStream.good()) { //Was that successful?
			try {
				qjson::value settingsRoot(fileStream); //Read the JSON in the settings file and convert it to a tree

				if (settingsRoot.has("resolution_width")) {
					qjson::value resolutionWidth = settingsRoot["resolution_width"];

					if (resolutionWidth.isNumber()) {
						viewWidth = static_cast<unsigned int>(resolutionWidth.asInt());
					}
				}

				if (settingsRoot.has("resolution_height")) {
					qjson::value resolutionHeight = settingsRoot["resolution_height"];

					if (resolutionHeight.isNumber()) {
						viewHeight = static_cast<unsigned int>(resolutionHeight.asInt());
					}
				}

				if (settingsRoot.has("antialiasing_level")) {
					qjson::value antialiasing = settingsRoot["antialiasing_level"];

					if (antialiasing.isNumber()) {
						antialiasingLevel = static_cast<unsigned int>(antialiasing.asInt());
					}
				}

				if (settingsRoot.has("fullscreen")) {
					qjson::value fullscreen = settingsRoot["fullscreen"];

					if (fullscreen.isBool()) {
						windowFullscreen = fullscreen.asBool();
					}
				}
			}

			catch (std::runtime_error & runtimeError) { //Show an error/message box.
				win32::createErrorBox("ERROR", (std::string("Error when loading settings!\n") + std::string(runtimeError.what()) + std::string("\n Will substitute default settings! Delete the corrupt settings file to generate a new, default one.")).c_str());
			}
		}

		fileStream.close();
	}

	else { //Create a default settings file.
		std::ofstream fileStream(resources::PATH_SETTINGSFILE);
		if (fileStream.good()) {
			engine::writeDefaultSettingsFile(fileStream);
		}

		fileStream.close();
	}

	if (!std::filesystem::exists(resources::PATH_SAVEGAME)) { //If a save game folder does not exist - create it.
		std::filesystem::create_directory(resources::PATH_SAVEGAME);
	}

	unsigned int windowStyle = windowFullscreen ? sf::Style::Fullscreen : sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar;
	engine::renderWindow = new sf::RenderWindow(sf::VideoMode(viewWidth, viewHeight), resources::STRING_WINDOWTITLE, windowStyle, sf::ContextSettings(0U, 0U, antialiasingLevel, 2U, 0U, sf::ContextSettings::Attribute::Default, false));
	engine::fullscreen = windowStyle & sf::Style::Fullscreen;

	resources::commonInitialization(); //Load shaders and art needed to run the game.
	engine::setCurrentScene(new scene::MainMenuScene(), false);

	sf::Event currentEvent;
	sf::Clock mainClock;
	sf::Clock updateClock;

	float targetUpdateFrequency = 1.F / ENGINE_TICKSPERSECOND;
	float updateAccumulator = 0.F; //Scores the time since an update tick was last performed
	bool hasFocus = true;

	while (engine::renderWindow->isOpen() && engine::hasScene()) {
 		while (engine::renderWindow->pollEvent(currentEvent)) {
			switch (currentEvent.type) {
			case sf::Event::Closed:
				engine::renderWindow->close();
				break;

			case sf::Event::GainedFocus:
				hasFocus = true;
				break;

			case sf::Event::LostFocus:
				hasFocus = false;
				break;

			default: {
				if (hasFocus && engine::hasScene()) {
					engine::getCurrentScene()->pumpEvent(currentEvent);
				}
			}
			}
		}

		while (updateAccumulator >= targetUpdateFrequency && engine::hasScene()) { 
			//If it has been long enough since we last updated - send an update tick to the current scene with the time since the last update.
			engine::getCurrentScene()->update(updateClock.restart().asSeconds());
			updateAccumulator -= targetUpdateFrequency;
		}

		if (hasFocus) { //Don't draw if the window doesn't have focus - there's no point.
			if (engine::hasScene()) {
				engine::renderWindow->draw(*engine::getCurrentScene()); //Draw the current scene.
			}

			engine::renderWindow->display();
		}

		updateAccumulator += mainClock.restart().asSeconds();
	}

	free(resources::GAME_ADJACENCYOFFSETS[0U]); //Free up dynamically allocated memory to be safe.
	free(resources::GAME_ADJACENCYOFFSETS[1U]);
	free(resources::GAME_ADJACENCYOFFSETS);

	return EXITCODE_SUCCESS;
}