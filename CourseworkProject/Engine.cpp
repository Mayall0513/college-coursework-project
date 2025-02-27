#include "stdafx.h"

#include "Engine.h"
#include "Scene.h"

#include <fstream>

const float engine::Hexagon::sqrt3d2 = std::sqrtf(3.F) / 2.F;
const float engine::Hexagon::d2b3 = 2.F / 3.F;
const float engine::Hexagon::md1b3 = -1.F / 3.F;
const float engine::Hexagon::sqrt3d3 = std::sqrtf(3.F) / 3.F;

engine::Hexagon::Hexagon() {
	vectors = std::make_unique<sf::Vector2f[]>(7U);
	generateHexagon(1.F);
}

void engine::Hexagon::generateHexagon(float sideLength) {
	float a = sqrt3d2 * sideLength;
	float b = sideLength / 2.F;
	float c = b + sideLength;
	float d = sideLength * 2.F;
	float e = a * 2.F;

	vectors[0] = sf::Vector2f(b, 0.F);
	vectors[1] = sf::Vector2f(c, 0.F);
	vectors[2] = sf::Vector2f(d, a);
	vectors[3] = sf::Vector2f(c, e);
	vectors[4] = sf::Vector2f(b, e);
	vectors[5] = sf::Vector2f(0.F, a);
	vectors[6] = sf::Vector2f(sideLength, a);
	size = sf::Vector2f(d, e);

	this->sideLength = sideLength;
	halfSideLength = b;
	oneAHalfSideLength = c;
	twoSideLength = d;
	twoAHalfSideLength = d + b;
	threeSideLength = d + sideLength;
	halfHeight = a;

	repeatX = threeSideLength;
	repeatY = size.y;
	boundaryGradient = halfHeight / halfSideLength;
}

std::unique_ptr<sf::Vector2f[]> engine::Hexagon::getTranslation(float x, float y) const {
	std::unique_ptr<sf::Vector2f[]> translation = std::make_unique<sf::Vector2f[]>(7U);
	if (translation) {
		memcpy(translation.get(), vectors.get(), sizeof(sf::Vector2f) * 7U); //Copy the data from the vectors to translation.

		for (unsigned int i = 0; i < 7U; ++i) { //Translate everything.
			translation[i].x += x;
			translation[i].y += y;
		}
	}

	return translation;
}

sf::Vector2i engine::Hexagon::mapCoordinates(float x, float y) const { //Converts from world coordinates to map/grid coordinates.
	float trueX = x - twoAHalfSideLength;
	float trueY = y - halfHeight;

	float axialX = (d2b3 * trueX) / sideLength; //Also cube x.
	float axialY = (md1b3 * trueX + sqrt3d3 * trueY) / sideLength; //Also cube z.

	//x + y + z = 0, therefore y = -x - z
	float cubeY = -axialX - axialY;

	int rCubeX = static_cast<int>(std::roundf(axialX));
	int rCubeY = static_cast<int>(std::roundf(cubeY));
	int rCubeZ = static_cast<int>(std::roundf(axialY));

	float differenceX = std::fabsf(rCubeX - axialX);
	float differenceY = std::fabsf(rCubeY - cubeY);
	float differenceZ = std::fabsf(rCubeZ - axialY);

	if (differenceX > differenceY && differenceX > differenceZ) {
		rCubeX = -rCubeY - rCubeZ;
	}

	else if (differenceY > differenceZ) {
		rCubeY = -rCubeX - rCubeZ;
	}

	else {
		rCubeZ = -rCubeX - rCubeY;
	}

	return sf::Vector2i(rCubeX, rCubeZ + (rCubeX + (rCubeX & 1)) / 2);
}

sf::Vector2f engine::Hexagon::worldCoordinates(float x, float y) const { //Gets the origin of the parent hexagon of the point.
	float xInset = std::fmodf(x, repeatX);

	if (xInset < 0.F) {
		xInset = repeatX + xInset;
	}

	bool firstColumn = xInset > halfSideLength && xInset < oneAHalfSideLength;
	bool secondColumn = xInset > twoSideLength && xInset < threeSideLength;

	float hexagonX = std::floorf(x / repeatX) * repeatX;
	float hexagonY = 0.F;

	if (firstColumn || secondColumn) {
		if (firstColumn) {
			y -= halfHeight;
			hexagonY = halfHeight;
		}

		else {
			hexagonX += oneAHalfSideLength;
		}

		hexagonY += std::floorf(y / size.y) * size.y;
	}

	else {
		hexagonY = std::floorf(y / size.y) * size.y;

		bool secondRow = xInset < halfSideLength;
		bool topSide = false;
		bool leftSide = false;
		float yInset = std::fmodf((secondRow ? y + halfHeight : y), size.y);

		if (yInset < 0.F) {
			yInset = repeatY + yInset;
		}

		if (!secondRow) {
			xInset -= oneAHalfSideLength;
		}

		if (yInset < halfHeight) {
			leftSide = halfHeight - boundaryGradient * xInset > yInset;
			topSide = true;
		}

		else {
			leftSide = halfHeight + boundaryGradient * xInset <= yInset;
			topSide = false;
		}

		if (!secondRow) {
			if (!leftSide) {
				hexagonX += oneAHalfSideLength;
			}

			else {
				if (topSide) {
					hexagonY -= halfHeight;
				}

				else {
					hexagonY += halfHeight;
				}
			}
		}

		else {
			if (leftSide) {
				hexagonX -= oneAHalfSideLength;
			}

			else {
				if (topSide) {
					hexagonY += halfHeight;
				}

				else {
					hexagonY -= halfHeight;
				}
			}
		}
	}

	return sf::Vector2f(hexagonX, hexagonY);
}

sf::Vector2f engine::Hexagon::worldCoordinates(int x, int y) const { //Converts from map/grid coordinates to world coordinates.
	return sf::Vector2f(oneAHalfSideLength * (x + 1), size.y * (y - 0.5F * !(x & 1)));
}

engine::ZoomLevel::ZoomLevel(float zoomLevel, sf::VertexBuffer * const vertexBuffer, sf::Vector2f * const unitHexagonMap, std::size_t unitHexagonMapSize, const sf::IntRect & coordinateBounds, const sf::FloatRect & unitBounds, const sf::FloatRect & worldBounds, const sf::Vector2f & imageOffset) {
	this->zoomLevel = zoomLevel;
	this->vertexBuffer = std::unique_ptr<sf::VertexBuffer>(vertexBuffer);
	this->unitHexagonMap = std::make_unique<sf::Vector2f[]>(unitHexagonMapSize);
	this->unitHexagonMapSize = unitHexagonMapSize;
	this->coordinateBounds = coordinateBounds;
	this->unitBounds = unitBounds;
	this->worldBounds = worldBounds;
	this->imageOffset = imageOffset;

	if (this->unitHexagonMap) {
		memcpy(this->unitHexagonMap.get(), unitHexagonMap, sizeof(sf::Vector2f) * unitHexagonMapSize); //Copy the contents of the passed unitHexagonMap pointer to our locally managed unitHexagonMap.
	}
}

engine::CombinedRenderStates::CombinedRenderStates() {
	renderState.texture = stateTexture.get();
	renderState.shader = stateShader.get();
}

void engine::JSONArrayBuilder::flush(std::ostream & targetStream) { //Print the array as JSON to the given stream.
	std::size_t sm1 = arrayEntries.size() - 1U;

	for (std::size_t i = 0U; i < arrayEntries.size(); ++i) {
		targetStream << arrayEntries[i];
		if (i < sm1) {
			targetStream << ",";
		}

		targetStream << std::endl;
	}
}

sf::FloatRect engine::applyLetterboxLayout() { //Returns the largest rectangle the game can be put into in the window while maintaining aspect ratio.
	sf::Vector2u windowSize = engine::renderWindow->getSize();
	sf::Vector2f backBufferSize = engine::renderWindow->getDefaultView().getSize();

	float xMultiplier = windowSize.x / backBufferSize.x;
	float yMultiplier = windowSize.y / backBufferSize.y;

	bool fromX = xMultiplier < yMultiplier;
	float multiplier = fromX ? xMultiplier : yMultiplier;

	float newResolutionWidth = backBufferSize.x * multiplier;
	float newResolutionHeight = backBufferSize.y * multiplier;
	float cardOffsetX = fromX ? 0.F : (windowSize.x - newResolutionWidth) / 2.F;
	float cardOffsetY = fromX ? (windowSize.y - newResolutionHeight) / 2.F : 0.F;

	return sf::FloatRect(cardOffsetX, cardOffsetY, newResolutionWidth, newResolutionHeight);
}

bool engine::writeDefaultSettingsFile(std::ofstream & fileStream) {
	fileStream << "{" << std::endl;
	engine::JSONArrayBuilder settingsArrayBuilder;

	settingsArrayBuilder.append(std::string("  \"resolution_width\": 1280"));
	settingsArrayBuilder.append(std::string("  \"resolution_height\": 720"));
	settingsArrayBuilder.append(std::string("  \"antialiasing_level\": 2"));
	settingsArrayBuilder.append(std::string("  \"fullscreen\": false"));

	settingsArrayBuilder.flush(fileStream);
	fileStream << "}" << std::endl;
	return true;
}

bool engine::fullscreen = false;