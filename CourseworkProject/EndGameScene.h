#pragma once

#include "stdafx.h"

#include "SimpleGUIScene.h"

#include "GameScene.h"

namespace scene {
	class EndGameScene : public scene::SimpleGUIScene {
	private:
		game::GameScene * gameScene;

		std::shared_ptr<tgui::Label> titleLabel;
		std::shared_ptr<tgui::Panel> backgroundPanel;

		std::shared_ptr<tgui::Label> turnCountLabel;
		std::shared_ptr<tgui::Label> totalScoreLabel;
		std::shared_ptr<tgui::Label> averageScoreLabel;
		std::shared_ptr<tgui::Label> wellDoneLabel;

		std::shared_ptr<tgui::Button> replayGameButton;
		std::shared_ptr<tgui::Button> gotoMainMenuButton;

	public:
		inline EndGameScene(game::GameScene * gameScene) { this->gameScene = gameScene; }
		virtual ~EndGameScene() = default;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;
	};
};