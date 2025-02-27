#pragma once

#include "stdafx.h"

#include "SimpleGUIScene.h"

namespace scene {
	class LoadGameScene : public scene::SimpleGUIScene {
	private:
		std::shared_ptr<tgui::Label> titleLabel;
		std::shared_ptr<tgui::Panel> backgroundPanel;

		std::shared_ptr<tgui::ListBox> selectGameListBox;
		std::shared_ptr<tgui::Button> loadSaveButton;
		std::shared_ptr<tgui::Button> deleteSaveButton;
		std::shared_ptr<tgui::Button> mainMenuButton;

	public:
		LoadGameScene() = default;
		virtual ~LoadGameScene() = default;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;
	};
};