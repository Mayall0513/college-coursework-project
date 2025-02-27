#include "stdafx.h"

#include "Scene.h"

std::size_t scene::Scene::nextID = 0U;

void scene::Scene::pressedLMButton(int x, int y) {
	leftMButtonDown = true;
	leftMClickLocation = sf::Vector2i(x, y);
	leftMButtonHoldTime = 0.F;

	anyMButtonDown = true;
	anyMClickLocation = leftMClickLocation;
}

void scene::Scene::pressedMMButton(int x, int y) {
	middleMButtonDown = true;
	middleMClickLocation = sf::Vector2i(x, y);
	middleMButtonHoldTime = 0.F;

	anyMButtonDown = true;
	anyMClickLocation = middleMClickLocation;
}

void scene::Scene::pressedRMButton(int x, int y) {
	rightMButtonDown = true;
	rightMClickLocation = sf::Vector2i(x, y);
	rightMButtonHoldTime = 0.F;

	anyMButtonDown = true;
	anyMClickLocation = rightMClickLocation;
}

void scene::Scene::releasedLMButton() {
	leftMButtonDown = false;
	leftMButtonHoldTime = 0.F;
	releasedButton();
}

void scene::Scene::releasedMMButton() {
	middleMButtonDown = false;
	middleMButtonHoldTime = 0.F;
	releasedButton();
}

void scene::Scene::releasedRMButton() {
	rightMButtonDown = false;
	rightMButtonHoldTime = 0.F;
	releasedButton();
}

void scene::Scene::releasedButton() {
	if (!(leftMButtonDown || middleMButtonDown || rightMButtonDown)) {
		anyMButtonDown = false;
		anyMButtonHoldTime = 0.F;
	}
}

void scene::Scene::update(float deltaSeconds) {
	if (leftMButtonDown) {
		leftMButtonHoldTime += deltaSeconds;
	}

	if (middleMButtonDown) {
		middleMButtonHoldTime += deltaSeconds;
	}

	if (rightMButtonDown) {
		rightMButtonHoldTime += deltaSeconds;
	}

	if (anyMButtonDown) {
		anyMButtonHoldTime += deltaSeconds;
	}

	totalTimeSeconds += deltaSeconds;
}

void scene::Scene::mousePressed(sf::Mouse::Button buttonPressed) {
	switch (buttonPressed) {
	case sf::Mouse::Button::Left:
		pressedLMButton(mouseLocation.x, mouseLocation.y);
		break;

	case sf::Mouse::Button::Middle:
		pressedMMButton(mouseLocation.x, mouseLocation.y);
		break;

	case sf::Mouse::Button::Right:
		pressedRMButton(mouseLocation.x, mouseLocation.y);
		break;
	}
}

void scene::Scene::mouseReleased(sf::Mouse::Button buttonReleased) {
	switch (buttonReleased) {
	case sf::Mouse::Button::Left:
		releasedLMButton();
		break;

	case sf::Mouse::Button::Middle:
		releasedMMButton();
		break;

	case sf::Mouse::Button::Right:
		releasedRMButton();
		break;
	}
}

void scene::Scene::letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) {
	letterBoxOffset = engine::rectangleLocation<int, int>(letterBox);
	letterBoxSize = engine::rectangleSize<unsigned int, int>(letterBox);
	this->letterBox = letterBox;
}

void scene::Scene::pumpEvent(const sf::Event & event) {
	switch (event.type) {
	case sf::Event::Resized: { //Updates the window such that it enforces a letter box layout.
		sf::View windowView = engine::renderWindow->getView();
		sf::Vector2u windowSize = engine::renderWindow->getSize();
		sf::FloatRect letterBoxBounds = engine::applyLetterboxLayout();

		windowView.setViewport(sf::FloatRect(letterBoxBounds.left / windowSize.x, letterBoxBounds.top / windowSize.y, letterBoxBounds.width / windowSize.x, letterBoxBounds.height / windowSize.y));
		engine::renderWindow->setView(windowView);
		letterBoxAdjusted(windowView, static_cast<sf::IntRect>(letterBoxBounds));
		break;
	}

	case sf::Event::MouseMoved:
		mouseMoved(event.mouseMove.x, event.mouseMove.y);
		break;

	case sf::Event::MouseButtonPressed:
		mousePressed(event.mouseButton.button);
		break;

	case sf::Event::MouseButtonReleased:
		mouseReleased(event.mouseButton.button);
		break;

	case sf::Event::MouseWheelScrolled:
		mouseWheelScrolled(event.mouseWheelScroll.delta);
		break;

	case sf::Event::KeyPressed:
		keyPressed(event.key);
		break;

	case sf::Event::KeyReleased:
		keyReleased(event.key);
		break;
	}
}

void scene::Scene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	letterBoxOffset = engine::rectangleLocation<int, int>(viewport);
	letterBoxSize = engine::rectangleSize<unsigned int, int>(viewport);
	letterBox = viewport;

	backBufferSize = static_cast<sf::Vector2u>(renderTarget->getDefaultView().getSize());
}

scene::Scene * engine::currentScene = nullptr;
sf::RenderWindow * engine::renderWindow = nullptr;

void engine::setCurrentScene(scene::Scene * newScene, bool autoDeallocate) {
	if (hasScene()) {
		currentScene->deactivated(renderWindow);

		if (autoDeallocate) {
			delete currentScene;
		}
	}

	currentScene = newScene;
	if (hasScene()) {
		currentScene->activated(renderWindow, static_cast<sf::IntRect>(renderWindow->getViewport(renderWindow->getView())));
	}
}