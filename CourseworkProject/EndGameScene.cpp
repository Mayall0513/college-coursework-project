#include "EndGameScene.h"

#include "MainMenuScene.h"

void scene::EndGameScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);

	titleLabel = tgui::Label::create("End Game");
	titleLabel->setTextSize(50U);
	titleLabel->ignoreMouseEvents(true);
	titleLabel->setPosition("(&.w - w) / 2", "2%");
	titleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(titleLabel, "titleLabel");

	backgroundPanel = tgui::Panel::create();
	backgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(backgroundPanel, "backgroundPanel");

	turnCountLabel = tgui::Label::create(std::string("In ") + std::to_string(gameScene->getDeckCount()) + std::string(" turn") + std::string(gameScene->getDeckCount() > 1U ? "s" : ""));
	turnCountLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(turnCountLabel, "turnCountLabel");

	totalScoreLabel = tgui::Label::create(std::string("you managed to amass ") + std::to_string(gameScene->getGameScore()) + std::string(" point") + std::string(gameScene->getGameScore() > 1U ? "s" : "") + std::string("."));
	totalScoreLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(totalScoreLabel, "totalScoreLabel");

	std::string averageScoreString = std::to_string(static_cast<float>(gameScene->getGameScore()) / gameScene->getDeckCount());
	averageScoreString.erase(averageScoreString.find_last_not_of('0') + 1, std::string::npos); //Remove trialing zeroes.
	if (averageScoreString[averageScoreString.size() - 1U] == '.') { //Remove trialing '.' if there is one.
		averageScoreString.erase(averageScoreString.size() - 1U, std::string::npos);
	}

	averageScoreLabel = tgui::Label::create(std::string("That's an average of ") + averageScoreString + std::string(" per turn!"));
	averageScoreLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(averageScoreLabel, "averageScoreLabel");

	wellDoneLabel = tgui::Label::create(std::string("Well Done!"));
	wellDoneLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(wellDoneLabel, "wellDoneLabel");

	replayGameButton = tgui::Button::create("Replay Game");
	replayGameButton->setTextSize(0U);
	replayGameButton->setRenderer(guiTheme->getRenderer("button"));
	replayGameButton->connect("pressed", [&]() { //Create new game with exact same settings as the one we came in from.
		engine::setCurrentScene(new game::GameScene(gameScene->getWorldStringSeed(), gameScene->getDeckCount()), true);
		delete gameScene;
	});
	guiManager->add(replayGameButton, "replayGameButton");

	gotoMainMenuButton = tgui::Button::create("Goto Main Menu");
	gotoMainMenuButton->setTextSize(0U);
	gotoMainMenuButton->setRenderer(guiTheme->getRenderer("button"));
	gotoMainMenuButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::MainMenuScene(), true);
		delete gameScene;
	});
	guiManager->add(gotoMainMenuButton, "gotoMainMenuButton");

	backgroundPanel->setSize("60%", "75%");
	backgroundPanel->setPosition("(&.w - w) / 2", "20%");

	replayGameButton->setSize("(backgroundPanel.w - 15) * (3 / 5)", "7%");
	replayGameButton->setPosition("(backgroundPanel.x + backgroundPanel.w) - (w + 5)", "backgroundPanel.y + backgroundPanel.height - (h + 5)");

	gotoMainMenuButton->setSize("(backgroundPanel.w - 15) * (2 / 5)", "7%");
	gotoMainMenuButton->setPosition("backgroundPanel.x + 5", "backgroundPanel.y + backgroundPanel.height - (h + 5)");

	turnCountLabel->setTextSize(replayGameButton->getTextSize());
	turnCountLabel->setPosition("(&.w - w) / 2", "backgroundPanel.y + 5");

	totalScoreLabel->setTextSize(replayGameButton->getTextSize());
	totalScoreLabel->setPosition("(&.w - w) / 2", "turnCountLabel.y + turnCountLabel.h + 5");

	averageScoreLabel->setTextSize(replayGameButton->getTextSize());
	averageScoreLabel->setPosition("(&.w - w) / 2", "totalScoreLabel.y + totalScoreLabel.h + 5");

	wellDoneLabel->setTextSize(replayGameButton->getTextSize() * 2U);
	wellDoneLabel->setPosition("(&.w - w) / 2", "averageScoreLabel.y + averageScoreLabel.h + 5");
}

void scene::EndGameScene::deactivated(sf::RenderTarget * renderTarget) {
	titleLabel.reset();
	backgroundPanel.reset();

	turnCountLabel.reset();
	totalScoreLabel.reset();
	averageScoreLabel.reset();
	wellDoneLabel.reset();

	replayGameButton.reset();
	gotoMainMenuButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}
