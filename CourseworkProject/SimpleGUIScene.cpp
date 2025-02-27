#include "stdafx.h"

#include "SimpleGUIScene.h"

void scene::SimpleGUIScene::draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const {
	renderTarget.clear(sf::Color::Black);

	if (guiManager.get() != nullptr) {
		guiManager->draw();
	}
}

void scene::SimpleGUIScene::letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) {
	Scene::letterBoxAdjusted(view, letterBox);
	guiManager->setView(view);
}

void scene::SimpleGUIScene::pumpEvent(const sf::Event & event) {
	if (guiManager.get() != nullptr) {
		if (guiManager->handleEvent(event)) {
			if (event.type == sf::Event::EventType::MouseMoved) {
				hasMouse &= false;
			}
		}

		else {
			if (event.type == sf::Event::EventType::MouseMoved) {
				hasMouse |= true;
			}

			Scene::pumpEvent(event);
		}
	}

	else {
		if (event.type == sf::Event::EventType::MouseMoved) {
			hasMouse |= true;
		}

		Scene::pumpEvent(event);
	}
}

void scene::SimpleGUIScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	Scene::activated(renderTarget, viewport);

	guiManager.reset(new tgui::Gui(*renderTarget));
	guiManager->setView(renderTarget->getView());
}