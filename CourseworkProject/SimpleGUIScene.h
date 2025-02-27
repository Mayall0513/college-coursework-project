#pragma once

#include "Scene.h"

#include "TGUI\TGUI.hpp"

namespace scene {
	class SimpleGUIScene : public scene::Scene {
	protected:
		std::unique_ptr<tgui::Gui> guiManager;
		std::unique_ptr<tgui::Theme> guiTheme;

	public:
		inline SimpleGUIScene() { guiTheme = std::make_unique<tgui::Theme>("content\\gui_theme.tthm"); }
		virtual ~SimpleGUIScene() = default;

		inline void update(float deltaSeconds) override {}
		void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const override;
		inline void mouseMoved(int x, int y) override {}
		inline void mousePressed(sf::Mouse::Button buttonPressed) override {}
		inline void mouseReleased(sf::Mouse::Button buttonReleased) override {}
		inline void mouseWheelScrolled(float scrollDelta) override {}
		inline void keyPressed(const sf::Event::KeyEvent & pressedEvent) override {}
		inline void keyReleased(const sf::Event::KeyEvent & releasedEvent) override {}
		void letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) override;

		void pumpEvent(const sf::Event & event) override;

		virtual void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		virtual void deactivated(sf::RenderTarget * renderTarget) override { guiManager.reset(); }
	};
};