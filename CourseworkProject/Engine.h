#pragma once

#include "stdafx.h"

#include <memory>

namespace engine {
	class Hexagon {
	private:
		static const float sqrt3d2;
		static const float d2b3;
		static float const md1b3;
		static float const sqrt3d3;

		float sideLength;
		std::unique_ptr<sf::Vector2f[]> vectors;
		sf::Vector2f size;

		float halfSideLength;
		float oneAHalfSideLength; //One and a half side lengths.
		float twoSideLength;
		float twoAHalfSideLength; //Two and a half side lengths.
		float threeSideLength;
		float halfHeight;

		float repeatX;
		float repeatY;
		float boundaryGradient;

	public:
		Hexagon();

		void generateHexagon(float sideLength);

		inline const sf::Vector2f * getVectors() const { return vectors.get(); }
		std::unique_ptr<sf::Vector2f[]> getTranslation(float x, float y) const;

		//Returns the map-coordinates of the hexagon that contains the point. 
		inline sf::Vector2i mapCoordinates(const sf::Vector2f & location) const { return mapCoordinates(location.x, location.y); }
		sf::Vector2i mapCoordinates(float x, float y) const;

		//Returns the world-coordinates of the hexagon that contains the point.
		inline sf::Vector2f worldCoordinates(const sf::Vector2f & location) const { return worldCoordinates(location.x, location.y); }
		sf::Vector2f worldCoordinates(float x, float y) const;

		//Returns the world-coordinates of the hexagon at the map-coordinates.
		inline sf::Vector2f worldCoordinates(const sf::Vector2i & coordinates) const { return worldCoordinates(coordinates.x, coordinates.y); }
		sf::Vector2f worldCoordinates(int x, int y) const;

		inline float getSideLength() const { return sideLength; }
		inline float getOneAHalfSideLength() const { return oneAHalfSideLength; } //One and a half side lengths.
		inline float getTwoSideLength() const { return twoSideLength; }
		inline float getTwoAHalfSideLength() const { return twoAHalfSideLength; } //Two and a half side lengths.
		inline float getThreeSideLength() const { return threeSideLength; }
		inline float getHalfHeight() const { return halfHeight; }

		inline float getWidth() const { return size.x; }
		inline float getHeight() const { return size.y; }
		inline sf::Vector2f getSize() const { return size; }

		inline float getRepeatX() const { return repeatX; }
		inline float getRepeatY() const { return repeatY; }
	};

	class ZoomLevel {
	private:
		float zoomLevel;

		std::unique_ptr<sf::VertexBuffer> vertexBuffer;
		std::unique_ptr<sf::Vector2f[]> unitHexagonMap;
		std::size_t unitHexagonMapSize;

		sf::IntRect coordinateBounds;
		sf::FloatRect unitBounds;
		sf::FloatRect worldBounds;
		sf::Vector2f imageOffset;

	public:
		ZoomLevel(float zoomLevel, sf::VertexBuffer * const vertexBuffer, sf::Vector2f * const unitHexagonMap, std::size_t unitHexagonMapSize, const sf::IntRect & coordinateBounds, const sf::FloatRect & unitBounds, const sf::FloatRect & worldBounds, const sf::Vector2f & imageOffset);

		inline float getZoomLevel() const { return zoomLevel; }

		inline const sf::VertexBuffer * getVertexBuffer() const { return vertexBuffer.get(); }
		inline const sf::Vector2f * getUnitHexagonMap() const { return unitHexagonMap.get(); }
		inline std::size_t getUnitHexagonMapSize() const { return unitHexagonMapSize; }

		inline sf::IntRect getCoordinateBounds() const { return coordinateBounds; }
		inline sf::FloatRect getUnitBounds() const { return unitBounds; }
		inline sf::FloatRect getWorldBounds() const { return worldBounds; }
		inline sf::Vector2f getImageOffset() const { return imageOffset; }
	};

	class CombinedRenderStates {
	private:
		sf::RenderStates renderState = sf::RenderStates();
		std::shared_ptr<sf::Texture> stateTexture = std::make_shared<sf::Texture>();
		std::shared_ptr<sf::Shader> stateShader = std::make_shared<sf::Shader>();

	public:
		CombinedRenderStates();

		inline sf::BlendMode & blendMode() { return renderState.blendMode; }
		inline sf::Transform & transform() { return renderState.transform; }

		inline operator sf::RenderStates &() { return renderState; }
		inline operator sf::RenderStates() const { return renderState; }

		inline sf::Texture * getTexture() { return stateTexture.get(); }
		inline sf::Shader * getShader() { return stateShader.get(); }
	};

	class EventHandler {
	public:
		virtual void mouseMoved(int x, int y) = 0;
		virtual void mousePressed(sf::Mouse::Button buttonPressed) = 0;
		virtual void mouseReleased(sf::Mouse::Button buttonReleased) = 0;
		virtual void mouseWheelScrolled(float scrollDelta) = 0;
		virtual void keyPressed(const sf::Event::KeyEvent & pressedEvent) = 0;
		virtual void keyReleased(const sf::Event::KeyEvent & releasedEvent) = 0;
		virtual void letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) = 0;
	};

	struct CardPlacement {
	public:
		float timeSeconds;
		sf::Vector2i coordinates;
		unsigned short cardID;

		CardPlacement() = default;
	};

	class JSONArrayBuilder {
	private:
		std::vector<std::string> arrayEntries = std::vector<std::string>();

	public:
		JSONArrayBuilder() = default;

		inline void append(const std::string & line) { arrayEntries.push_back(line); }
		void flush(std::ostream & targetStream);

		inline std::size_t size() const { return arrayEntries.size(); }
	};

	template<typename T, typename U>
	inline sf::Rect<T> translateRectangle(const sf::Rect<T> & rectangle, const sf::Vector2<U> & translation) { return sf::Rect<T>(rectangle.left + translation.x, rectangle.top + translation.y, rectangle.width, rectangle.height); }

	template<typename T, typename U>
	inline sf::Vector2<T> rectangleSize(const sf::Rect<U> & rectangle) { return sf::Vector2<T>(rectangle.width, rectangle.height); }

	template<typename T, typename U>
	inline sf::Vector2<T> rectangleLocation(const sf::Rect<U> & rectangle) { return sf::Vector2<T>(rectangle.left, rectangle.top); }

	sf::FloatRect applyLetterboxLayout();
	bool writeDefaultSettingsFile(std::ofstream & fileStream);

	extern bool fullscreen;
};
