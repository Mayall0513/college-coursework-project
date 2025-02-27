#include "stdafx.h"

#include "GameSetupScene.h"

#include "GameResources.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Scene.h"

#include <random>

const char * scene::GameSetupScene::randomSeedAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";

scene::GameSetupScene::GameSetupScene() {
	unsigned int length = std::uniform_int_distribution<unsigned int>(1U, 64U)(resources::GAME_RANDOMENGINE);
	std::uniform_int_distribution<unsigned int> randomSeedDistribution(0U, 62U);

	randomSeed = std::string(length, 0);
	std::generate_n(randomSeed.begin(), length, [&]() -> char {
		return randomSeedAlphabet[randomSeedDistribution(resources::GAME_RANDOMENGINE)];
	});

	lastDecksString = std::string("8");
	selectedDecks = 8U;
}

void scene::GameSetupScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);

	titleLabel = tgui::Label::create("Game Setup");
	titleLabel->setTextSize(50U);
	titleLabel->ignoreMouseEvents(true);
	titleLabel->setPosition("(&.w - w) / 2", "2%");
	titleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(titleLabel, "titleLabel");

	backgroundPanel = tgui::Panel::create();
	backgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(backgroundPanel, "backgroundPanel");

	randomSeedLabel = tgui::Label::create("Will generate random seed");
	randomSeedLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(randomSeedLabel, "randomSeedLabel");

	seedEditBox = tgui::EditBox::create();
	seedEditBox->setMaximumCharacters(64U);
	seedEditBox->setRenderer(guiTheme->getRenderer("editbox"));
	seedEditBox->connect("textchanged", [&]() {
		randomSeedLabel->setVisible(seedEditBox->getText().isEmpty());
	});
	guiManager->add(seedEditBox, "seedEditBox");

	seedLabel = tgui::Label::create("Seed: ");
	seedLabel->ignoreMouseEvents(true);
	seedLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Bottom);
	seedLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(seedLabel, "seedLabel");

	decksLabel = tgui::Label::create("Deck Count: ");
	decksLabel->ignoreMouseEvents(true);
	decksLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Bottom);
	decksLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(decksLabel, "decksLabel");

	decksSlider = tgui::Slider::create(1.F, 999.F);
	decksSlider->setStep(1.F);
	decksSlider->setValue(8.F);
	decksSlider->setRenderer(guiTheme->getRenderer("slider"));
	decksSlider->connect("valuechanged", [&]() {
		selectedDecks = static_cast<unsigned int>(decksSlider->getValue());
		lastDecksString = std::to_string(selectedDecks);

		decksEditBox->setText(lastDecksString);
	});
	guiManager->add(decksSlider, "decksSlider");

	decksEditBox = tgui::EditBox::create();
	decksEditBox->setTextSize(0U);
	decksEditBox->setMaximumCharacters(3U);
	decksEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	decksEditBox->setText("8");
	decksEditBox->setRenderer(guiTheme->getRenderer("invisibleeditbox"));
	decksEditBox->connect("textchanged", [&]() { //Validate the input - cancelling input if necessary.
		try {
			std::string stdString = decksEditBox->getText().toAnsiString();
			unsigned long turns = std::stoul(stdString);
			std::string stdTurns = std::to_string(turns);

			if (stdString != stdTurns) {
				decksEditBox->setText(stdTurns);

				lastDecksString = stdString;
				selectedDecks = turns;
			}

			decksSlider->setValue(static_cast<float>(turns));
		}

		catch (std::invalid_argument &) {
			decksEditBox->setText("1");
		}

		catch (std::out_of_range &) {
			decksEditBox->setText(lastDecksString);
		}
	});
	guiManager->add(decksEditBox, "decksEditBox");

	startGameButton = tgui::Button::create("Start Game");
	startGameButton->setTextSize(0U);
	startGameButton->setRenderer(guiTheme->getRenderer("button"));
	startGameButton->connect("pressed", [&]() {
		engine::setCurrentScene(new game::GameScene(randomSeedLabel->isVisible() ? randomSeed : seedEditBox->getText().toAnsiString(), static_cast<int>(decksSlider->getValue())), true);
	});
	guiManager->add(startGameButton, "startGameButton");

	mainMenuButton = tgui::Button::create("Back To Main Menu");
	mainMenuButton->setTextSize(0U);
	mainMenuButton->setRenderer(guiTheme->getRenderer("button"));
	mainMenuButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::MainMenuScene(), true);
	});
	guiManager->add(mainMenuButton, "mainMenuButton");

	backgroundPanel->setSize("60%", "75%");
	backgroundPanel->setPosition("(&.w - w) / 2", "20%");

	seedLabel->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + 5");
	seedEditBox->setSize("backgroundPanel.w - (seedLabel.w + 10)", "7%");
	seedEditBox->setTextSize(0U);

	seedLabel->setTextSize(seedEditBox->getTextSize());
	seedEditBox->setPosition("seedLabel.x + seedLabel.w", "backgroundPanel.y + 5");

	decksLabel->setPosition("backgroundPanel.x + 5", "seedLabel.y + seedLabel.h + 5");
	decksEditBox->setSize(0.F, "7%");
	decksEditBox->setTextSize(0U);
	decksEditBox->setSize(tgui::Text::getLineWidth("999", tgui::getGlobalFont(), decksEditBox->getTextSize()) + 10U, "7%");

	decksLabel->setTextSize(decksEditBox->getTextSize());
	decksEditBox->setPosition("decksLabel.x + decksLabel.w - 5", "decksLabel.y + 5");

	decksSlider->setSize("backgroundPanel.w - (decksLabel.w + decksEditBox.w + 20)", "3%");
	decksSlider->setPosition("decksEditBox.x + decksEditBox.w + 10", "decksEditBox.y + 2%");
	
	startGameButton->setSize("(backgroundPanel.w - 15) / 2", "7%");
	startGameButton->setPosition("(backgroundPanel.x + backgroundPanel.w) - (w + 5)", "backgroundPanel.y + backgroundPanel.height - (h + 5)");

	mainMenuButton->setSize("(backgroundPanel.w - 15) / 2", "7%");
	mainMenuButton->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + backgroundPanel.height - (h + 5)");

	randomSeedLabel->setTextSize(mainMenuButton->getTextSize() / 2U);
	randomSeedLabel->setPosition("backgroundPanel.x + backgroundPanel.w - (w + 5)", "mainMenuButton.y - (h + 5)");
}

void scene::GameSetupScene::deactivated(sf::RenderTarget * renderTarget) {
	titleLabel.reset();
	backgroundPanel.reset();
	seedEditBox.reset();
	seedLabel.reset();
	decksLabel.reset();
	decksSlider.reset();
	startGameButton.reset();
	mainMenuButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}
