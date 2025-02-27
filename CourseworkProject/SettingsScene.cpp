#include "stdafx.h"

#include "SettingsScene.h"

#include "MainMenuScene.h"
#include "GameResources.h"

#include <fstream>

void scene::SettingsScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);

	titleLabel = tgui::Label::create("Settings");
	titleLabel->setPosition("(&.w - w) / 2", "2%");
	titleLabel->setTextSize(50);
	titleLabel->ignoreMouseEvents(true);
	titleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(titleLabel, "titleLabel");

	backgroundPanel = tgui::Panel::create();
	backgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(backgroundPanel, "backgroundPanel");

	resolutionWidthComboBox = tgui::ComboBox::create();
	resolutionWidthComboBox->addItem("480");
	resolutionWidthComboBox->addItem("640");
	resolutionWidthComboBox->addItem("1280");
	resolutionWidthComboBox->addItem("1600");
	resolutionWidthComboBox->addItem("1920");
	resolutionWidthComboBox->addItem("2560");
	resolutionWidthComboBox->setRenderer(guiTheme->getRenderer("combobox"));
	guiManager->add(resolutionWidthComboBox, "resolutionWidthComboBox");

	resolutionWidthLabel = tgui::Label::create("Width: ");
	resolutionWidthLabel->setTextSize(resolutionWidthComboBox->getTextSize());
	resolutionWidthLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Bottom);
	resolutionWidthLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(resolutionWidthLabel, "resolutionWidthLabel");

	resolutionHeightLabel = tgui::Label::create("Height: ");
	resolutionHeightLabel->setTextSize(resolutionWidthComboBox->getTextSize());
	resolutionHeightLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Bottom);
	resolutionHeightLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(resolutionHeightLabel, "resolutionHeightLabel");

	resolutionHeightComboBox = tgui::ComboBox::create();
	resolutionHeightComboBox->addItem("320");
	resolutionHeightComboBox->addItem("480");
	resolutionHeightComboBox->addItem("720");
	resolutionHeightComboBox->addItem("900");
	resolutionHeightComboBox->addItem("1080");
	resolutionHeightComboBox->addItem("1440");
	resolutionHeightComboBox->setRenderer(guiTheme->getRenderer("combobox"));
	guiManager->add(resolutionHeightComboBox, "resolutionHeightComboBox");

	fullscreenCheckBox = tgui::CheckBox::create("Fullscreen");
	fullscreenCheckBox->setChecked(engine::fullscreen);
	fullscreenCheckBox->setTextSize(0U);
	fullscreenCheckBox->setRenderer(guiTheme->getRenderer("checkbox"));
	guiManager->add(fullscreenCheckBox, "fullscreenCheckBox");

	saveAndReturnButton = tgui::Button::create("Save and Return");
	saveAndReturnButton->setTextSize(0U);
	saveAndReturnButton->setRenderer(guiTheme->getRenderer("button"));
	saveAndReturnButton->connect("pressed", [&]() {
		std::string resolutionWidthString = resolutionWidthComboBox->getSelectedItem().toAnsiString();
		std::string resolutionHeightString = resolutionHeightComboBox->getSelectedItem().toAnsiString();
		std::ofstream fileStream(resources::PATH_SETTINGSFILE);

		if (fileStream.good()) {
			fileStream << "{" << std::endl;
			engine::JSONArrayBuilder settingsArrayBuilder;

			settingsArrayBuilder.append(std::string("  \"resolution_width\": ") + resolutionWidthString);
			settingsArrayBuilder.append(std::string("  \"resolution_height\": ") + resolutionHeightString);
			settingsArrayBuilder.append(std::string("  \"antialiasing_level\": 2"));
			settingsArrayBuilder.append(std::string("  \"fullscreen\": " + std::to_string(fullscreenCheckBox->isChecked())));

			settingsArrayBuilder.flush(fileStream);
			fileStream << "}" << std::endl;
		}

		fileStream.close();
		unsigned int newWidth = static_cast<unsigned int>(std::stoi(resolutionWidthString));
		unsigned int newHeight = static_cast<unsigned int>(std::stoi(resolutionHeightString));

		delete engine::renderWindow;
		unsigned int windowStyle = fullscreenCheckBox->isChecked() ? sf::Style::Fullscreen : sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar;
		
		//Create new window with the settings selected.
		engine::renderWindow = new sf::RenderWindow(sf::VideoMode(newWidth, newHeight), resources::STRING_WINDOWTITLE, windowStyle, sf::ContextSettings(0U, 0U, 8U, 2U, 0U, sf::ContextSettings::Attribute::Default, false));
		engine::fullscreen = fullscreenCheckBox->isChecked();
		engine::setCurrentScene(new scene::MainMenuScene(), true);
	});
	guiManager->add(saveAndReturnButton, "saveAndReturnButton");

	returnButton = tgui::Button::create("Return");
	returnButton->setTextSize(0U);
	returnButton->setRenderer(guiTheme->getRenderer("button"));
	returnButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::MainMenuScene(), true);
	});
	guiManager->add(returnButton, "returnButton");

	std::string currentResolutionWidthString = std::to_string(static_cast<int>(renderTarget->getDefaultView().getSize().x));
	if (!resolutionWidthComboBox->contains(currentResolutionWidthString)) {
		resolutionWidthComboBox->addItem(currentResolutionWidthString);
	}

	resolutionWidthComboBox->setSelectedItem(currentResolutionWidthString);

	std::string currentResolutionHeightString = std::to_string(static_cast<int>(renderTarget->getDefaultView().getSize().y));
	if (!resolutionHeightComboBox->contains(currentResolutionHeightString)) {
		resolutionHeightComboBox->addItem(currentResolutionHeightString);
	}

	resolutionHeightComboBox->setSelectedItem(currentResolutionHeightString);

	backgroundPanel->setSize("70% + 10", "60% + 25");
	backgroundPanel->setPosition("(&.w - w) / 2", "(&.h - h) / 2");

	resolutionWidthLabel->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + 5");
	resolutionWidthComboBox->setSize(0.F, "7%");
	resolutionWidthComboBox->setTextSize(0U);
	resolutionWidthComboBox->setSize(tgui::Text::getLineWidth("9999", tgui::getGlobalFont(), resolutionWidthComboBox->getTextSize()) + resolutionWidthComboBox->getSize().y + 10, "7%");

	resolutionWidthLabel->setTextSize(resolutionWidthComboBox->getTextSize());
	resolutionWidthComboBox->setPosition("resolutionWidthLabel.x + resolutionWidthLabel.w", "backgroundPanel.y + 5");

	resolutionHeightLabel->setPosition("resolutionWidthComboBox.x + resolutionWidthComboBox.w + 5", "resolutionWidthComboBox.y");
	resolutionHeightLabel->setTextSize(resolutionWidthComboBox->getTextSize());
	
	resolutionHeightComboBox->setPosition("resolutionHeightLabel.x + resolutionHeightLabel.w", "resolutionHeightLabel.y");
	resolutionHeightComboBox->setSize("resolutionWidthComboBox.w", "7%");
	resolutionHeightComboBox->setTextSize(0U);

	fullscreenCheckBox->setPosition("backgroundPanel.x + 5", "resolutionHeightComboBox.y + resolutionHeightComboBox.h + 5");
	fullscreenCheckBox->setSize(0.F, "7%");
	fullscreenCheckBox->setSize("h", "7%");

	saveAndReturnButton->setSize("(backgroundPanel.w - 15) * (3 / 5)", "7%");
	saveAndReturnButton->setPosition("(backgroundPanel.x + backgroundPanel.w) - w - 5", "backgroundPanel.y + backgroundPanel.h - 5 - h");

	returnButton->setSize("(backgroundPanel.w - 15) * (2 / 5)", "7%");
	returnButton->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + backgroundPanel.h - h - 5");
}

void scene::SettingsScene::deactivated(sf::RenderTarget * renderTarget) {
	titleLabel.reset();
	backgroundPanel.reset();
	resolutionWidthLabel.reset();
	resolutionWidthComboBox.reset();
	resolutionHeightLabel.reset();
	resolutionHeightComboBox.reset();
	fullscreenCheckBox.reset();
	saveAndReturnButton.reset();
	returnButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}

