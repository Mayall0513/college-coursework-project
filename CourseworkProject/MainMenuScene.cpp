#include "stdafx.h"

#include "MainMenuScene.h"

#include "GameSetupScene.h"
#include "LoadGameScene.h"
#include "SettingsScene.h"

void scene::MainMenuScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);

	titleLabel = tgui::Label::create("Coursework Project");
	titleLabel->setTextSize(50U);
	titleLabel->ignoreMouseEvents(true);
	titleLabel->setPosition("(&.w - w) / 2", "8%");
	titleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(titleLabel, "titleLabel");

	backgroundPanel = tgui::Panel::create();
	backgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(backgroundPanel, "backgroundPanel");

	newGameButton = tgui::Button::create("New Game");
	newGameButton->setTextSize(0U);
	newGameButton->setRenderer(guiTheme->getRenderer("button"));
	newGameButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::GameSetupScene(), true);
	});
	guiManager->add(newGameButton, "newGameButton");

	loadGameButton = tgui::Button::create("Load Game");
	loadGameButton->setTextSize(0U);
	loadGameButton->setRenderer(guiTheme->getRenderer("button"));
	loadGameButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::LoadGameScene(), true);
	});
	guiManager->add(loadGameButton, "loadGameButton");

	settingsButton = tgui::Button::create("Settings");
	settingsButton->setTextSize(0U);
	settingsButton->setRenderer(guiTheme->getRenderer("button"));
	settingsButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::SettingsScene(), true);
	});
	guiManager->add(settingsButton, "settingsButton");

	quitButton = tgui::Button::create("Quit");
	quitButton->setTextSize(0U);
	quitButton->setRenderer(guiTheme->getRenderer("button"));
	quitButton->connect("pressed", [&]() {
		engine::setCurrentScene(nullptr, true);
	});
	guiManager->add(quitButton, "quitButton");

	backgroundPanel->setPosition("(parent.w - w) / 2", "(parent.h - h) / 2");
	backgroundPanel->setSize("60%", "40%");

	newGameButton->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + 5");
	newGameButton->setSize("backgroundPanel.w - 10", "(backgroundPanel.h - 40) / 4");

	loadGameButton->setPosition("backgroundPanel.x + 5", "newGameButton.y + newGameButton.h + 10");
	loadGameButton->setSize("backgroundPanel.w - 10", "(backgroundPanel.h - 40) / 4");

	settingsButton->setPosition("backgroundPanel.x + 5", "loadGameButton.y + loadGameButton.h + 10");
	settingsButton->setSize("backgroundPanel.w - 10", "(backgroundPanel.h - 40) / 4");

	quitButton->setPosition("backgroundPanel.x + 5", "settingsButton.y + settingsButton.h + 10");
	quitButton->setSize("backgroundPanel.w - 10", "(backgroundPanel.h - 40) / 4");
}

void scene::MainMenuScene::deactivated(sf::RenderTarget * renderTarget) {
	titleLabel.reset();
	backgroundPanel.reset();
	newGameButton.reset();
	loadGameButton.reset();
	settingsButton.reset();
	quitButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}