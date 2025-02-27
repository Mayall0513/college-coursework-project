#pragma once

#include "SimpleGUIScene.h"

namespace scene {
	class MainMenuScene : public scene::SimpleGUIScene {
	private:
		std::shared_ptr<tgui::Label> titleLabel;
		std::shared_ptr<tgui::Panel> backgroundPanel;

		std::shared_ptr<tgui::Button> newGameButton;
		std::shared_ptr<tgui::Button> loadGameButton;
		std::shared_ptr<tgui::Button> settingsButton;
		std::shared_ptr<tgui::Button> quitButton;

	public:
		MainMenuScene() = default;
		virtual ~MainMenuScene() = default;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;
	};
};