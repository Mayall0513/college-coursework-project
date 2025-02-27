#pragma once

#include "stdafx.h"

#include "SimpleGUIScene.h"

#include "GameScene.h"

#include <filesystem>

namespace scene {
	class SaveScene : public scene::SimpleGUIScene {
	private:
		game::GameScene * gameScene;
		std::filesystem::path saveGamePath;

		std::shared_ptr<tgui::Label> titleLabel;

		std::shared_ptr<tgui::Panel> backgroundPanel;
		std::shared_ptr<tgui::Label> saveNameLabel;
		std::shared_ptr<tgui::EditBox> saveNameEditBox;
		std::shared_ptr<tgui::Button> saveGameButton;

		std::shared_ptr<tgui::Label> errorLabel;
		std::shared_ptr<tgui::Button> returnToGameButton;
		std::shared_ptr<tgui::Button> mainMenuButton;

	public:
		inline SaveScene(game::GameScene * gameScene) { this->gameScene = gameScene; }
		virtual ~SaveScene() = default;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;
	};
};