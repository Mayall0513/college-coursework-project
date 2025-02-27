#pragma once

#include "SimpleGUIScene.h"

namespace scene {
	class SettingsScene : public scene::SimpleGUIScene {
	private:
		std::shared_ptr<tgui::Label> titleLabel;
		std::shared_ptr<tgui::Panel> backgroundPanel;

		std::shared_ptr<tgui::Label> resolutionWidthLabel;
		std::shared_ptr<tgui::ComboBox> resolutionWidthComboBox;
		std::shared_ptr<tgui::Label> resolutionHeightLabel;
		std::shared_ptr<tgui::ComboBox> resolutionHeightComboBox;
		std::shared_ptr<tgui::CheckBox> fullscreenCheckBox;

		std::shared_ptr<tgui::Button> saveAndReturnButton;
		std::shared_ptr<tgui::Button> returnButton;

	public:
		SettingsScene() = default;
		virtual ~SettingsScene() = default;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;
	};
};