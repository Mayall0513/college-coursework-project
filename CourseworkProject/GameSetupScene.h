#pragma once

#include "SimpleGUIScene.h"

namespace scene {
	class GameSetupScene : public scene::SimpleGUIScene {
	private:
		const static char * randomSeedAlphabet;

		std::shared_ptr<tgui::Label> titleLabel;
		std::shared_ptr<tgui::Panel> backgroundPanel;

		std::shared_ptr<tgui::Label> randomSeedLabel;
		std::shared_ptr<tgui::EditBox> seedEditBox;
		std::shared_ptr<tgui::Label> seedLabel; 
		std::string randomSeed;

		std::shared_ptr<tgui::Label> decksLabel;
		std::shared_ptr<tgui::EditBox> decksEditBox;
		std::shared_ptr<tgui::Slider> decksSlider;
		std::string lastDecksString;
		unsigned int selectedDecks;

		std::shared_ptr<tgui::Button> startGameButton;
		std::shared_ptr<tgui::Button> mainMenuButton;

	public:
		GameSetupScene();
		virtual ~GameSetupScene() = default;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;
	};
};