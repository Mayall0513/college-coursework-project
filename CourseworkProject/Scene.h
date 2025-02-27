#pragma once

#include "Engine.h"

namespace scene {
	class Scene : public engine::EventHandler, public sf::Drawable {
	private:
		static std::size_t nextID;
		std::size_t id;

	protected:
		bool leftMButtonDown = false;
		sf::Vector2i leftMClickLocation = sf::Vector2i();
		float leftMButtonHoldTime = 0.F;
		bool middleMButtonDown = false;
		sf::Vector2i middleMClickLocation = sf::Vector2i();
		float middleMButtonHoldTime = 0.F;
		bool rightMButtonDown = false;
		sf::Vector2i rightMClickLocation = sf::Vector2i();
		float rightMButtonHoldTime = 0.F;
		bool anyMButtonDown = false;
		sf::Vector2i anyMClickLocation = sf::Vector2i();
		float anyMButtonHoldTime = 0.F;

		sf::Vector2i mouseLocation = sf::Vector2i();
		bool hasMouse = false;
		float cumulativeScroll = 0.F;
		float totalTimeSeconds = 0.F;

		sf::Vector2i letterBoxOffset = sf::Vector2i();
		sf::Vector2u letterBoxSize = sf::Vector2u();
		sf::IntRect letterBox = sf::IntRect();

		sf::Vector2u backBufferSize = sf::Vector2u();

	public:
		inline Scene() { id = Scene::nextID++; }
		virtual ~Scene() = default;

	protected:
		virtual void pressedLMButton(int x, int y);
		virtual void pressedMMButton(int x, int y);
		virtual void pressedRMButton(int x, int y);

		virtual void releasedLMButton();
		virtual void releasedMMButton();
		virtual void releasedRMButton();

		virtual void releasedButton();

	public:
		virtual void update(float deltaSeconds);
		virtual void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const override = 0;
		inline virtual void mouseMoved(int x, int y) override { mouseLocation = sf::Vector2i(x, y); }
		virtual void mousePressed(sf::Mouse::Button buttonPressed) override;
		virtual void mouseReleased(sf::Mouse::Button buttonReleased) override;
		inline virtual void mouseWheelScrolled(float scrollDelta) override { cumulativeScroll += scrollDelta; }
		virtual void keyPressed(const sf::Event::KeyEvent & pressedEvent) override = 0;
		virtual void keyReleased(const sf::Event::KeyEvent & releasedEvent) override = 0;
		virtual void letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) override;
		inline virtual bool equals(const Scene & otherScene) const { return id == otherScene.getID(); }

		virtual void pumpEvent(const sf::Event & event);

		virtual void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport);
		virtual void deactivated(sf::RenderTarget * renderTarget) = 0;

		inline std::size_t getID() const { return id; }

		inline bool operator ==(const scene::Scene & otherScene) { return equals(otherScene); }
	};
};

namespace engine {
	extern scene::Scene * currentScene;
	extern sf::RenderWindow * renderWindow;

	inline scene::Scene * getCurrentScene() { return currentScene; }
	inline bool hasScene() { return currentScene != nullptr; }
	void setCurrentScene(scene::Scene * newScene, bool autoDeallocate = true);
}