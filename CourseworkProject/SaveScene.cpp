#include "SaveScene.h"

#include "MainMenuScene.h"
#include "GameResources.h"

#include <fstream>

void scene::SaveScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);

	titleLabel = tgui::Label::create("Save Game");
	titleLabel->setTextSize(50U);
	titleLabel->ignoreMouseEvents(true);
	titleLabel->setPosition("(&.w - w) / 2", "8%");
	titleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(titleLabel, "titleLabel");

	backgroundPanel = tgui::Panel::create();
	backgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(backgroundPanel, "backgroundPanel");

	saveNameEditBox = tgui::EditBox::create();
	saveNameEditBox->setMaximumCharacters(16U);
	saveNameEditBox->setRenderer(guiTheme->getRenderer("editbox"));
	saveNameEditBox->connect("textchanged", [&]() { //Checks if a entered save name is valid and updates other components as necessary.
		std::wstring text = saveNameEditBox->getText().toWideString();

		if (text.empty()) { //If the edit box is empty we need to disable the save game button.
			errorLabel->setVisible(true);

			saveGameButton->setText("Save Game");
			saveGameButton->setEnabled(false);
		}

		else {
			std::transform(text.begin(), text.end(), text.begin(), ::towlower);//Put the string to the lower case.
			saveGamePath = resources::PATH_SAVEGAME / text;
			bool newFIle = true;

			for (const std::filesystem::directory_entry & directory : std::filesystem::directory_iterator(resources::PATH_SAVEGAME)) { //Check if the file already exists.
				std::wstring saveName = directory.path().filename().wstring();
				std::transform(saveName.begin(), saveName.end(), saveName.begin(), ::towlower);

				if (text == saveName) {
					newFIle = false;
					break;
				}
			}

			if (newFIle) { //Uses a file stream to check if the name is otherwise valid.
				std::ofstream testStream;
				testStream.open(saveGamePath, std::ofstream::out);

				if (testStream.good()) {
					errorLabel->setVisible(false);

					testStream.close();
					std::filesystem::remove(saveGamePath);
					saveGameButton->setEnabled(true);
				}

				else {
					testStream.close();
					saveGameButton->setEnabled(false);
					errorLabel->setVisible(true);
				}

				saveGameButton->setText("Save Game");
			}

			else {
				saveGameButton->setText("Save Game (Overwrite)");
				saveGameButton->setEnabled(true);
			}
		}
	});
	guiManager->add(saveNameEditBox, "saveNameEditBox");

	saveNameLabel = tgui::Label::create("Save Name: ");
	saveNameLabel->ignoreMouseEvents(true);
	saveNameLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Bottom);
	saveNameLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(saveNameLabel, "saveNameLabel");

	returnToGameButton = tgui::Button::create("Return To Game");
	returnToGameButton->setTextSize(0U);
	returnToGameButton->setRenderer(guiTheme->getRenderer("button"));
	returnToGameButton->connect("pressed", [&]() {
		engine::setCurrentScene(gameScene, true);
	});
	guiManager->add(returnToGameButton, "returnToGameButton");

	errorLabel = tgui::Label::create("Save name is invalid");
	errorLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(errorLabel, "errorLabel");

	mainMenuButton = tgui::Button::create("Goto Main Menu");
	mainMenuButton->setTextSize(0U);
	mainMenuButton->setRenderer(guiTheme->getRenderer("button"));
	mainMenuButton->connect("pressed", [&]() {
		delete gameScene;
		engine::setCurrentScene(new scene::MainMenuScene(), true);
	});
	guiManager->add(mainMenuButton, "mainMenuButton");

	saveGameButton = tgui::Button::create("Save Game");
	saveGameButton->setTextSize(0U);
	saveGameButton->setEnabled(false);
	saveGameButton->setRenderer(guiTheme->getRenderer("button"));
	saveGameButton->connect("pressed", [&]() {
		saveGameButton->setText("Save Game (Overwrite)");
		gameScene->saveGame(saveGamePath.filename());
	});
	guiManager->add(saveGameButton, "saveGameButton");

	backgroundPanel->setSize("60%", "75%");
	backgroundPanel->setPosition("(&.w - w) / 2", "20%");

	saveNameLabel->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + 5");
	saveNameEditBox->setSize("backgroundPanel.w - (saveNameLabel.w + 10)", "7%");
	saveNameEditBox->setTextSize(0U);

	saveNameLabel->setTextSize(saveNameEditBox->getTextSize());
	saveNameEditBox->setPosition("saveNameLabel.x + saveNameLabel.w", "backgroundPanel.y + 5");

	returnToGameButton->setSize("(backgroundPanel.w - 15) / 2", "7%");
	returnToGameButton->setPosition("(backgroundPanel.x + backgroundPanel.w) - (w + 5)", "(backgroundPanel.y + backgroundPanel.h) - (h + 5)");

	mainMenuButton->setSize("(backgroundPanel.w - 15) / 2", "7%");
	mainMenuButton->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + backgroundPanel.h - (h + 5)");

	saveGameButton->setSize("backgroundPanel.w - 10", "7%");
	saveGameButton->setPosition("backgroundPanel.x + 5", "mainMenuButton.y - (h + 5)");

	errorLabel->setTextSize(saveGameButton->getTextSize() / 2U);
	errorLabel->setPosition("backgroundPanel.x + backgroundPanel.w - (w + 5)", "saveGameButton.y - (h + 5)");
}

void scene::SaveScene::deactivated(sf::RenderTarget * renderTarget) {
	titleLabel.reset();
	backgroundPanel.reset();
	saveNameLabel.reset();
	saveNameEditBox.reset();
	saveGameButton.reset();
	errorLabel.reset();
	returnToGameButton.reset();
	mainMenuButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}
