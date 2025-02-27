#pragma once

#include "stdafx.h"

#include "Engine.h"

namespace mapping {
	template<typename T>
	class HexagonMap {
	protected:
		int xCoordinate;
		int yCoordinate;
		unsigned int coordinateWidth;
		unsigned int coordinateHeight;
		std::size_t entryCount;

		const engine::Hexagon * hexagon;
		T ** map;

		sf::Vector2i coordinateLocation;
		sf::Vector2u coordinateSize;
		sf::IntRect coordinateBounds;

	public:
		inline HexagonMap(int xCoordinate, int yCoordinate, unsigned int coordinateWidth, unsigned int coordinateHeight, const engine::Hexagon * hexagon) {
			this->xCoordinate = xCoordinate;
			this->yCoordinate = yCoordinate;
			this->coordinateWidth = coordinateWidth;
			this->coordinateHeight = coordinateHeight;
			entryCount = coordinateWidth * coordinateHeight;
			this->hexagon = hexagon;

			//Allocate space for the map.
			map = static_cast<T **>(malloc(sizeof(T *) * coordinateWidth));
			for (unsigned int x = 0U; x < coordinateWidth; ++x) {
				map[x] = static_cast<T *>(malloc(sizeof(T) * coordinateHeight));

				//Initialize the map's contents to default.
				for (unsigned int y = 0U; y < coordinateHeight; ++y) {
					map[x][y] = T();
				}
			}

			coordinateLocation = sf::Vector2i(xCoordinate, yCoordinate);
			coordinateSize = sf::Vector2u(coordinateWidth, coordinateHeight);
			coordinateBounds = sf::IntRect(coordinateLocation, static_cast<sf::Vector2i>(coordinateSize));
		}

		inline HexagonMap(const HexagonMap<T> & tileMap) { //Copies a map
			xCoordinate = tileMap.xCoordinate;
			yCoordinate = tileMap.yCoordinate;
			coordinateWidth = tileMap.coordinateWidth;
			coordinateHeight = tileMap.coordinateHeight;
			entryCount = tileMap.entryCount;
			hexagon = tileMap.hexagon;

			//Allocate space for the map.
			map = static_cast<T **>(malloc(sizeof(T *) * coordinateWidth));
			for (unsigned int x = 0U; x < coordinateWidth; ++x) {
				map[x] = static_cast<T *>(malloc(sizeof(T) * coordinateHeight));

				if (map[x] != nullptr) {
					memcpy(map[x], tileMap.map[x], sizeof(T) * coordinateHeight); //Copy the contents over.
				}
			}

			coordinateLocation = tileMap.coordinateLocation;
			coordinateSize = tileMap.coordinateSize;
			coordinateBounds = tileMap.coordinateBounds;
		}

		inline virtual ~HexagonMap() { //Deallocaes the map to prevent memory leaks.
			for (unsigned int x = 0U; x < coordinateWidth; ++x) {
				free(map[x]);
			}

			free(map);
		}

		inline virtual bool containsCoordinate(const sf::Vector2i & location) const {
			return containsCoordinate(location.x, location.y);
		}

		inline virtual bool containsCoordinate(int x, int y) const {
			return coordinateBounds.contains(x, y);
		}

		inline virtual T & queryMap(const sf::Vector2u & location) {
			return queryMap(location.x, location.y);
		}

		inline virtual T & queryMap(unsigned int x, unsigned int y) {
			return map[x][y];
		}

		inline virtual sf::Vector2i asLocalCoordinatesI(const sf::Vector2i & location) {
			return location - coordinateLocation;
		}

		inline virtual sf::Vector2i asLocalCoordinatesI(int x, int y) {
			return asLocalCoordinatesI(sf::Vector2i(x, y));
		}

		inline virtual sf::Vector2u asLocalCoordinatesU(const sf::Vector2i & location) {
			return static_cast<sf::Vector2u>(location - coordinateLocation);
		}

		inline virtual sf::Vector2u asLocalCoordinatesU(int x, int y) {
			return asLocalCoordinatesU(sf::Vector2i(x, y));
		}

		inline virtual int getXCoordinate() const {
			return xCoordinate;
		}

		inline virtual int getYCoordinate() const {
			return yCoordinate;
		}

		inline virtual unsigned int getCoordinateWidth() const {
			return coordinateWidth;
		}

		inline virtual unsigned int getCoordinateHeight() const {
			return coordinateHeight;
		}

		inline virtual std::size_t getEntryCount() const {
			return entryCount;
		}

		inline virtual const engine::Hexagon * getHexagon() const {
			return hexagon;
		}

		inline virtual T ** getMap() const {
			return map;
		}

		inline virtual sf::Vector2i getCoordinateLocation() const {
			return coordinateLocation;
		}

		inline virtual sf::Vector2u getCoordinateSize() const {
			return coordinateSize;
		}

		inline virtual sf::IntRect getCoordinateBounds() const {
			return coordinateBounds;
		}
	};
};
