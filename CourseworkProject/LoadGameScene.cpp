#include "stdafx.h"

#include "LoadGameScene.h"

#include "GameResources.h"
#include "GameScene.h"
#include "MainMenuScene.h"

#include <filesystem>

void scene::LoadGameScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);

	titleLabel = tgui::Label::create("Load Game");
	titleLabel->ignoreMouseEvents(true);
	titleLabel->setTextSize(50U);
	titleLabel->setPosition("(&.w - w) / 2", "2%");
	titleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(titleLabel, "titleLabel");

	backgroundPanel = tgui::Panel::create();
	backgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(backgroundPanel, "backgroundPanel");

	selectGameListBox = tgui::ListBox::create();
	selectGameListBox->setRenderer(guiTheme->getRenderer("listbox"));
	selectGameListBox->connect("itemselected", [&]() {
		bool selectedItem = selectGameListBox->getSelectedItemIndex() != -1;

		loadSaveButton->setEnabled(selectedItem);
		deleteSaveButton->setEnabled(selectedItem);
	});
	guiManager->add(selectGameListBox, "selectGameListBox");

	loadSaveButton = tgui::Button::create("Load Save");
	loadSaveButton->setTextSize(0U);
	loadSaveButton->setEnabled(false);
	loadSaveButton->setRenderer(guiTheme->getRenderer("button"));
	loadSaveButton->connect("pressed", [&]() {
		game::GameScene * gameScene = new game::GameScene();
		std::wstring saveLocation = selectGameListBox->getSelectedItem().toWideString(); //Needs to be cached - call to scene::deactivated() by engine::setCurrentScene() deallocates selectGameListBox.
		engine::setCurrentScene(gameScene, false);

		if (!gameScene->loadGame(saveLocation)) {
			engine::setCurrentScene(this, true);
		}

		else {
			delete this;
		}
	});
	guiManager->add(loadSaveButton, "loadSaveButton");

	deleteSaveButton = tgui::Button::create("Delete Save");
	deleteSaveButton->setTextSize(0U);
	deleteSaveButton->setEnabled(false);
	deleteSaveButton->setRenderer(guiTheme->getRenderer("button"));
	deleteSaveButton->connect("pressed", [&]() {
		std::filesystem::remove(resources::PATH_SAVEGAME / selectGameListBox->getSelectedItem().toWideString());
		selectGameListBox->removeItemByIndex(selectGameListBox->getSelectedItemIndex());

		loadSaveButton->setEnabled(false);
		deleteSaveButton->setEnabled(false);
	});
	guiManager->add(deleteSaveButton, "deleteSaveButton");

	mainMenuButton = tgui::Button::create("Back To Main Menu");
	mainMenuButton->setTextSize(0U);
	mainMenuButton->setRenderer(guiTheme->getRenderer("button"));
	mainMenuButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::MainMenuScene(), true);
	});
	guiManager->add(mainMenuButton, "mainMenuButton");

	for (const std::filesystem::directory_entry & directory : std::filesystem::directory_iterator(resources::PATH_SAVEGAME)) {
		selectGameListBox->addItem(directory.path().filename().wstring());
	}

	backgroundPanel->setSize("60%", "75%");
	backgroundPanel->setPosition("(&.w - w) / 2", "20%");

	selectGameListBox->setSize("backgroundPanel.w - 10", "backgroundPanel.h - (loadSaveButton.h + mainMenuButton.h + 20)");
	selectGameListBox->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + 5");

	deleteSaveButton->setSize("(backgroundPanel.w - 15) / 2", "7%");
	deleteSaveButton->setPosition("(backgroundPanel.x + backgroundPanel.w) - (w + 5)", "(backgroundPanel.y + backgroundPanel.h) - (h + 5)");

	mainMenuButton->setSize("(backgroundPanel.w - 15) / 2", "7%");
	mainMenuButton->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + backgroundPanel.h - (h + 5)");

	loadSaveButton->setSize("(backgroundPanel.w - 10)", "7%");
	loadSaveButton->setPosition("backgroundPanel.x + 5", "mainMenuButton.y - (h + 5)");
}

void scene::LoadGameScene::deactivated(sf::RenderTarget * renderTarget) {
	titleLabel.reset();
	backgroundPanel.reset();
	selectGameListBox.reset();
	loadSaveButton.reset();
	deleteSaveButton.reset();
	mainMenuButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}
