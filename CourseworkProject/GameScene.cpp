#include "GameScene.h"

#include "GameResources.h"
#include "WindowsFunctions.h"
#include "MainMenuScene.h"
#include "SaveScene.h"
#include "EndGameScene.h"

#include <iomanip>
#include <iostream>
#include <fstream>

game::PointOfInterest::PointOfInterest(unsigned short poiType) {
	type = static_cast<game::PointOfInterest::POIType>(poiType);
	textureCoordinateIndex = poiType * 4U;
}

game::PointOfInterest::PointOfInterest(POIType poiType) {
	type = poiType;
	textureCoordinateIndex = static_cast<unsigned short>(poiType) * 4U;
}

game::PlayerCard::PlayerCard(unsigned short cardID) {
	type = static_cast<CardType>(cardID);
	textureCoordinateIndex = cardID * 4U;
}

game::PlayerCard::PlayerCard(CardType cardType) {
	type = cardType;
	textureCoordinateIndex = static_cast<unsigned short>(cardType) * 4U;
}

game::PlayerCard::Adjacency & game::PlayerCard::getAdjacency(unsigned short adjacencyID) {
	switch (adjacencyID) {
	case 0U: return northWestAdjacency;
	case 1U: return northAdjacency;
	case 2U: return northEastAdjacency;
	case 3U: return southEastAdjacency;
	case 4U: return southAdjacency;
	default: return southWestAdjacency;
	}
}

game::PlayerCard::Adjacency game::PlayerCard::getAdjacency(unsigned short adjacencyID) const {
	switch (adjacencyID) {
	case 0U: return northWestAdjacency;
	case 1U: return northAdjacency;
	case 2U: return northEastAdjacency;
	case 3U: return southEastAdjacency;
	case 4U: return southAdjacency;
	default: return southWestAdjacency;
	}
}

std::wstring & game::PlayerCard::getStringName() const {
	switch (type) {
	case game::PlayerCard::PLAYERCARD_SPECIAL_HEADQUARTERS:
		return resources::STRING_PLAYERCARD_SPECIAL_HEADERQUARTERS_NAME;

	case game::PlayerCard::PLAYERCARD_SPECIAL_OUTPOST:
		return resources::STRING_PLAYERCARD_SPECIAL_OUTPOST_NAME;

	case game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY:
		return resources::STRING_PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY_NAME;

	case game::PlayerCard::PLAYERCARD_PRODUCER_LOGGINGCAMP:
		return resources::STRING_PLAYERCARD_PRODUCER_LOGGINGCAMP_NAME;

	case game::PlayerCard::PLAYERCARD_PRODUCER_FISHINGWHARF:
		return resources::STRING_PLAYERCARD_PRODUCER_FISHINGWHARF_NAME;

	case game::PlayerCard::PLAYERCARD_PRODUCER_FARM:
		return resources::STRING_PLAYERCARD_PRODUCER_FARM_NAME;

	case game::PlayerCard::PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX:
		return resources::STRING_PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX_NAME;

	case game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY:
		return resources::STRING_PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY_NAME;

	case game::PlayerCard::PLAYERCARD_PRODUCER_MINE:
		return resources::STRING_PLAYERCARD_PRODUCER_MINE_NAME;

	case game::PlayerCard::PLAYERCARD_PRODUCER_OILJACK:
		return resources::STRING_PLAYERCARD_PRODUCER_OILJACK_NAME;

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX:
		return resources::STRING_PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX_NAME;

	case game::PlayerCard::PLAYERCARD_POWERPLANT_WINDTURBINES:
		return resources::STRING_PLAYERCARD_POWERPLANT_WINDTURBINE_NAME;

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_FOOD:
		return resources::STRING_PLAYERCARD_INTERMEDIATE_FACTORY_FOOD_NAME;

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL:
		return resources::STRING_PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL_NAME;

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL:
		return resources::STRING_PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL_NAME;

	case game::PlayerCard::PLAYERCARD_POWERPLANT_COAL:
		return resources::STRING_PLAYERCARD_POWERPLANT_COAL_NAME;

	case game::PlayerCard::PLAYERCARD_POWERPLANT_GEOTHERMAL:
		return resources::STRING_PLAYERCARD_POWERPLANT_GEOTHERMAL_NAME;

	case game::PlayerCard::PLAYERCARD_POWERPLANT_NUCLEAR:
		return resources::STRING_PLAYERCARD_POWERPLANT_NUCLEAR_NAME;

	case game::PlayerCard::PLAYERCARD_CONSUMER_AIRPORT:
		return resources::STRING_PLAYERCARD_CONSUMER_AIRPORT_NAME;

	default:
		return resources::STRING_PLAYERCARD_SPECIAL_LABORATORY_NAME;
	}
}

sf::Color game::PlayerCard::getBackColor() const {
	switch (type) {
	case PLAYERCARD_SPECIAL_HEADQUARTERS:
	case PLAYERCARD_SPECIAL_OUTPOST:
	case PLAYERCARD_SPECIAL_LABORATORY:
		return resources::COLOR_CARDTYPE_SPECIAL;

	case PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY:
	case PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX:
	case PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY:
	case PLAYERCARD_CONSUMER_AIRPORT:
		return resources::COLOR_CARDTYPE_CONSUMER;

	case PLAYERCARD_PRODUCER_LOGGINGCAMP:
	case PLAYERCARD_PRODUCER_FISHINGWHARF:
	case PLAYERCARD_PRODUCER_FARM:
	case PLAYERCARD_PRODUCER_MINE:
	case PLAYERCARD_PRODUCER_OILJACK:
		return resources::COLOR_CARDTYPE_PRODUCER;

	case PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX:
	case PLAYERCARD_INTERMEDIATE_FACTORY_FOOD:
	case PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL:
	case PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL:
		return resources::COLOR_CARDTYPE_INTERMEDIATE;

	default:
		return resources::COLOR_CARDTYPE_POWERPLANT;
	}
}

bool game::PlayerCard::emitsSmoke() const {
	switch (type) {
	case PLAYERCARD_INTERMEDIATE_FACTORY_FOOD:
	case PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL:
	case PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL:
	case PLAYERCARD_POWERPLANT_COAL:
	case PLAYERCARD_POWERPLANT_GEOTHERMAL:
	case PLAYERCARD_POWERPLANT_NUCLEAR:
		return true;

	default:
		return false;
	}
}

unsigned int game::PlayerCard::getScore() const {
	switch (type) {
	case game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY: {
		float fractionalScore = 20.F;

		multiplyPerCard(fractionalScore, 1.1F, game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY);
		multiplyPerCard(fractionalScore, 1.15F, game::PlayerCard::PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX);
		multiplyPerCard(fractionalScore, 1.2F, game::PlayerCard::PLAYERCARD_CONSUMER_AIRPORT);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_POWERPLANT_WINDTURBINES);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_POWERPLANT_COAL);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_POWERPLANT_GEOTHERMAL);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_POWERPLANT_NUCLEAR);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_FOOD);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL);
		multiplyPerCard(fractionalScore, 0.8F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_PRODUCER_LOGGINGCAMP: {
		float fractionalScore = 15.F;

		multiplyPerPOI(fractionalScore, 2.25F, game::PointOfInterest::POINTOFINTEREST_TIMBER);
		multiplyPerCard(fractionalScore, 0.65F, game::PlayerCard::PLAYERCARD_PRODUCER_LOGGINGCAMP);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_PRODUCER_FISHINGWHARF: {
		float fractionalScore = 20.F;

		multiplyPerPOI(fractionalScore, 2.F, game::PointOfInterest::POINTOFINTEREST_FISH);
		multiplyPerCard(fractionalScore, 0.9F,game::PlayerCard::PLAYERCARD_PRODUCER_FISHINGWHARF);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_PRODUCER_FARM: {
		float fractionalScore = 15.F;

		multiplyPerPOI(fractionalScore, 1.25F, game::PointOfInterest::POINTOFINTEREST_TIMBER);
		multiplyPerPOI(fractionalScore, 2.25F, game::PointOfInterest::POINTOFINTEREST_ARABLE);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX: {
		float fractionalScore = 20.F;

		multiplyPerCard(fractionalScore, 1.05F, game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY);
		multiplyPerCard(fractionalScore, 1.05F, game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY);
		multiplyPerCard(fractionalScore, 1.25F, game::PlayerCard::PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX);
		multiplyPerCard(fractionalScore, 1.5F, game::PlayerCard::PLAYERCARD_CONSUMER_AIRPORT);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY: {
		float fractionalScore = 30.F;

		multiplyPerCard(fractionalScore, 1.05F, game::PlayerCard::PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY);
		multiplyPerCard(fractionalScore, 1.1F, game::PlayerCard::PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX);
		multiplyPerCard(fractionalScore, 1.1F, game::PlayerCard::PLAYERCARD_CONSUMER_AIRPORT);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_POWERPLANT_WINDTURBINES);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_POWERPLANT_COAL);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_POWERPLANT_GEOTHERMAL);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_POWERPLANT_NUCLEAR);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_FOOD);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL);
		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_PRODUCER_MINE: {
		float fractionalScore = 60.F;
		bool anyMinerals = false;

		//Cannot use |= (or ||) with the method calls as if the first bool is true nothing else is evaluated.
		//Meaning that if (for example) there is stone, nothing else will be checked cause the bool is already true.
		if (multiplyPerPOI(fractionalScore, 1.05F, game::PointOfInterest::POINTOFINTEREST_STONE)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.15F, game::PointOfInterest::POINTOFINTEREST_IRON)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.15F, game::PointOfInterest::POINTOFINTEREST_COAL)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.15F, game::PointOfInterest::POINTOFINTEREST_SILICON)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.2F, game::PointOfInterest::POINTOFINTEREST_ALUMINIUM)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.2F, game::PointOfInterest::POINTOFINTEREST_MARBLE)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.5F, game::PointOfInterest::POINTOFINTEREST_GOLD)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.55F,game::PointOfInterest::POINTOFINTEREST_GEMS)) {
			anyMinerals |= true;
		}

		if (multiplyPerPOI(fractionalScore, 1.45F, game::PointOfInterest::POINTOFINTEREST_URANIUM)) {
			anyMinerals |= true;
		}

		if (!anyMinerals) {
			fractionalScore *= 0.05F;
		}

		multiplyPerCard(fractionalScore, 0.85F, game::PlayerCard::PLAYERCARD_PRODUCER_MINE);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_PRODUCER_OILJACK: {
		float fractionalScore = 60.F;

		if (!multiplyPerPOI(fractionalScore, 3.F, game::PointOfInterest::POINTOFINTEREST_OIL)) {
			return 0U;
		}

		else {
			multiplyPerCard(fractionalScore, 0.9F, game::PlayerCard::PLAYERCARD_PRODUCER_OILJACK);
		}


		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX: {
		float fractionalScore = 30.F;

		multiplyPerCard(fractionalScore, 1.25F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_POWERPLANT_WINDTURBINES: {
		float fractionalScore = 30.F;

		multiplyPerPOI(fractionalScore, 2.F, game::PointOfInterest::POINTOFINTEREST_MOUNTAIN);
		multiplyPerCard(fractionalScore, 1.15F, game::PlayerCard::PLAYERCARD_POWERPLANT_WINDTURBINES);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_FOOD: {
		float fractionalScore = 20.F;

		multiplyPerPOI(fractionalScore, 1.1F, game::PointOfInterest::POINTOFINTEREST_TIMBER);
		multiplyPerPOI(fractionalScore, 1.5F, game::PointOfInterest::POINTOFINTEREST_ARABLE);
		multiplyPerPOI(fractionalScore, 1.5F, game::PointOfInterest::POINTOFINTEREST_FISH);
		multiplyPerCard(fractionalScore, 2.F, game::PlayerCard::PLAYERCARD_PRODUCER_FARM);
		multiplyPerCard(fractionalScore, 1.15F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_FOOD);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL: {
		float fractionalScore = 25.F;

		multiplyPerPOI(fractionalScore, 1.05F, game::PointOfInterest::POINTOFINTEREST_STONE);
		multiplyPerPOI(fractionalScore, 1.1F, game::PointOfInterest::POINTOFINTEREST_IRON);
		multiplyPerPOI(fractionalScore, 1.15F, game::PointOfInterest::POINTOFINTEREST_ALUMINIUM);
		multiplyPerPOI(fractionalScore, 1.20F, game::PointOfInterest::POINTOFINTEREST_MARBLE);
		multiplyPerPOI(fractionalScore, 1.35F, game::PointOfInterest::POINTOFINTEREST_GOLD);
		multiplyPerCard(fractionalScore, 1.5F, game::PlayerCard::PLAYERCARD_PRODUCER_MINE);
		multiplyPerCard(fractionalScore, 1.2F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL: {
		float fractionalScore = 30.F;

		multiplyPerPOI(fractionalScore, 1.15F, game::PointOfInterest::POINTOFINTEREST_SILICON);
		multiplyPerPOI(fractionalScore, 1.5F, game::PointOfInterest::POINTOFINTEREST_OIL);
		multiplyPerPOI(fractionalScore, 1.35F, game::PointOfInterest::POINTOFINTEREST_GEMS);
		multiplyPerPOI(fractionalScore, 1.35F, game::PointOfInterest::POINTOFINTEREST_URANIUM);
		multiplyPerCard(fractionalScore, 1.25F, game::PlayerCard::PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_POWERPLANT_COAL: {
		float fractionalScore = 35.F;

		multiplyPerCard(fractionalScore, 1.1F, game::PlayerCard::PLAYERCARD_PRODUCER_FARM);
		multiplyPerCard(fractionalScore, 1.15F, game::PlayerCard::PLAYERCARD_POWERPLANT_COAL);
		multiplyPerPOI(fractionalScore, 1.35F, game::PointOfInterest::POINTOFINTEREST_COAL);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_POWERPLANT_GEOTHERMAL: {
		float fractionalScore = 40.F;

		multiplyPerCard(fractionalScore, 1.05F, game::PlayerCard::PLAYERCARD_POWERPLANT_GEOTHERMAL);
		multiplyPerPOI(fractionalScore, 6.F, game::PointOfInterest::POINTOFINTEREST_GEOTHERMAL);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_POWERPLANT_NUCLEAR: {
		float fractionalScore = 50.F;

		multiplyPerCard(fractionalScore, 1.25F, game::PlayerCard::PLAYERCARD_POWERPLANT_NUCLEAR);
		multiplyPerPOI(fractionalScore, 2.5F, game::PointOfInterest::POINTOFINTEREST_URANIUM);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_CONSUMER_AIRPORT: {
		float fractionalScore = 80.F;

		multiplyPerCard(fractionalScore, 2.F, game::PlayerCard::PLAYERCARD_SPECIAL_HEADQUARTERS);
		multiplyPerCard(fractionalScore, 2.F, game::PlayerCard::PLAYERCARD_SPECIAL_OUTPOST);
		multiplyPerCard(fractionalScore, 2.F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);
		multiplyPerCard(fractionalScore, 0.25F, game::PlayerCard::PLAYERCARD_CONSUMER_AIRPORT);
		multiplyPerCard(fractionalScore, 1.4F, game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY);

		return static_cast<unsigned int>(fractionalScore);
	}

	case game::PlayerCard::PLAYERCARD_SPECIAL_LABORATORY: {
		unsigned int score = 0;

		if (hasCardAdjacency(game::PlayerCard::PLAYERCARD_SPECIAL_HEADQUARTERS)) {
			score += 50U;
		}

		if (hasCardAdjacency(game::PlayerCard::PLAYERCARD_SPECIAL_OUTPOST)) {
			score += 15U;
		}

		return score;
	}

	default:
		return 0U;
	}
}

bool game::PlayerCard::hasCardAdjacency(PlayerCard::CardType cardType) const {
	for (unsigned int i = 0U; i < 6U; ++i) {
		Adjacency adjacency = getAdjacency(i);

		if (adjacency.hasPlayerCardAdjacency && adjacency.cardType == cardType) {
			return true;
		}
	}

	return false;
}

bool game::PlayerCard::hasPOIAdjacency(PointOfInterest::POIType poiType) const {
	if (hasPOI && this->poiType == poiType) {
		return true;
	}

	for (unsigned int i = 0U; i < 6U; ++i) {
		Adjacency adjacency = getAdjacency(i);

		if (adjacency.hasPointOfInterestAdjacency && adjacency.poiType == poiType) {
			return true;
		}
	}

	return false;
}

bool game::PlayerCard::multiplyPerCard(float & fractionalScore, float multiplier, PlayerCard::CardType cardType) const {
	bool multiplied = false;

	for (unsigned int i = 0U; i < 6U; ++i) {
		Adjacency adjacency = getAdjacency(i);

		if (adjacency.hasPlayerCardAdjacency && adjacency.cardType == cardType) {
			fractionalScore *= multiplier;
			multiplied |= true;
		}
	}

	return multiplied;
}

bool game::PlayerCard::multiplyPerPOI(float & fractionalScore, float multiplier, PointOfInterest::POIType poiType) const {
	bool multiplied = false;
	if (hasPOI && this->poiType == poiType) {
		fractionalScore *= multiplier;
		multiplied |= true;
	}

	for (unsigned int i = 0U; i < 6U; ++i) {
		Adjacency adjacency = getAdjacency(i);

		if (adjacency.hasPointOfInterestAdjacency && adjacency.poiType == poiType) {
			fractionalScore *= multiplier;
			multiplied |= true;
		}
	}

	return multiplied;
}

game::Deck::Deck() {
	deckNameText->rotate(90.F);
	deckNameText->setFont(tgui::getGlobalFont());
	deckNameText->setColor(sf::Color::Black);

	cardIDText->setFont(tgui::getGlobalFont());
	cardIDText->setColor(sf::Color::Black);
	cardIDText->setCharacterSize(20U);
}

void game::Deck::draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const {
	renderStates.transform.translate(xStart, getPosition().y);

	for (std::size_t i = 0U; i < cards.size(); ++i) {
		if (clickedCard && i == selectedCardIndex) {
			renderStates.transform.translate(cardTextSpace, 0.F);
			continue;
		}

		game::PlayerCard fullCard(cards[i].cardType); //Encapsulate the card in a PlayerCard so we can get extra information like its string name.

		deckNameText->setString(fullCard.getStringName());
		deckNameText->setCharacterSize(cards[i].textSize);
		deckNameText->setOrigin(-(getSize().y - deckNameText->getSize().x) / 2.F, deckNameText->getSize().y + 5.F);

		if (hoveringOverCard && i == selectedCardIndex) {
			renderStates.transform.translate(0.F, -40.F);
		}

		Widget::drawRectangleShape(renderTarget, renderStates, tgui::Vector2f(cardWidth, getSize().y), fullCard.getBackColor());
		Widget::drawBorders(renderTarget, renderStates, { 1, 1, 1, 1 }, tgui::Vector2f(cardWidth, getSize().y), tgui::Color::Black);
		deckNameText->draw(renderTarget, renderStates);

		if (hoveringOverCard && i == selectedCardIndex || i == cards.size() - 1U) { //Draw hovered cards (pulled out)
			cardIDText->setString(std::to_string(cards[i].cardID));
			float xTranslation = cardWidth - (cardIDText->getLineWidth() + 5.F);
			renderStates.transform.translate(xTranslation, 5.F);

			cardIDText->draw(renderTarget, renderStates);
			renderStates.transform.translate(-xTranslation, (hoveringOverCard && i == selectedCardIndex ? 35.F : -5.F));
		}

		renderStates.transform.translate(cardTextSpace, 0.F);
	}

	if (clickedCard) { //Draw clicked card last so it's on top of everything else.
		game::PlayerCard fullCard(cards[selectedCardIndex].cardType);

		deckNameText->setString(fullCard.getStringName());
		deckNameText->setCharacterSize(cards[selectedCardIndex].textSize);
		deckNameText->setOrigin(-(getSize().y - deckNameText->getSize().x) / 2.F, deckNameText->getSize().y + 5.F);

		renderStates.transform = sf::Transform();
		renderStates.transform.translate(xStart + cardTextSpace * selectedCardIndex, getPosition().y - 40.F);

		Widget::drawRectangleShape(renderTarget, renderStates, tgui::Vector2f(cardWidth, getSize().y), fullCard.getBackColor());
		Widget::drawBorders(renderTarget, renderStates, { 1, 1, 1, 1 }, tgui::Vector2f(cardWidth, getSize().y), tgui::Color::Black);
		deckNameText->draw(renderTarget, renderStates);

		float xTranslation = cardWidth - (cardIDText->getLineWidth() + 5.F);
		cardIDText->setString(std::to_string(cards[selectedCardIndex].cardID));
		renderStates.transform.translate(xTranslation, 5.F);

		cardIDText->draw(renderTarget, renderStates);
		renderStates.transform.translate(-xTranslation, 35.F);
	}
}

void game::Deck::setSize(const tgui::Layout2d & size) {
	Widget::setSize(size);

	cardWidth = getSize().y * 0.75F + 10.F;
	cardTextSpace = cardWidth / 4.F;
	cardTextSize = cardTextSpace == 0.F ? 1U : tgui::Text::findBestTextSize(deckNameText->getFont(), cardTextSpace) / 2U;

	for (DeckCard & deckCard : cards) { //Resize all of the cards and update text sizes so everything fits snuggly.
		unsigned int textSize = cardTextSize;
		deckNameText->setString(game::PlayerCard(deckCard.cardType).getStringName());
		deckNameText->setCharacterSize(textSize);

		while (deckNameText->getLineWidth() + 10.F > getSize().y) {
			deckNameText->setCharacterSize(--textSize);

			if (cardTextSize == 0U) {
				break;
			}
		}

		deckCard.textSize = textSize;
	}

	xWidth = cardTextSpace * (cards.size() == 1U ? 0U : cards.size()) + getSize().y * 0.75F - 5.F;
	xStart = (getPosition().x + getSize().x - xWidth) / 2.F;
}

void game::Deck::mouseMoved(tgui::Vector2f position) {
	if (!clickedCard) {
		if (position.x >= xStart && position.x - (getPosition().x + xStart) <= xWidth) { //If the mouse is actually on a card.
			selectedCardIndex = std::min<std::size_t>(cards.size() - 1U, static_cast<std::size_t>((position.x - xStart) / cardTextSpace));
			hoveringOverCard |= true;
		}

		else {
			hoveringOverCard &= false;
		}
	}
}

void game::Deck::leftMouseReleased(tgui::Vector2f position) {
	if (hoveringOverCard) {
		clickedCard = true;
		hoveringOverCard = false;
	}

	else if (clickedCard) {
		if (position.x >= xStart && position.x - (getPosition().x + xStart) <= xWidth) { //If the mouse is actually on a card.
			std::size_t tempSelectedCardIndex = std::min<std::size_t>(cards.size() - 1U, static_cast<std::size_t>((position.x - xStart) / cardTextSpace));
			if (tempSelectedCardIndex == selectedCardIndex) {
				hoveringOverCard |= true;
				clickedCard = false;
			}

			selectedCardIndex = tempSelectedCardIndex;
		}

		else {
			hoveringOverCard &= false;
			clickedCard = false;
		}
	}
}

void game::Deck::shuffleDeck(std::mt19937_64 & randomEngine) { //Shuffle all cards randomly and update card number labels.
	std::shuffle(cards.begin(), cards.end(), randomEngine);

	for (std::size_t i = 0U; i < cards.size(); ++i) {
		cards[i].cardID = i;
	}
}

void game::Deck::addCard(game::PlayerCard::CardType cardType) {
	unsigned int textSize = cardTextSize;
	deckNameText->setString(game::PlayerCard(cardType).getStringName());
	deckNameText->setCharacterSize(textSize);

	while (deckNameText->getLineWidth() + 10.F > getSize().y) {
		deckNameText->setCharacterSize(--textSize);

		if (textSize == 0U) {
			break;
		}
	}

	DeckCard deckCard;
	deckCard.cardType = cardType;
	deckCard.textSize = textSize;
	deckCard.cardID = cards.size();

	//Calculate new card area.
	xWidth = cardTextSpace * (cards.size() == 1U ? 0U : cards.size()) + getSize().y * 0.75F - 5.F;
	xStart = (getPosition().x + getSize().x - xWidth) / 2.F;
	cards.push_back(deckCard);
}

game::PlayerCard::CardType game::Deck::useCard() {
	game::PlayerCard::CardType selectedCard = cards[selectedCardIndex].cardType;
	cards.erase(cards.cbegin() + selectedCardIndex);

	clickedCard = false;
	xWidth = cardTextSpace * cards.size() + getSize().y * 0.75F - 5.F;
	xStart = (getPosition().x + getSize().x - xWidth) / 2.F;
	
	return selectedCard;
}

void game::Deck::clearDeck() {
	cards.clear();

	hoveringOverCard = false;
	clickedCard = false;
	selectedCardIndex = 0U;
	xWidth = 0.F;
	xStart = 0.F;
}

game::GameScene::GameScene() {
	hoverVertexBuffer = sf::VertexBuffer(sf::PrimitiveType::TriangleFan, sf::VertexBuffer::Dynamic);
	playerCardBackgroundVertexBuffer = sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Stream);
	playerCardImageVertexBuffer = sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Stream);
	pointOfInterestVertexBuffer = sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Stream);

	hoverColor = sf::Color(255U, 255U, 255U, 198U);
	goodHoverColor = sf::Color(0U, 255U, 0U, 198U);
	badHoverColor = sf::Color(255U, 0U, 0U, 198U);

	cardDealerDistribution = std::uniform_real_distribution<float>(0.F, std::logf(PlayerCard::PLAYERCARD_COUNT - 3.F));

	font.loadFromFile("content\\fonts\\arial narrow.ttf");
	placementScoreIndicator = sf::Text();
	placementScoreIndicator.setFont(font);
	placementScoreIndicator.setFillColor(sf::Color::Yellow);

	poiGeneratorDistribution = std::uniform_real_distribution<float>(0.F, std::logf(PointOfInterest::POINTOFINTEREST_COUNT) / std::logf(1.4F));
	hoverVertexBuffer.create(13U);

	//Load shaders.
	backgroundRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, sf::Shader::Type::Vertex); 
	playerCardBackgroundRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, resources::SHADERFRAG_VERTEXCOLOR_MEM);
	playerCardImageRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, sf::Shader::Type::Vertex);
	hoverRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, resources::SHADERFRAG_HOVER_MEM);
	pointOfInterestRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, sf::Shader::Type::Vertex);
	backgroundParticleManager.managerRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, sf::Shader::Type::Vertex);
	foregroundParticleManager.managerRenderStates.getShader()->loadFromMemory(resources::SHADERVERT_OFFSET_MEM, sf::Shader::Type::Vertex);

	hoverRenderStates.getShader()->setUniform("secondMultiplier", 3.5F / resources::GAME_RADIALANIMATION_SECONDS);

	//Load images
	if (!playerCardImageRenderStates.getTexture()->loadFromFile(resources::STRING_TEXTURE_PLAYERCARDS_LOCATION)) {
		win32::createErrorBox("ERROR", "Error when creating hexagon image atlas!");
	}

	if (!pointOfInterestRenderStates.getTexture()->loadFromFile(resources::STRING_TEXTURE_POINTSOFINTEREST_LOCATION)) {
		win32::createErrorBox("ERROR", "Error when reading points of interest texture atlas!");
	}

	if (!backgroundParticleManager.managerRenderStates.getTexture()->loadFromFile(resources::STRING_TEXTURE_PARTICLES_LOCATION)) {
		win32::createErrorBox("ERROR", "Error when creating particle texture atlas!");
	}

	if (!foregroundParticleManager.managerRenderStates.getTexture()->loadFromFile(resources::STRING_TEXTURE_PARTICLES_LOCATION)) {
		win32::createErrorBox("ERROR", "Error when creating particle texture atlas!");
	}

	updateBackgroundColors(sf::Color(38U, 38U, 38U, 255U), sf::Color(48U, 48U, 48U, 255U), sf::Color(255U, 255U, 255U, 255U), sf::Color(86U, 86U, 86U, 255U));
	deck->addCard(game::PlayerCard::CardType::PLAYERCARD_SPECIAL_HEADQUARTERS);
}

game::GameScene::GameScene(const std::string & seed, std::size_t deckCount) : GameScene() { //Create a game with a preset seed and deck count.
	this->seed = seed;
	worldSeed = std::hash<std::string>()(seed);
	seedSet = true;

	maximumDeckCount = deckCount;
	cardPlacementHistory = std::make_unique<std::vector<engine::CardPlacement>[]>(deckCount + 1U);
}

void game::GameScene::update(float deltaSeconds) {
	Scene::update(deltaSeconds);

	//Update particle managers.
	backgroundParticleManager.update(deltaSeconds); 
	foregroundParticleManager.update(deltaSeconds);

	if (hasMouse && anyMButtonDown) {
		if (hoverHoldTime < resources::GAME_RADIALANIMATION_SECONDS) {
			hoverHoldTime += deltaSeconds;
			hoverRenderStates.getShader()->setUniform("secondsHeldDown", hoverHoldTime);
		}

		else if (validPlacementHover && deck->hasSelectedCard() && leftMButtonDown && hoverHoldTime >= resources::GAME_RADIALANIMATION_SECONDS) {
			placeSelectedCard(coordinateHoverLocation);

			validPlacementHover = false;
			updateHoverColors();
			collectWorldHexagons();
		}
	}
}

void game::GameScene::draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const {
	renderTarget.clear(backgroundColor);
	if (currentZoomLevel != nullptr) {
		renderTarget.draw(*currentZoomLevel->getVertexBuffer(), backgroundRenderStates);
	}

	renderTarget.draw(backgroundParticleManager);
	renderTarget.draw(pointOfInterestVertexBuffer, pointOfInterestRenderStates);
	renderTarget.draw(playerCardBackgroundVertexBuffer, playerCardBackgroundRenderStates);
	renderTarget.draw(playerCardImageVertexBuffer, playerCardImageRenderStates);
	renderTarget.draw(foregroundParticleManager);

	if (hasMouse) {
		renderTarget.draw(hoverVertexBuffer, hoverRenderStates);
	}

	if (validPlacementHover && deck->hasSelectedCard() && !(deck->getSelectedCardType() == PlayerCard::PLAYERCARD_SPECIAL_OUTPOST || deck->getSelectedCardType() == PlayerCard::PLAYERCARD_SPECIAL_HEADQUARTERS)) {
		renderTarget.draw(placementScoreIndicator);
	}

	if (guiManager) {
		guiManager->draw();
	}
}

void game::GameScene::mouseMoved(int x, int y) {
	Scene::mouseMoved(x, y);

	static sf::Vector2f worldGenerationDelta = sf::Vector2f();
	bool currentlyHasMouse = letterBox.contains(sf::Vector2i(x, y));
	mouseWorldLocation = static_cast<sf::Vector2f>(mouseLocation + mouseOffset) * mouseMultiplier;

	if (hasMouse != currentlyHasMouse) {
		if (hasMouse) { 
			hoverHoldTime = 0.F;
		}

		hasMouse = currentlyHasMouse;
	}

	if (middleMButtonDown) {
		sf::Vector2f moveDelta = scrollStart - mouseWorldLocation;

		screenHoverLocation -= moveDelta;
		fullScroll -= moveDelta;
		scrollStart = mouseWorldLocation;

		updateScroll();
		placementScoreIndicator.setPosition(placementScoreIndicator.getPosition() - moveDelta);

		worldGenerationDelta.x += std::fabsf(moveDelta.x);
		worldGenerationDelta.y += std::fabsf(moveDelta.y);

		if (worldGenerationDelta.x > backgroundOutsideHexagon.getRepeatX() || worldGenerationDelta.y > backgroundOutsideHexagon.getRepeatY()) {
			free(visibleHexagonMaps);
			getVisibleMaps(visibleHexagonMaps, hexagonMapCount);

			generateVisibleWorld();
			collectWorldHexagons();

			worldGenerationDelta.x = 0.F;
			worldGenerationDelta.y = 0.F;
		}
	}

	else {
		sf::Vector2i newHoverCoordinates = backgroundOutsideHexagon.mapCoordinates(mouseWorldLocation - fullScroll);

		if (newHoverCoordinates.x != coordinateHoverLocation.x || newHoverCoordinates.y != coordinateHoverLocation.y) {
			updateHoverVertices();
			collectWorldHexagons();
			
			if (deck->hasSelectedCard()) {
				placementScoreIndicator.setString(std::to_string(calculatePotentialScore(coordinateHoverLocation, deck->getSelectedCardType())));
			}

			sf::FloatRect textBounds = placementScoreIndicator.getLocalBounds();
			placementScoreIndicator.setOrigin(textBounds.left + textBounds.width / 2.F, textBounds.top + textBounds.height / 2.F);
			placementScoreIndicator.setPosition(screenHoverLocation + backgroundOutsideHexagon.getVectors()[6U]);

			hoverHoldTime = 0.F;
			updateHoverColors();
		}
	}
}

void game::GameScene::mousePressed(sf::Mouse::Button buttonPressed) {
	if (!middleMButtonDown && buttonPressed == sf::Mouse::Button::Middle) {
		scrollStart = mouseWorldLocation;
	}

	Scene::mousePressed(buttonPressed);

	if (buttonPressed == sf::Mouse::Button::Left) {
		updateHoverColors();
	}
}

void game::GameScene::mouseReleased(sf::Mouse::Button buttonReleased) {
	Scene::mouseReleased(buttonReleased);

	if (!anyMButtonDown) {
		hoverHoldTime = 0.F;
		hoverRenderStates.getShader()->setUniform("secondsHeldDown", hoverHoldTime);
	}
}

void game::GameScene::mouseWheelScrolled(float scrollDelta) {
	Scene::mouseWheelScrolled(scrollDelta);
	bool negativeDelta = std::signbit(scrollDelta);

	if ((negativeDelta && cumulativeZoom + scrollDelta < resources::GAME_MINIMUMZOOM) || (!negativeDelta && cumulativeZoom + scrollDelta > resources::GAME_MAXIMUMZOOM)) {
		return; //We're at or will go beyond the zoom limits, the player cannot complete the zoom.
	}

	float scaleFactor = std::powf(resources::GAME_ZOOMMULTIPLIER, scrollDelta);
	cumulativeZoom += scrollDelta;
	foregroundParticleManager.zoomManager(scaleFactor);
	backgroundParticleManager.zoomManager(scaleFactor);
	fullScroll += (mouseWorldLocation - fullScroll) * (1.F - scaleFactor);

	updateScroll();
	placementScoreIndicator.setCharacterSize(static_cast<int>(std::floorf(backgroundInsideHexagon.getHeight() / 2.F)));

	sf::FloatRect textBounds = placementScoreIndicator.getLocalBounds();
	placementScoreIndicator.setOrigin(textBounds.left + textBounds.width / 2.F, textBounds.top + textBounds.height / 2.F);
	placementScoreIndicator.setPosition(screenHoverLocation + backgroundOutsideHexagon.getVectors()[6U]);

	generateVisibleWorld();
	collectWorldHexagons();
}

void game::GameScene::keyReleased(const sf::Event::KeyEvent & releasedEvent) {
	if (releasedEvent.code == sf::Keyboard::Key::H && decksDealt > 0U) { //Center the game on the headquarters card. 'H' for 'home'
		sf::Vector2f headquartersCenter = -(backgroundOutsideHexagon.worldCoordinates(headquartersCoordinates) + backgroundOutsideHexagon.getVectors()[6U] + (headquartersCoordinates.x & 1 ? -backgroundOutsideHexagon.getVectors()[5U] : backgroundOutsideHexagon.getVectors()[5U]));

		fullScroll = headquartersCenter + static_cast<sf::Vector2f>(backBufferSize) / 2.F;
		updateScroll();

		sf::FloatRect textBounds = placementScoreIndicator.getLocalBounds();
		placementScoreIndicator.setOrigin(textBounds.left + textBounds.width / 2.F, textBounds.top + textBounds.height / 2.F);
		placementScoreIndicator.setPosition(screenHoverLocation + backgroundOutsideHexagon.getVectors()[6U]);

		generateVisibleWorld();
		collectWorldHexagons();
	}

	switch (releasedEvent.code) {
	case sf::Keyboard::Key::D: 
		if (deck->hasSelectedCard() && decksDealt > 0U) {
			deck->useCard();

			if (deck->isEmpty()) {
				drawNewDeck();
			}
		}

		break;

	case sf::Keyboard::Key::Escape:
		menuButton->leftMousePressed(menuButton->getPosition());
		menuButton->leftMouseReleased(menuButton->getPosition());
		break;

	case sf::Keyboard::Key::F1:
		if (!menuUnderlayPanel->isVisible()) {
			hudTopPanelLeft->setVisible(!hudTopPanelLeft->isVisible());
			hudTopPanelRight->setVisible(!hudTopPanelRight->isVisible());
			hudBottomPanel->setVisible(!hudBottomPanel->isVisible());
			scoreLabel->setVisible(!scoreLabel->isVisible());
			decksDealtLabel->setVisible(!decksDealtLabel->isVisible());
			menuButton->setVisible(!menuButton->isVisible());
			deck->setVisible(!deck->isVisible());
		}
		
		break;
	}
}

void game::GameScene::letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) {
	SimpleGUIScene::letterBoxAdjusted(view, letterBox);

	mouseOffset.x = -letterBoxOffset.x;
	mouseOffset.y = -letterBoxOffset.y;
	mouseMultiplier = static_cast<float>(backBufferSize.x) / static_cast<float>(letterBoxSize.x);
	hoverRenderStates.getShader()->setUniform("radius", ((backgroundOutsideHexagon.getSideLength() * 1.25F) / mouseMultiplier));
}

void game::GameScene::pumpEvent(const sf::Event & event) {
	if (guiManager.get() != nullptr) {
		if (guiManager->handleEvent(event)) { //If the guimanager handled the event, I still want to perform scrolls for ease of use.
			if (event.type == sf::Event::EventType::MouseMoved && menuUnderlayPanel && !menuUnderlayPanel->isVisible()) {
				mouseMoved(event.mouseMove.x, event.mouseMove.y);
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

void game::GameScene::activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) {
	SimpleGUIScene::activated(renderTarget, viewport);
	
	letterBoxAdjusted(renderTarget->getView(), viewport);	
	defaultSideLength = std::min<unsigned int>(backBufferSize.x, backBufferSize.y) / 25.F;

	backgroundOutsideHexagon.generateHexagon(defaultSideLength);
	backgroundInsideHexagon.generateHexagon(defaultSideLength * resources::GAME_INSIDEHEXAGON_PERCENTAGE);

	calculatedZoomLevels.clear();
	updateBackgroundVertices();
	updateHoverVertices();

	placementScoreIndicator.setCharacterSize(static_cast<int>(std::floorf(backgroundInsideHexagon.getHeight() / 2.F)));
	if (seedSet) {
		generateVisibleWorld();
		collectWorldHexagons();
	}

	hudTopPanelLeft = tgui::Panel::create();
	hudTopPanelLeft->setRenderer(guiTheme->getRenderer("hudpanel"));
	guiManager->add(hudTopPanelLeft, "hudTopPanelLeft");

	hudTopPanelRight = tgui::Panel::create();
	hudTopPanelRight->setRenderer(guiTheme->getRenderer("hudpanel"));
	guiManager->add(hudTopPanelRight, "hudTopPanelRight");

	hudBottomPanel = tgui::Panel::create();
	hudBottomPanel->setRenderer(guiTheme->getRenderer("hudpanel"));
	guiManager->add(hudBottomPanel, "hudBottomPanel");

	scoreLabel = tgui::Label::create(std::string("Score: ") + std::to_string(gameScore));
	scoreLabel->setRenderer(guiTheme->getRenderer("hudlabel"));
	guiManager->add(scoreLabel, "scoreLabel");

	decksDealtLabel = tgui::Label::create(std::string("Decks Dealt: ") + std::to_string(decksDealt) + std::string(" out of ") + std::to_string(maximumDeckCount));
	decksDealtLabel->setRenderer(guiTheme->getRenderer("hudlabel"));
	guiManager->add(decksDealtLabel, "decksDealtLabel");

	menuButton = tgui::Button::create("Menu (ESC)");
	menuButton->setTextSize(0U);
	menuButton->setRenderer(guiTheme->getRenderer("button"));
	menuButton->getRenderer()->setOpacity(0.5F);
	menuButton->connect("pressed", [&]() {
		deck->setEnabled(false);
		
		menuTitleLabel->setVisible(true);
		menuUnderlayPanel->setVisible(true);
		menuBackgroundPanel->setVisible(true);
		menuSeedLabel->setVisible(true);
		menuReturnToGameButton->setVisible(true);
		menuSaveButton->setVisible(true);
		menuGotoMainMenuButton->setVisible(true);
	});
	guiManager->add(menuButton, "menuButton");

	deck->setEnabled(true); //SaveScene call disables deck, may as well enable it here just incase.
	guiManager->add(deck, "deck");

	menuTitleLabel = tgui::Label::create("Menu");
	menuTitleLabel->setVisible(false);
	menuTitleLabel->setTextSize(50U);
	menuTitleLabel->ignoreMouseEvents(true);
	menuTitleLabel->setPosition("(&.w - w) / 2", "8%");
	menuTitleLabel->setRenderer(guiTheme->getRenderer("titlelabel"));
	guiManager->add(menuTitleLabel, "menuLabel");

	menuUnderlayPanel = tgui::Panel::create();
	menuUnderlayPanel->setVisible(false);
	menuUnderlayPanel->setRenderer(guiTheme->getRenderer("hudpanel"));
	guiManager->add(menuUnderlayPanel, "menuUnderlayPanel");

	menuBackgroundPanel = tgui::Panel::create();
	menuBackgroundPanel->setVisible(false);
	menuBackgroundPanel->setRenderer(guiTheme->getRenderer("backgroundpanel"));
	guiManager->add(menuBackgroundPanel, "menuPanel");

	menuSeedLabel = tgui::Label::create(std::string("Seed: \"") + seed + std::string("\""));
	menuSeedLabel->setVisible(false);
	menuSeedLabel->setRenderer(guiTheme->getRenderer("label"));
	guiManager->add(menuSeedLabel, "menuSeedLabel");

	menuReturnToGameButton = tgui::Button::create("Return To Game");
	menuReturnToGameButton->setTextSize(0U);
	menuReturnToGameButton->setVisible(false);
	menuReturnToGameButton->setRenderer(guiTheme->getRenderer("button"));
	menuReturnToGameButton->connect("pressed", [&]() {
		deck->setEnabled(true);

		menuTitleLabel->setVisible(false);
		menuUnderlayPanel->setVisible(false);
		menuBackgroundPanel->setVisible(false);
		menuSeedLabel->setVisible(false);
		menuReturnToGameButton->setVisible(false);
		menuSaveButton->setVisible(false);
		menuGotoMainMenuButton->setVisible(false);
	});
	guiManager->add(menuReturnToGameButton, "menuReturnToGameButton");

	menuSaveButton = tgui::Button::create("Save Game");
	menuSaveButton->setTextSize(0U);
	menuSaveButton->setVisible(false);
	menuSaveButton->setRenderer(guiTheme->getRenderer("button"));
	menuSaveButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::SaveScene(this), false); 
	});
	guiManager->add(menuSaveButton, "menuSaveButton");

	menuGotoMainMenuButton = tgui::Button::create("Goto Main Menu");
	menuGotoMainMenuButton->setTextSize(0U);
	menuGotoMainMenuButton->setVisible(false);
	menuGotoMainMenuButton->setRenderer(guiTheme->getRenderer("button"));
	menuGotoMainMenuButton->connect("pressed", [&]() {
		engine::setCurrentScene(new scene::MainMenuScene(), true);
	});
	guiManager->add(menuGotoMainMenuButton, "menuGotoMenuButton");

	hudTopPanelLeft->setSize("80% - 10", "3% + 10");

	hudTopPanelRight->setPosition("80% - 10", 0.F);
	hudTopPanelRight->setSize("20% + 10", "5% + 10");

	hudBottomPanel->setPosition(10.F, "&.h - (30% + 10)");
	hudBottomPanel->setSize("&.w - 20", "30%");

	scoreLabel->setPosition(5.F, 5.F);
	scoreLabel->setSize(0.F, "hudTopPanelLeft.h - 10");
	scoreLabel->setTextSize(tgui::Text::findBestTextSize(tgui::getGlobalFont(), scoreLabel->getSize().y));
	scoreLabel->setAutoSize(true);

	decksDealtLabel->setPosition("scoreLabel.x + scoreLabel.w + 5", 5.F);
	decksDealtLabel->setTextSize(scoreLabel->getTextSize());

	menuButton->setPosition("hudTopPanelRight.x + 5", "hudTopPanelRight.y + 5");
	menuButton->setSize("20%", "5%");

	deck->setPosition("hudBottomPanel.x + 5", "hudBottomPanel.y + 5");
	deck->setSize("hudBottomPanel.w - 10", "hudBottomPanel.h - 10");

	menuBackgroundPanel->setSize("45%", "25%");

	menuSeedLabel->setTextSize(24U);
	while (menuSeedLabel->getSize().x > menuBackgroundPanel->getSize().x) {
		menuSeedLabel->setTextSize(menuSeedLabel->getTextSize() - 1U);
	}

	menuBackgroundPanel->setSize("45%", "30%");
	menuBackgroundPanel->setPosition("(&.w - w) / 2", "(&.h - h) / 2");
	menuSeedLabel->setPosition("menuPanel.x + (menuPanel.w - w) / 2", "menuPanel.y + 5");

	menuReturnToGameButton->setPosition("menuPanel.x + 5", "menuSeedLabel.y + menuSeedLabel.h");
	menuReturnToGameButton->setSize("menuPanel.w - 10", "(menuPanel.h - (menuSeedLabel.h + 20)) / 3");
	
	menuSaveButton->setPosition("menuPanel.x + 5", "menuReturnToGameButton.y + menuReturnToGameButton.h + 5");
	menuSaveButton->setSize("menuPanel.w - 10", "(menuPanel.h - (menuSeedLabel.h + 20)) / 3");

	menuGotoMainMenuButton->setPosition("menuPanel.x + 5", "menuSaveButton.y + menuSaveButton.h + 5");
	menuGotoMainMenuButton->setSize("menuPanel.w - 10", "(menuPanel.h - (menuSeedLabel.h + 20)) / 3");
}

void game::GameScene::deactivated(sf::RenderTarget * renderTarget) {
	hudTopPanelLeft.reset();
	hudTopPanelRight.reset();
	hudBottomPanel.reset();
	scoreLabel.reset();
	decksDealtLabel.reset();
	menuButton.reset();
	menuTitleLabel.reset();
	menuUnderlayPanel.reset();
	menuBackgroundPanel.reset();
	menuSeedLabel.reset();
	menuReturnToGameButton.reset();
	menuSaveButton.reset();
	menuGotoMainMenuButton.reset();

	SimpleGUIScene::deactivated(renderTarget);
}

bool game::GameScene::saveGame(const std::wstring & fileName) {
	static std::string singleIndent    ("  ");
	static std::string doubleIndent    ("    ");
	static std::string tripleIndent    ("      ");
	static std::string quadrupleIndent ("        ");
	static std::string quintupleIndent ("          ");
	static std::string sextupleIndent  ("            ");

	static std::string worldSeedString("\"world_seed\": ");
	static std::string cumulativeZoomString("\"cumulative_zoom\": ");
	static std::string maximumDeckCountString("\"maximum_deck_count\": ");
	static std::string xString("\"x\": ");
	static std::string yString("\"y\": ");
	static std::string timeStampString("\"time_stamp\": ");
	static std::string idString("\"id\": ");
	static std::string pointOfInterestIDString("\"pointofinterest_id\": ");

	std::ofstream fileStream;
	fileStream.open(resources::PATH_SAVEGAME / fileName, std::ios::out | std::ios::trunc); //Open the save file - clearing it if it already exists.

	if (!fileStream.good()) { //Did everything go fine?
		fileStream.close();
		return false;
	}

	engine::JSONArrayBuilder rootArrayBuilder;
	fileStream << "{" << std::endl;

	//The save creator only writes what is absolutely necessary to the save. For example if the player hasn't scrolled at all (camera is at (0, 0)) the 
	//load game procedure correctly assumes this. Not only does this save space - but it also makes saving quicker.

	{
		//Automatically escapes any speech marks in the world seed to prevent crashes when loading the save later.
		//"Hello" becomes "\"Hello\"" in the save file, which the JSON parser can understand as "Hello" properly.
		std::ostringstream worldSeedStream;
		worldSeedStream << std::quoted(seed);

		rootArrayBuilder.append(singleIndent + worldSeedString + worldSeedStream.str());
	}

	if (cumulativeZoom != 0.F) {
		rootArrayBuilder.append(singleIndent + cumulativeZoomString + std::to_string(cumulativeZoom));
	}

	rootArrayBuilder.append(singleIndent + maximumDeckCountString + std::to_string(maximumDeckCount));

	if (fullScroll.x != 0.F || fullScroll.y != 0.F) {
		std::ostringstream cameraCoordinatesBuilder;
		engine::JSONArrayBuilder cameraCoordinatesArrayBuilder;

		cameraCoordinatesBuilder << singleIndent << "\"camera_coordinates\": {" << std::endl;

		if (fullScroll.x != 0.F) {
			cameraCoordinatesArrayBuilder.append(doubleIndent + xString + std::to_string(fullScroll.x));
		}

		if (fullScroll.y != 0.F) {
			cameraCoordinatesArrayBuilder.append(doubleIndent + yString + std::to_string(fullScroll.y));
		}

		cameraCoordinatesArrayBuilder.flush(cameraCoordinatesBuilder);

		cameraCoordinatesBuilder << singleIndent << "}";
		rootArrayBuilder.append(cameraCoordinatesBuilder.str());
	}

	bool hasPlacementHistory = false;
	for (std::size_t i = 0U; i < decksDealt; ++i) {
		if (cardPlacementHistory[i].size() > 0U) {
			hasPlacementHistory = true;
			break;
		}
	}

	if (hasPlacementHistory) {
		std::ostringstream cardPlacementHistoryBuilder;
		engine::JSONArrayBuilder cardPlacementHistoryArrayBuilder;

		cardPlacementHistoryBuilder << singleIndent << "\"card_placements\": [" << std::endl;

		for (std::size_t i = 0U; i <= decksDealt; ++i) {
			std::vector<engine::CardPlacement> & turnHistory = cardPlacementHistory[i];
			std::ostringstream cardPlacementsBuilder;
			engine::JSONArrayBuilder cardPlacementsArrayBuilder;

			cardPlacementsBuilder << doubleIndent << "[" << std::endl;

			std::cout << turnHistory.size() << std::endl;

			for (engine::CardPlacement cardPlacement : turnHistory) {
				std::ostringstream cardPlacementBuilder;
				engine::JSONArrayBuilder cardPlacementArrayBuilder;

				GameHexagonMap * parentHexagonMap;
				getParentMap(parentHexagonMap, cardPlacement.coordinates);
				game::PlayerCard & playerCard = parentHexagonMap->getPlayerCard(parentHexagonMap->queryMap(parentHexagonMap->asLocalCoordinatesU(cardPlacement.coordinates)).playerCardIndex);

				cardPlacementBuilder << tripleIndent << "{" << std::endl;
				cardPlacementArrayBuilder.append(quadrupleIndent + timeStampString + std::to_string(cardPlacement.timeSeconds));

				if (playerCard.hasPointOfInterest()) {
					cardPlacementArrayBuilder.append(quadrupleIndent + pointOfInterestIDString + std::to_string(playerCard.getPointOfInterest()));
				}
				
				if (cardPlacement.cardID != game::PlayerCard::CardType::PLAYERCARD_SPECIAL_HEADQUARTERS) {
					cardPlacementArrayBuilder.append(quadrupleIndent + idString + std::to_string(cardPlacement.cardID));
				}

				if (cardPlacement.coordinates.x != 0 || cardPlacement.coordinates.y != 0) {
					std::ostringstream cardCoordinatesBuilder;
					engine::JSONArrayBuilder cardCoordinatesArrayBuilder;

					cardCoordinatesBuilder << quadrupleIndent << "\"coordinates\": {" << std::endl;

					if (cardPlacement.coordinates.x != 0) {
						cardCoordinatesArrayBuilder.append(quintupleIndent + xString + std::to_string(cardPlacement.coordinates.x));
					}

					if (cardPlacement.coordinates.y != 0) {
						cardCoordinatesArrayBuilder.append(quintupleIndent + yString + std::to_string(cardPlacement.coordinates.y));
					}

					cardCoordinatesArrayBuilder.flush(cardCoordinatesBuilder);

					cardCoordinatesBuilder << quadrupleIndent << "}";
					cardPlacementArrayBuilder.append(cardCoordinatesBuilder.str());
				}

				engine::JSONArrayBuilder cardAdjacenciesIndexArrayBuilder;
				for (unsigned int i = 0U; i < 6U; ++i) {
					game::PlayerCard::Adjacency indexedAdjacency = playerCard.getAdjacency(i);

					if (indexedAdjacency.hasPointOfInterestAdjacency) {
						std::ostringstream cardAdjacencyBuilder;
						engine::JSONArrayBuilder cardAdjacencyArrayBuilder;

						cardAdjacencyBuilder << quintupleIndent << "{" << std::endl;
						cardAdjacencyArrayBuilder.append(sextupleIndent + idString + std::to_string(i));

						if (indexedAdjacency.hasPointOfInterestAdjacency) {
							cardAdjacencyArrayBuilder.append(sextupleIndent + pointOfInterestIDString + std::to_string(indexedAdjacency.poiType));
						}

						cardAdjacencyArrayBuilder.flush(cardAdjacencyBuilder);

						cardAdjacencyBuilder << quintupleIndent << "}";
						cardAdjacenciesIndexArrayBuilder.append(cardAdjacencyBuilder.str());
					}
				}

				if (cardAdjacenciesIndexArrayBuilder.size() > 0U) {
					std::ostringstream cardAdjacenciesBuilder;
					cardAdjacenciesBuilder << quadrupleIndent << "\"adjacencies\": [" << std::endl;

					cardAdjacenciesIndexArrayBuilder.flush(cardAdjacenciesBuilder);

					cardAdjacenciesBuilder << quadrupleIndent << "]";
					cardPlacementArrayBuilder.append(cardAdjacenciesBuilder.str());
				}

				cardPlacementArrayBuilder.flush(cardPlacementBuilder);

				cardPlacementBuilder << tripleIndent << "}";
				cardPlacementsArrayBuilder.append(cardPlacementBuilder.str());
			}

			cardPlacementsArrayBuilder.flush(cardPlacementsBuilder);

			if (cardPlacementsArrayBuilder.size() > 0U) {
				cardPlacementsBuilder << doubleIndent << "]";
				cardPlacementHistoryArrayBuilder.append(cardPlacementsBuilder.str());
			}

			else { //Denote that a turn happened here, just that nothing was placed.
				cardPlacementHistoryArrayBuilder.append(doubleIndent + std::string("null"));
			}
		}

		cardPlacementHistoryArrayBuilder.flush(cardPlacementHistoryBuilder);

		cardPlacementHistoryBuilder << singleIndent << "]";
		rootArrayBuilder.append(cardPlacementHistoryBuilder.str());
	}

	if (!deck->isEmpty()) {
		std::ostringstream remainingDeckBuilder;
		engine::JSONArrayBuilder remainingDeckArrayBuilder;

		remainingDeckBuilder << singleIndent << "\"remaining_deck\": [" << std::endl;

		for (game::Deck::DeckCard deckCard : *deck) {
			remainingDeckArrayBuilder.append(doubleIndent + std::to_string(deckCard.cardType));
		}

		remainingDeckArrayBuilder.flush(remainingDeckBuilder);

		remainingDeckBuilder << singleIndent << "]";
		rootArrayBuilder.append(remainingDeckBuilder.str());
	}

	rootArrayBuilder.flush(fileStream);
	fileStream << "}" << std::endl;
	fileStream.close();

	return !fileStream.fail();
}

bool game::GameScene::loadGame(const std::wstring & fileName) {
	std::ifstream fileStream;
	fileStream.open(resources::PATH_SAVEGAME / fileName, std::ios::in); //Open the save file

	if (!fileStream.good()) { //Did that go ok?
		fileStream.close();
		return false;
	}

	try {
		//Convert the json in the save file to a tree.
		//If an error occurs it is caught and the method returns false.

		qjson::value saveRoot(fileStream); 

		if (saveRoot.has("world_seed")) {
			qjson::value worldSeed = saveRoot["world_seed"];

			if (worldSeed.isString()) {
				this->seed = worldSeed.string();
				this->worldSeed = std::hash<std::string>()(seed);
				
				menuSeedLabel->setText(std::string("Seed: \"") + seed + std::string("\""));
				seedSet = true;
			}
		}

		if (saveRoot.has("cumulative_zoom")) {
			qjson::value cumulativeZoom = saveRoot["cumulative_zoom"];

			if (cumulativeZoom.isNumber()) {
				mouseWheelScrolled(cumulativeZoom.asFloat());
			}
		}

		if (saveRoot.has("maximum_deck_count")) {
			qjson::value maximumDeckCount = saveRoot["maximum_deck_count"];

			if (maximumDeckCount.isNumber()) {
				this->maximumDeckCount = maximumDeckCount.asInt();
				cardPlacementHistory = std::make_unique<std::vector<engine::CardPlacement>[]>(this->maximumDeckCount + 1U);
			}
		}

		if (saveRoot.has("camera_coordinates")) {
			qjson::value cameraCoordinates = saveRoot["camera_coordinates"];

			if (cameraCoordinates.has("x")) {
				qjson::value cameraCoordinatesX = cameraCoordinates["x"];

				if (cameraCoordinatesX.isNumber()) {
					fullScroll.x = cameraCoordinatesX.asFloat();
				}
			}

			if (cameraCoordinates.has("y")) {
				qjson::value cameraCoordinatesY = cameraCoordinates["y"];

				if (cameraCoordinatesY.isNumber()) {
					fullScroll.y = cameraCoordinatesY.asFloat();
				}
			}
		}

		if (saveRoot.has("card_placements")) {
			qjson::value cardPlacements = saveRoot["card_placements"];

			if (cardPlacements.isList()) {
				qjson::value_list turnPlacementList = cardPlacements.list();
	
				if (!turnPlacementList.empty()) {
					hexagonMaps.clear();
					visibleHexagonMaps = nullptr;
					decksDealt = 0U;
					hexagonMapCount = 0U;
					gameScore = 0U;

					for (std::size_t i = 0U; i < turnPlacementList.size(); ++i) {
						qjson::value & turnCardPlacements = *turnPlacementList[i];

						if (turnCardPlacements.isList()) {
							qjson::value_list turnCardPlacementList = turnCardPlacements.list();

							if (!turnCardPlacementList.empty()) {
								for (qjson::value_ptr & savedCardPlacementPointer : turnCardPlacementList) {
									qjson::value savedCardPlacement = *savedCardPlacementPointer;

									engine::CardPlacement cardPlacement;
									cardPlacement.cardID = game::PlayerCard::CardType::PLAYERCARD_SPECIAL_HEADQUARTERS;
									
									if (savedCardPlacement.has("time_stamp")) {
										qjson::value cardTimeStamp = savedCardPlacement["time_stamp"];

										if (cardTimeStamp.isNumber()) {
											cardPlacement.timeSeconds = cardTimeStamp.asFloat();
										}
									}

									if (savedCardPlacement.has("id")) {
										qjson::value cardID = savedCardPlacement["id"];

										if (cardID.isNumber()) {
											cardPlacement.cardID = static_cast<game::PlayerCard::CardType>(static_cast<unsigned short>(cardID.asInt()));
										}
									}

									if (savedCardPlacement.has("coordinates")) {
										qjson::value cardCoordinates = savedCardPlacement["coordinates"];

										if (cardCoordinates.has("x")) {
											qjson::value cardCoordinatesX = cardCoordinates["x"];

											if (cardCoordinatesX.isNumber()) {
												cardPlacement.coordinates.x = cardCoordinatesX.asInt();
											}
										}

										if (cardCoordinates.has("y")) {
											qjson::value cardCoordinatesY = cardCoordinates["y"];

											if (cardCoordinatesY.isNumber()) {
												cardPlacement.coordinates.y = cardCoordinatesY.asInt();
											}
										}
									}

									game::PlayerCard & newPlayerCard = placePlayerCard(cardPlacement.coordinates, static_cast<game::PlayerCard::CardType>(cardPlacement.cardID));
									if (savedCardPlacement.has("pointofinterest_id")) {
										qjson::value pointOfInterestID = savedCardPlacement["pointofinterest_id"];

										if (pointOfInterestID.isNumber()) {
											newPlayerCard.setHasPointOfInterest(true);
											newPlayerCard.setPointOfInterest(static_cast<game::PointOfInterest::POIType>(static_cast<unsigned short>(pointOfInterestID.asInt())));
										}
									}

									if (savedCardPlacement.has("adjacencies")) {
										qjson::value cardAdjacencies = savedCardPlacement["adjacencies"];

										if (cardAdjacencies.isList()) {
											qjson::value_list cardAdjacenciesList = cardAdjacencies.list();

											for (qjson::value_ptr & cardAdjacencyPointer : cardAdjacenciesList) {
												qjson::value cardAdjacency = *cardAdjacencyPointer;
												unsigned int cardAdjacencyID = 0U;

												if (cardAdjacency.has("id")) {
													qjson::value savedCardAdjacencyID = cardAdjacency["id"];

													if (savedCardAdjacencyID.isNumber()) {
														cardAdjacencyID = static_cast<unsigned int>(savedCardAdjacencyID.asInt());
													}
												}

												game::PlayerCard::Adjacency & targetAdjacency = newPlayerCard.getAdjacency(cardAdjacencyID);
												if (cardAdjacency.has("pointofinterest_id")) {
													qjson::value cardAdjacencyPOIID = cardAdjacency["pointofinterest_id"];

													if (cardAdjacencyPOIID.isNumber()) {
														targetAdjacency.hasPointOfInterestAdjacency = true;
														targetAdjacency.poiType = static_cast<game::PointOfInterest::POIType>(static_cast<unsigned short>(cardAdjacencyPOIID.asInt()));
													}
												}
											}
										}
									}

									if (cardPlacement.cardID == game::PlayerCard::CardType::PLAYERCARD_SPECIAL_HEADQUARTERS) {
										headquartersCoordinates = cardPlacement.coordinates;
									}

									cardPlacementHistory[decksDealt].push_back(cardPlacement);
								}
							}
						}

						if (i < turnPlacementList.size() - 1U) { //If this is not the last turn - simulate every piece - adding score to correctly recalculate the game's score.
							++decksDealt;

							for (GameHexagonMap & hexagonMap : hexagonMaps) {
								std::size_t playerCardCount = hexagonMap.getPlayerCardCount();

								for (std::size_t i = 0U; i < playerCardCount; ++i) {
									gameScore += hexagonMap.getPlayerCard(i).getScore();
								}
							}
						}
					}
				}
			}

			//Update GUI labels.
			scoreLabel->setText(std::string("Score: ") + std::to_string(gameScore));
			decksDealtLabel->setPosition("scoreLabel.x + scoreLabel.w + 5", 5.F);
			decksDealtLabel->setText(std::string("Decks Dealt: ") + std::to_string(decksDealt) + std::string(" out of ") + std::to_string(maximumDeckCount));
		}

		if (saveRoot.has("remaining_deck")) {
			qjson::value savedRemainingDeck = saveRoot["remaining_deck"];

			if (savedRemainingDeck.isList()) {
				qjson::value_list savedRemainingDeckList = savedRemainingDeck.list();
				deck->clearDeck();

				for (qjson::value_ptr & savedRemainingDeckCardPointer : savedRemainingDeckList) {
					qjson::value savedRemainingDeckCard = *savedRemainingDeckCardPointer;

					if (savedRemainingDeckCard.isNumber()) {
						deck->addCard(static_cast<game::PlayerCard::CardType>(static_cast<unsigned short>(savedRemainingDeckCard.asInt())));
					}
				}
			}
		}
	}

	catch (std::runtime_error & runtimeError) {
		fileStream.close();
		win32::createErrorBox("ERROR", (std::string("Error when loading save!\n") + std::string(runtimeError.what())).c_str());

		return false;
	}

	fileStream.close();

	updateScroll();                                                   
	generateVisibleWorld();
	collectWorldHexagons();

	return true;
}

bool game::GameScene::hoveringOverValidPlacement() { //Checks whether the hexagon under the mouse's cursor is a valid place to place the selected piece.
	GameHexagonMap * parentHexagonMap;
	getParentMap(parentHexagonMap, coordinateHoverLocation);

	GameHexagonTile & gameHexagon = parentHexagonMap->queryMap(parentHexagonMap->asLocalCoordinatesU(coordinateHoverLocation));
	if (!gameHexagon.hasPlayerCard && !(gameHexagon.hasPointOfInterest && parentHexagonMap->getPointOfInterest(gameHexagon.pointOfInterestIndex).getType() == PointOfInterest::POINTOFINTEREST_MOUNTAIN)) {
		if (deck->hasSelectedCard() && (deck->getSelectedCardType() == PlayerCard::PLAYERCARD_SPECIAL_HEADQUARTERS || deck->getSelectedCardType() == PlayerCard::PLAYERCARD_SPECIAL_OUTPOST)) {
			return true;
		}

		sf::Vector2i * relevantOffsets = resources::GAME_ADJACENCYOFFSETS[coordinateHoverLocation.x & 1];
		GameHexagonMap * adjacencyHexagonMap;

		for (unsigned int i = 0U; i < 6U; ++i) {	
			sf::Vector2i adjacencyCoordinate = coordinateHoverLocation + relevantOffsets[i];
			getParentMap(adjacencyHexagonMap, adjacencyCoordinate);

			if (adjacencyHexagonMap->queryMap(adjacencyHexagonMap->asLocalCoordinatesU(adjacencyCoordinate)).hasPlayerCard) {
				return true;
			}
		}
	}

	return false;
}

void game::GameScene::generateZoomLevel() {
	//Generates a ZoomLevel - caching everything that is needed about the zoom level.
	//Information that is stored includes how many hexagons can be seen as well as the actual vertices needed for the background
	//These are sent to the GPU to be drawn.
	//ZoomLevels have to be cached as this is quite CPU and memory intensive - doing it every time the player scrolled would damage performance.

	sf::VertexBuffer * zoomLevelVertexBuffer;
	std::vector<sf::Vertex> worldVertices;
	std::vector<sf::Vector2f> unitHexagons;

	sf::FloatRect unitBounds;
	sf::FloatRect worldBounds;

	{
		sf::Vector2f offset;

		float repeatX4 = unitHexagon.getRepeatX() * 4.F;
		float repeatY4 = unitHexagon.getRepeatY() * 4.F;

		float x0 = -(unitHexagon.getOneAHalfSideLength() + repeatX4);
		float x1 = static_cast<int>(backBufferSize.x) / backgroundOutsideHexagon.getSideLength() + repeatX4;

		float y0 = -(unitHexagon.getHalfHeight() + repeatY4);
		float y1 = -repeatY4;
		float y2 = static_cast<int>(backBufferSize.y) / backgroundOutsideHexagon.getSideLength() + repeatY4;

		unitBounds = sf::FloatRect(x0, y0, x1 - x0, y2 - y0);

		bool n2 = false;
		for (float x = x0; x < x1; x += unitHexagon.getOneAHalfSideLength()) {
			offset.x = x;

			for (float y = (n2 ? y0 : y1); y < y2; y += unitHexagon.getRepeatY()) {
				offset.y = y;
				unitHexagons.push_back(offset);
			}

			n2 = !n2;
		}
	}

	{
		const sf::Vector2f * outsideVectorsPointer = backgroundOutsideHexagon.getVectors();
		sf::Vector2f offset;

		float repeatX4 = backgroundOutsideHexagon.getRepeatX() * 4.F;
		float repeatY4 = backgroundOutsideHexagon.getRepeatY() * 4.F;

		float x0 = -(backgroundOutsideHexagon.getOneAHalfSideLength() + repeatX4);
		float x1 = static_cast<int>(backBufferSize.x) + repeatX4;

		float y0 = -(backgroundOutsideHexagon.getHalfHeight() + repeatY4);
		float y1 = -repeatY4;
		float y2 = static_cast<int>(backBufferSize.y) + repeatY4;

		worldBounds = sf::FloatRect(x0, y0, x1 - x0, y2 - y0);

		bool n2 = false;
		for (float x = x0; x < x1; x += backgroundOutsideHexagon.getOneAHalfSideLength()) {
			offset.x = x;

			for (float y = (n2 ? y0 : y1); y < y2; y += backgroundOutsideHexagon.getRepeatY()) {
				offset.y = y;

				sf::Vertex vertex0(outsideVectorsPointer[0] + offset, backgroundOverlayColor);
				sf::Vertex vertex1(outsideVectorsPointer[3] + offset, backgroundOverlayColor);
				sf::Vertex vertex2(outsideVectorsPointer[4] + offset, backgroundOverlayColor);
				sf::Vertex vertex3(outsideVectorsPointer[5] + offset, backgroundOverlayColor);

				worldVertices.push_back(vertex1);
				worldVertices.push_back(vertex2);

				worldVertices.push_back(vertex2);
				worldVertices.push_back(vertex3);

				worldVertices.push_back(vertex3);
				worldVertices.push_back(vertex0);
			}

			n2 = !n2;
		}

		zoomLevelVertexBuffer = new sf::VertexBuffer(sf::PrimitiveType::Lines, sf::VertexBuffer::Static);
		zoomLevelVertexBuffer->create(worldVertices.size());
		zoomLevelVertexBuffer->update(worldVertices.data());
	}

	sf::IntRect coordinateBounds(-6, -4, static_cast<int>(std::ceilf(worldBounds.width / backgroundOutsideHexagon.getOneAHalfSideLength())) + 2, static_cast<int>(std::ceilf(worldBounds.height / backgroundOutsideHexagon.getHeight())));
	calculatedZoomLevels.emplace_back(cumulativeZoom, zoomLevelVertexBuffer, unitHexagons.data(), unitHexagons.size(), coordinateBounds, unitBounds, worldBounds, sf::Vector2f(0.F, (backgroundOutsideHexagon.getHeight() - backgroundOutsideHexagon.getSideLength()) / 2.F));
}

bool game::GameScene::generateVisibleWorld() { //Generates points of interest for the visible screen area.
	sf::IntRect zoomLevelCoordinateBounds = currentZoomLevel->getCoordinateBounds();
	sf::Vector2i offset = engine::rectangleLocation<int, int>(zoomLevelCoordinateBounds) - backgroundOutsideHexagon.mapCoordinates(fullScroll);
	bool newCreated = false;

	int coordinateWidth = currentZoomLevel->getCoordinateBounds().width;
	int coordinateHeight = currentZoomLevel->getCoordinateBounds().height;

	for (int x = 0; x < coordinateWidth; ++x) {
		for (int y = 0; y < coordinateHeight; ++y) {
			sf::Vector2i tileCoordinates = sf::Vector2i(x, y) + offset;

			GameHexagonMap * parentHexagonMap;
			getParentMap(parentHexagonMap, tileCoordinates);

			sf::Vector2u localTileCoordinates = parentHexagonMap->asLocalCoordinatesU(tileCoordinates);
			GameHexagonTile & gameHexagon = parentHexagonMap->queryMap(localTileCoordinates);

			if (!gameHexagon.discovered) { //If the hexagon hasn't already attempted to generate a point of interest on it.
				//Seed the scene's random engine based upon the world seed and the coordinates of the tile - making it predictable.
				std::seed_seq seedSequence{ worldSeed, static_cast<std::size_t>(tileCoordinates.x), static_cast<std::size_t>(tileCoordinates.y) };
				sceneEngine.seed(seedSequence);

				if (resources::ZERO_ONE_DISTRIBUTION(sceneEngine) < resources::GAME_POI_FREQUENCY) {
					parentHexagonMap->addPointOfInterest(localTileCoordinates, static_cast<unsigned short>(std::powf(1.4F, poiGeneratorDistribution(sceneEngine)) - 1.F));
					newCreated |= true;
				}

				gameHexagon.discovered = true;
			}
		}
	}

	return newCreated;
}


void game::GameScene::updateBackgroundVertices() {
	static const sf::Color hoverBackgroundColor(255U, 255U, 255U, 50U);

	float cumulativeZoomMultiplier = std::powf(resources::GAME_ZOOMMULTIPLIER, cumulativeZoom);
	backgroundOutsideHexagon.generateHexagon(defaultSideLength * cumulativeZoomMultiplier);
	backgroundInsideHexagon.generateHexagon(defaultSideLength * cumulativeZoomMultiplier * resources::GAME_INSIDEHEXAGON_PERCENTAGE);
	outInTranslation = (backgroundOutsideHexagon.getSize() - backgroundInsideHexagon.getSize()) / 2.F;

	const sf::Vector2f * outsideHexagonVectors = backgroundOutsideHexagon.getVectors();
	std::unique_ptr<sf::Vertex[]> hoverVertices = std::make_unique<sf::Vertex[]>(13U);

	sf::Vertex vertex0(outsideHexagonVectors[0], hoverBackgroundColor);
	sf::Vertex vertex1(outsideHexagonVectors[1], hoverBackgroundColor);
	sf::Vertex vertex2(outsideHexagonVectors[2], hoverBackgroundColor);
	sf::Vertex vertex3(outsideHexagonVectors[3], hoverBackgroundColor);
	sf::Vertex vertex4(outsideHexagonVectors[4], hoverBackgroundColor);
	sf::Vertex vertex5(outsideHexagonVectors[5], hoverBackgroundColor);
	sf::Vertex vertex6(outsideHexagonVectors[6], hoverBackgroundColor);

	hoverVertices[0] = vertex6;
	hoverVertices[1] = vertex0;
	hoverVertices[2] = vertex1;

	hoverVertices[3] = vertex1;
	hoverVertices[4] = vertex2;

	hoverVertices[5] = vertex2;
	hoverVertices[6] = vertex3;

	hoverVertices[7] = vertex3;
	hoverVertices[8] = vertex4;

	hoverVertices[9] = vertex4;
	hoverVertices[10] = vertex5;

	hoverVertices[11] = vertex5;
	hoverVertices[12] = vertex0;

	hoverVertexBuffer.update(hoverVertices.get(), 13U, 0U);
	hoverRenderStates.getShader()->setUniform("radius", ((backgroundOutsideHexagon.getSideLength() * 1.25F) / mouseMultiplier));

	for (unsigned int i = 0U; i < calculatedZoomLevels.size(); ++i) {
		engine::ZoomLevel * zoomLevel = &calculatedZoomLevels[i];

		if (zoomLevel->getZoomLevel() == cumulativeZoom) {
			currentZoomLevel = zoomLevel;
			return;
		}
	}

	//There is no cached zoom level - generate a new one.
	generateZoomLevel();
	currentZoomLevel = &*calculatedZoomLevels.rbegin();
}

void game::GameScene::updateHoverVertices() {
	screenHoverLocation = backgroundOutsideHexagon.worldCoordinates(mouseWorldLocation - partialScroll) + partialScroll;

	hoverRenderStates.getShader()->setUniform("offset", screenHoverLocation);
	hoverRenderStates.getShader()->setUniform("center", (screenHoverLocation + backgroundOutsideHexagon.getVectors()[6]) / mouseMultiplier + static_cast<sf::Vector2f>(letterBoxOffset));

	coordinateHoverLocation = backgroundOutsideHexagon.mapCoordinates(mouseWorldLocation - fullScroll);
	validPlacementHover = hoveringOverValidPlacement();
}

void game::GameScene::updateBackgroundColors(const sf::Color & backgroundColor, const sf::Color & backgroundOverlayColor, const sf::Color & highlightColor, const sf::Color & lowlightColor) {
	this->backgroundColor = backgroundColor;
	this->backgroundOverlayColor = backgroundOverlayColor;
	this->highlightColor = highlightColor;
	this->lowlightColor = lowlightColor;

	midlightColor = sf::Color((highlightColor.r + lowlightColor.r) / 2U, (highlightColor.g + lowlightColor.g) / 2U, (highlightColor.b + lowlightColor.b) / 2U, (highlightColor.a + lowlightColor.a) / 2U);

	{
		unsigned int x4R = backgroundColor.r * 4U;
		unsigned int x4G = backgroundColor.g * 4U;
		unsigned int x4B = backgroundColor.b * 4U;

		transparentHighlightColor = sf::Color((highlightColor.r + x4R) / 5U, (highlightColor.g + x4G) / 5U, (highlightColor.b + x4B) / 5U, 255U);
		transparentMidlightColor = sf::Color((midlightColor.r + x4R) / 5U, (midlightColor.g + x4G) / 5U, (midlightColor.b + x4B) / 5U, 255U);
		transparentLowlightColor = sf::Color((lowlightColor.r + x4R) / 5U, (lowlightColor.g + x4G) / 5U, (lowlightColor.b + x4B) / 5U, 255U);
	}

	calculatedZoomLevels.clear();
	updateBackgroundVertices();
}


void game::GameScene::collectWorldHexagons() {
	//Pushes everything visible on screen into a vertexbuffer so they can be drawn super quickly.
	//Also manages some graphical effects such as translucent pieces.
	std::vector<sf::Vertex> playerCardBackgroundVertices;
	std::vector<sf::Vertex> playerCardImageVertices;
	std::vector<sf::Vertex> pointOfInterestVertices;

	const sf::Vector2f * hexagonOutsideVectors = backgroundOutsideHexagon.getVectors();
	const sf::Vector2f * hexagonInsideVectors = backgroundInsideHexagon.getVectors();

	playerCardBackgroundVertices.reserve(playerCardBackgroundVertexBuffer.getVertexCount());
	playerCardImageVertices.reserve(playerCardImageVertexBuffer.getVertexCount());
	pointOfInterestVertices.reserve(pointOfInterestVertexBuffer.getVertexCount());

	free(visibleHexagonMaps);
	getVisibleMaps(visibleHexagonMaps, hexagonMapCount);
	sf::IntRect visibleCoordinateRectangle = engine::translateRectangle<int>(currentZoomLevel->getCoordinateBounds(), -backgroundOutsideHexagon.mapCoordinates(fullScroll));

	for (std::size_t i = 0U; i < hexagonMapCount; ++i) {
		GameHexagonMap * hexagonMap = visibleHexagonMaps[i];

		sf::IntRect intersection;
		if (visibleCoordinateRectangle.intersects(hexagonMap->getCoordinateBounds(), intersection)) { //If any of the hexagon map is actually visible.
			sf::Vector2i trueLocation = engine::rectangleLocation<int, int>(intersection);
			sf::Vector2u translatedOrigin = hexagonMap->asLocalCoordinatesU(trueLocation);
			sf::Vector2u translatedEnd = translatedOrigin + engine::rectangleSize<unsigned int, int>(intersection);

			for (unsigned int x = translatedOrigin.x; x < translatedEnd.x; ++x, ++trueLocation.x) {
				for (unsigned int y = translatedOrigin.y; y < translatedEnd.y; ++y, ++trueLocation.y) {
					GameHexagonTile & hexagonTile = hexagonMap->queryMap(x, y);

					//Is there anythign to draw here? If not, skip it.
					if (!hexagonTile.hasPlayerCard && !hexagonTile.hasPointOfInterest) {
						continue;
					}

					bool drawPOI = false;
					bool translucentCard = false;

					if (coordinateHoverLocation == trueLocation) {
						if (hexagonTile.hasPointOfInterest) {
							drawPOI = true;
						}

						if (hexagonTile.hasPlayerCard) {
							translucentCard = true;
						}
					}

					else if (!hexagonTile.hasPlayerCard && hexagonTile.hasPointOfInterest) {
						drawPOI = true;
					}

					if (hexagonTile.hasPlayerCard) { //Push the vertices into the vector if there is a player card here that needs to be drawn.
						game::PlayerCard & playerCard = hexagonMap->getPlayerCard(hexagonTile.playerCardIndex);
						sf::Vector2f truePosition = hexagonTile.unitMapLocation * backgroundOutsideHexagon.getSideLength();
						sf::Color backColor = playerCard.getBackColor();

						const sf::Vector2f * textureCoordinates = resources::GAME_TEXTURECOORDINATES_PLAYERCARD.get() + playerCard.getTextureCoordinateIndex();

						sf::Vertex vertex0(hexagonOutsideVectors[0U] + truePosition, transparentMidlightColor);
						sf::Vertex vertex1(hexagonOutsideVectors[1U] + truePosition, transparentLowlightColor);
						sf::Vertex vertex2(hexagonOutsideVectors[2U] + truePosition, transparentLowlightColor);
						sf::Vertex vertex3(hexagonOutsideVectors[3U] + truePosition, transparentMidlightColor);
						sf::Vertex vertex4(hexagonOutsideVectors[4U] + truePosition, transparentHighlightColor);
						sf::Vertex vertex5(hexagonOutsideVectors[5U] + truePosition, transparentHighlightColor);
						sf::Vertex vertex6(hexagonInsideVectors[0U] + truePosition + outInTranslation, midlightColor);
						sf::Vertex vertex7(hexagonInsideVectors[1U] + truePosition + outInTranslation, lowlightColor);
						sf::Vertex vertex8(hexagonInsideVectors[2U] + truePosition + outInTranslation, lowlightColor);
						sf::Vertex vertex9(hexagonInsideVectors[3U] + truePosition + outInTranslation, midlightColor);
						sf::Vertex vertex10(hexagonInsideVectors[4U] + truePosition + outInTranslation, highlightColor);
						sf::Vertex vertex11(hexagonInsideVectors[5U] + truePosition + outInTranslation, highlightColor);
						sf::Vertex vertex12(hexagonInsideVectors[0U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex13(hexagonInsideVectors[1U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex14(hexagonInsideVectors[2U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex15(hexagonInsideVectors[3U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex16(hexagonInsideVectors[4U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex17(hexagonInsideVectors[5U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex18(hexagonInsideVectors[6U] + truePosition + outInTranslation, backColor);
						sf::Vertex vertex19(hexagonOutsideVectors[0U] + truePosition + currentZoomLevel->getImageOffset(), textureCoordinates[0U]);
						sf::Vertex vertex20(hexagonOutsideVectors[1U] + truePosition + currentZoomLevel->getImageOffset(), textureCoordinates[1U]);
						sf::Vertex vertex21(hexagonOutsideVectors[3U] + truePosition - currentZoomLevel->getImageOffset(), textureCoordinates[2U]);
						sf::Vertex vertex22(hexagonOutsideVectors[4U] + truePosition - currentZoomLevel->getImageOffset(), textureCoordinates[3U]);

						//Render the card at quarter opacity by bitshifting all of the alpha bits left ways twice.
						if (translucentCard) {
							vertex0.color.a >>= 2;
							vertex1.color.a >>= 2;
							vertex2.color.a >>= 2;
							vertex3.color.a >>= 2;
							vertex4.color.a >>= 2;
							vertex5.color.a >>= 2;
							vertex6.color.a >>= 2;
							vertex7.color.a >>= 2;
							vertex8.color.a >>= 2;
							vertex9.color.a >>= 2;
							vertex10.color.a >>= 2;
							vertex11.color.a >>= 2;
							vertex12.color.a >>= 2;
							vertex13.color.a >>= 2;
							vertex14.color.a >>= 2;
							vertex15.color.a >>= 2;
							vertex16.color.a >>= 2;
							vertex17.color.a >>= 2;
							vertex18.color.a >>= 2;
							vertex19.color.a >>= 2;
							vertex20.color.a >>= 2;
							vertex21.color.a >>= 2;
							vertex22.color.a >>= 2;
						}

						playerCardBackgroundVertices.push_back(vertex0);
						playerCardBackgroundVertices.push_back(vertex1);
						playerCardBackgroundVertices.push_back(vertex6);

						playerCardBackgroundVertices.push_back(vertex1);
						playerCardBackgroundVertices.push_back(vertex6);
						playerCardBackgroundVertices.push_back(vertex7);

						playerCardBackgroundVertices.push_back(vertex1);
						playerCardBackgroundVertices.push_back(vertex2);
						playerCardBackgroundVertices.push_back(vertex7);

						playerCardBackgroundVertices.push_back(vertex2);
						playerCardBackgroundVertices.push_back(vertex7);
						playerCardBackgroundVertices.push_back(vertex8);

						playerCardBackgroundVertices.push_back(vertex2);
						playerCardBackgroundVertices.push_back(vertex3);
						playerCardBackgroundVertices.push_back(vertex8);

						playerCardBackgroundVertices.push_back(vertex3);
						playerCardBackgroundVertices.push_back(vertex8);
						playerCardBackgroundVertices.push_back(vertex9);

						playerCardBackgroundVertices.push_back(vertex3);
						playerCardBackgroundVertices.push_back(vertex4);
						playerCardBackgroundVertices.push_back(vertex9);

						playerCardBackgroundVertices.push_back(vertex4);
						playerCardBackgroundVertices.push_back(vertex9);
						playerCardBackgroundVertices.push_back(vertex10);

						playerCardBackgroundVertices.push_back(vertex4);
						playerCardBackgroundVertices.push_back(vertex5);
						playerCardBackgroundVertices.push_back(vertex10);

						playerCardBackgroundVertices.push_back(vertex5);
						playerCardBackgroundVertices.push_back(vertex10);
						playerCardBackgroundVertices.push_back(vertex11);

						playerCardBackgroundVertices.push_back(vertex5);
						playerCardBackgroundVertices.push_back(vertex0);
						playerCardBackgroundVertices.push_back(vertex11);

						playerCardBackgroundVertices.push_back(vertex0);
						playerCardBackgroundVertices.push_back(vertex11);
						playerCardBackgroundVertices.push_back(vertex6);

						playerCardBackgroundVertices.push_back(vertex12);
						playerCardBackgroundVertices.push_back(vertex13);
						playerCardBackgroundVertices.push_back(vertex18);

						playerCardBackgroundVertices.push_back(vertex13);
						playerCardBackgroundVertices.push_back(vertex14);
						playerCardBackgroundVertices.push_back(vertex18);

						playerCardBackgroundVertices.push_back(vertex14);
						playerCardBackgroundVertices.push_back(vertex15);
						playerCardBackgroundVertices.push_back(vertex18);

						playerCardBackgroundVertices.push_back(vertex15);
						playerCardBackgroundVertices.push_back(vertex16);
						playerCardBackgroundVertices.push_back(vertex18);

						playerCardBackgroundVertices.push_back(vertex16);
						playerCardBackgroundVertices.push_back(vertex17);
						playerCardBackgroundVertices.push_back(vertex18);

						playerCardBackgroundVertices.push_back(vertex17);
						playerCardBackgroundVertices.push_back(vertex12);
						playerCardBackgroundVertices.push_back(vertex18);

						playerCardImageVertices.push_back(vertex19);
						playerCardImageVertices.push_back(vertex20);
						playerCardImageVertices.push_back(vertex22);

						playerCardImageVertices.push_back(vertex20);
						playerCardImageVertices.push_back(vertex22);
						playerCardImageVertices.push_back(vertex21);
					}

					if (drawPOI) { //Push the vertices into the vector if there is a point of interest here that needs to be drawn.
						game::PointOfInterest & pointOfInterest = hexagonMap->getPointOfInterest(hexagonTile.pointOfInterestIndex);
						sf::Vector2f truePosition = hexagonTile.unitMapLocation * backgroundOutsideHexagon.getSideLength();

						const sf::Vector2f * textureCoordinates = resources::GAME_TEXTURECOORDINATES_POINTOFINTEREST.get() + pointOfInterest.getTextureCoordinateIndex();

						sf::Vertex vertex0(hexagonOutsideVectors[0U] + truePosition + currentZoomLevel->getImageOffset(), textureCoordinates[0U]);
						sf::Vertex vertex1(hexagonOutsideVectors[1U] + truePosition + currentZoomLevel->getImageOffset(), textureCoordinates[1U]);
						sf::Vertex vertex2(hexagonOutsideVectors[3U] + truePosition - currentZoomLevel->getImageOffset(), textureCoordinates[2U]);
						sf::Vertex vertex3(hexagonOutsideVectors[4U] + truePosition - currentZoomLevel->getImageOffset(), textureCoordinates[3U]);

						pointOfInterestVertices.push_back(vertex0);
						pointOfInterestVertices.push_back(vertex1);
						pointOfInterestVertices.push_back(vertex3);

						pointOfInterestVertices.push_back(vertex1);
						pointOfInterestVertices.push_back(vertex3);
						pointOfInterestVertices.push_back(vertex2);
					}
				}

				trueLocation.y = intersection.top;
			}
		}
	}

	//Update the vertex buffers as to now store everything visible on screen.
	playerCardBackgroundVertexBuffer.create(playerCardBackgroundVertices.size());
	playerCardBackgroundVertexBuffer.update(playerCardBackgroundVertices.data());

	playerCardImageVertexBuffer.create(playerCardImageVertices.size());
	playerCardImageVertexBuffer.update(playerCardImageVertices.data());

	pointOfInterestVertexBuffer.create(pointOfInterestVertices.size());
	pointOfInterestVertexBuffer.update(pointOfInterestVertices.data());
}

void game::GameScene::updateScroll() {
	playerCardBackgroundRenderStates.getShader()->setUniform("offset", fullScroll);
	playerCardImageRenderStates.getShader()->setUniform("offset", fullScroll);
	pointOfInterestRenderStates.getShader()->setUniform("offset", fullScroll);
	foregroundParticleManager.managerRenderStates.getShader()->setUniform("offset", fullScroll);
	backgroundParticleManager.managerRenderStates.getShader()->setUniform("offset", fullScroll);

	updateBackgroundVertices();
	free(visibleHexagonMaps);
	getVisibleMaps(visibleHexagonMaps, hexagonMapCount);

	partialScroll.x = std::fmod(fullScroll.x, backgroundOutsideHexagon.getRepeatX());
	partialScroll.y = std::fmod(fullScroll.y, backgroundOutsideHexagon.getRepeatY());
	backgroundRenderStates.getShader()->setUniform("offset", partialScroll);
	updateHoverVertices();
}

void game::GameScene::drawNewDeck() { //Creates a new deck - usually called when the deck is empty. Also checks if the same is over and updates score automatically.
	//Seed the scene's random engine depending on the number of deck's dealt and the world's seed.
	std::seed_seq seedSequence{ worldSeed, decksDealt };
	sceneEngine.seed(seedSequence);

	deck->addCard(PlayerCard::PLAYERCARD_SPECIAL_OUTPOST); //Give the guaranteed outpost card.
	for (unsigned int i = 1U; i < resources::GAME_DECK_SIZE; ++i) {
		deck->addCard(static_cast<game::PlayerCard::CardType>(static_cast<unsigned short>(std::powf(resources::e, cardDealerDistribution(sceneEngine)) + 1U)));
	}

	deck->shuffleDeck(sceneEngine);
	++decksDealt;

	for (GameHexagonMap & hexagonMap : hexagonMaps) {
		std::size_t playerCardCount = hexagonMap.getPlayerCardCount();

		for (std::size_t i = 0U; i < playerCardCount; ++i) {
			gameScore += hexagonMap.getPlayerCard(i).getScore();
		}
	}

	if (decksDealt > maximumDeckCount) { //Check if the game has ended.
		engine::setCurrentScene(new scene::EndGameScene(this), false);
	}

	else { //Update labels.
		scoreLabel->setText(std::string("Score: ") + std::to_string(gameScore));
		decksDealtLabel->setPosition("scoreLabel.x + scoreLabel.w + 5", 5);
		decksDealtLabel->setText(std::string("Decks Dealt: ") + std::to_string(decksDealt) + std::string(" out of ") + std::to_string(maximumDeckCount));
	}
}

game::PlayerCard & game::GameScene::placeSelectedCard(const sf::Vector2i & coordinates) { //Place the card currently selected in the deck. Uses placePlayerCard() internally.
	game::PlayerCard::CardType drawnCardType = deck->useCard();
	game::PlayerCard & newPlayerCard = placePlayerCard(coordinates, drawnCardType);

	engine::CardPlacement newPlacement;
	newPlacement.timeSeconds = totalTimeSeconds;
	newPlacement.coordinates = coordinates;
	newPlacement.cardID = drawnCardType;
	cardPlacementHistory[decksDealt].push_back(newPlacement);

	if (deck->isEmpty()) {
		drawNewDeck();
	}

	return newPlayerCard;
}

game::PlayerCard & game::GameScene::placePlayerCard(const sf::Vector2i & coordinates, PlayerCard::CardType cardType) { //Places a card of a given type at a given location.
	GameHexagonMap * parentHexagonMap;
	getParentMap(parentHexagonMap, coordinates);
	sf::Vector2u mapCoordinates = parentHexagonMap->asLocalCoordinatesU(coordinates);

	game::PlayerCard & playerCard = parentHexagonMap->addPlayerCard(mapCoordinates, cardType);

	particles::ParticleEmitter & particleEmitter = backgroundParticleManager.addEmitter();
	particleEmitter.setUsingSpeedVariation(true);
	particleEmitter.setSpeedVariation(0.125F);

	particleEmitter.setUsingRotationSpeedVariation(true);
	particleEmitter.setRotationSpeedVariation(0.5F);

	particleEmitter.setUsingLifeTimeVariation(true);
	particleEmitter.setLifeTimeVariation(0.125F);

	particleEmitter.setUsingSizeVariation(true);
	particleEmitter.setSizeVariation(0.25F);

	particleEmitter.setUsingMultipleTextures(true);

	particleEmitter.setMaxTotalParticles(6U);
	particleEmitter.setParticleTextureCoordinates(resources::GAME_TEXTURECOORDINATES_PARTICLE.get(), 4U);
	particleEmitter.setParticleSpeed(backgroundOutsideHexagon.getOneAHalfSideLength());
	particleEmitter.setParticleResistance(0.9990234375F);
	particleEmitter.setParticleLifeSeconds(0.625F);
	particleEmitter.setParticleRotationsPerSecond(resources::PI / 4.F);
	particleEmitter.setEmitterLocation((parentHexagonMap->queryMap(mapCoordinates).unitMapLocation * backgroundOutsideHexagon.getSideLength()) + backgroundOutsideHexagon.getVectors()[6U]);
	particleEmitter.setMaxWorldParticles(6U);
	particleEmitter.setParticleEmitTimeSeconds(0.F);
	particleEmitter.setParticleSize(sf::Vector2f(backgroundOutsideHexagon.getOneAHalfSideLength(), backgroundOutsideHexagon.getOneAHalfSideLength()));
	particleEmitter.setFadeOutStart(0.5F);

	if (playerCard.getID() == game::PlayerCard::PLAYERCARD_SPECIAL_HEADQUARTERS) {
		headquartersCoordinates = coordinates;
	}

	else if (playerCard.emitsSmoke()) {
		particles::ParticleEmitter & particleEmitter = foregroundParticleManager.addEmitter();

		particleEmitter.setMaxTotalParticles(0U);
		particleEmitter.setMaxWorldParticles(32U);

		particleEmitter.setParticleUseRandomDirection(false);
		particleEmitter.setParticleSpeed(backgroundOutsideHexagon.getHalfHeight() / 32.F);
		particleEmitter.setParticleResistance(-0.1875F);
		particleEmitter.setParticleEmitTimeSeconds(3.F);
		particleEmitter.setParticleRotationsPerSecond(resources::PI / 24.F);
		particleEmitter.setParticleLifeSeconds(14.F);
		particleEmitter.setParticleDirection((11.F * resources::PI) / 6.F);
		particleEmitter.setParticleSize(sf::Vector2f(backgroundOutsideHexagon.getTwoSideLength(), backgroundOutsideHexagon.getTwoSideLength()));

		particleEmitter.setUsingMultipleTextures(true);
		particleEmitter.setParticleTextureCoordinates(resources::GAME_TEXTURECOORDINATES_PARTICLE.get(), 4U);

		particleEmitter.setFadesIn(true);
		particleEmitter.setFadeInEnd(0.125F);

		particleEmitter.setFadesOut(true);
		particleEmitter.setFadeOutStart(0.875F);

		particleEmitter.setUsingEmitTimeVariation(true);
		particleEmitter.setEmitTimeVariation(0.5F);

		particleEmitter.setUsingRotationSpeedVariation(true);
		particleEmitter.setRotationSpeedVariation(0.125F);

		particleEmitter.setUsingLifeTimeVariation(true);
		particleEmitter.setLifeTimeVariation(0.125F);

		particleEmitter.setEmitterLocation((parentHexagonMap->queryMap(mapCoordinates).unitMapLocation * backgroundOutsideHexagon.getSideLength()) + backgroundOutsideHexagon.getVectors()[6U]);
	}

	applyAdjacencies(coordinates, playerCard, true);
	return playerCard;
}

bool game::GameScene::getVisibleMaps(GameHexagonMap **& visibleHexagonMaps, std::size_t & hexagonMapCount) { //Gets all the visible hexagon maps on screen.
	sf::IntRect zoomLevelCoordinateBounds = currentZoomLevel->getCoordinateBounds();

	//Calculates the maximum number of maps that are needed to represent all hexagons on the screen.
	//For example, if the screen was showing 500 by 500 hexagons you may need 9 maps to show them all.
	unsigned int visibleMapsX = static_cast<unsigned int>(std::ceilf(static_cast<float>(zoomLevelCoordinateBounds.width) / resources::GAME_MAPDIMENSIONS)) + 1U;
	unsigned int visibleMapsY = static_cast<unsigned int>(std::ceilf(static_cast<float>(zoomLevelCoordinateBounds.height) / resources::GAME_MAPDIMENSIONS)) + 1U;
	std::size_t maxMapCount = visibleMapsX * visibleMapsY;

	sf::Vector2i * checkPositions = static_cast<sf::Vector2i *>(malloc(maxMapCount * sizeof(sf::Vector2i)));
	bool * foundPositions = static_cast<bool *>(calloc(maxMapCount, sizeof(bool)));

	//Calculates points to check which map owns it. If all are checked each hexagon on the screen will be accounted for.
	sf::Vector2i coordinateOffset = engine::rectangleLocation<int, int>(zoomLevelCoordinateBounds) - backgroundOutsideHexagon.mapCoordinates(fullScroll);
	float moveX = zoomLevelCoordinateBounds.width / (visibleMapsX - 1.F);
	float moveY = zoomLevelCoordinateBounds.height / (visibleMapsY - 1.F);
	for (unsigned int x = 0U, i = 0U; x < visibleMapsX; ++x) {
		for (unsigned int y = 0U; y < visibleMapsY; ++y) {
			checkPositions[i++] = sf::Vector2i(static_cast<int>(std::roundf(moveX * x)), static_cast<int>(std::roundf(moveY * y))) + coordinateOffset;
		}
	}

	//Stores the indexes of all the maps found to be visible.
	std::vector<std::size_t> visibleMapIndices;
	visibleMapIndices.reserve(maxMapCount);

	GameHexagonMap * mapData = hexagonMaps.data();
	for (std::size_t i = 0U; i < hexagonMaps.size(); ++i) { //Checks through all of the maps calculated and checks if they contain the coordinate.
		//Also makes sure not to add the same map twice if it accounts for more than one of the check points.
		GameHexagonMap * hexagonMap = mapData + i;

		bool mapAdded = false;
		for (std::size_t j = 0U; j < maxMapCount; ++j) {
			if (!foundPositions[j] && hexagonMap->containsCoordinate(checkPositions[j])) {
				foundPositions[j] = true;

				if (!mapAdded) { //Do not add twice.
					visibleMapIndices.push_back(i);
					mapAdded = true;
				}
			}
		}
	}

	//Adds any new hexagon maps as required if not all points were accounted for.
	//The method guarantees that maps will be created to store everything on the screen.
	bool addedNew = false;
	for (std::size_t i = 0U; i < maxMapCount; ++i) {
		if (!foundPositions[i]) {
			sf::Vector2i & checkPosition = checkPositions[i];
			//Gets the origin coordinates of the map that would store this check point.
			int xCoordinate = static_cast<int>(std::floorf(static_cast<float>(checkPosition.x) / resources::GAME_MAPDIMENSIONS) * resources::GAME_MAPDIMENSIONS);
			int yCoordinate = static_cast<int>(std::floorf(static_cast<float>(checkPosition.y) / resources::GAME_MAPDIMENSIONS) * resources::GAME_MAPDIMENSIONS);

			visibleMapIndices.push_back(hexagonMaps.size()); 
			hexagonMaps.emplace_back(xCoordinate, yCoordinate, resources::GAME_MAPDIMENSIONS, resources::GAME_MAPDIMENSIONS, &unitHexagon); //Create new hexagon map.
			addedNew = true;
		}
	}

	//Converts the stored indices into pointers to be passed to the callee.
	hexagonMapCount = visibleMapIndices.size();
	visibleHexagonMaps = static_cast<GameHexagonMap **>(malloc(sizeof(game::GameHexagonMap *) * hexagonMapCount));
	for (std::size_t i = 0U; i < hexagonMapCount; ++i) {
		visibleHexagonMaps[i] = hexagonMaps.data() + visibleMapIndices[i];
	}

	//Free up dynamically allocated arrays to prevent memory leaks.
	free(checkPositions);
	free(foundPositions);
	return addedNew;
}

bool game::GameScene::getParentMap(GameHexagonMap *& parentHexagonMap, const sf::Vector2i & coordinate) { //Gets the parent map of a given point.
	std::size_t mapCount = hexagonMaps.size();
	GameHexagonMap * mapData = hexagonMaps.data();

	for (std::size_t i = 0U; i < mapCount; ++i) { //Checks through each of the maps and see if they contain the point.
		GameHexagonMap * hexagonMap = mapData + i;

		if (hexagonMap->containsCoordinate(coordinate)) {
			parentHexagonMap = hexagonMap;
			return false;
		}
	}

	//Generate a new map if one does not exists that contains the point.
	//The method guarantees that a new map will be created to store the point if it doesn't have one yet.
	int xCoordinate = static_cast<int>(std::floorf(static_cast<float>(coordinate.x) / resources::GAME_MAPDIMENSIONS) * resources::GAME_MAPDIMENSIONS);
	int yCoordinate = static_cast<int>(std::floorf(static_cast<float>(coordinate.y) / resources::GAME_MAPDIMENSIONS) * resources::GAME_MAPDIMENSIONS);
	hexagonMaps.emplace_back(xCoordinate, yCoordinate, resources::GAME_MAPDIMENSIONS, resources::GAME_MAPDIMENSIONS, &unitHexagon);
	parentHexagonMap = hexagonMaps.data() + mapCount;

	return true;
}

void game::GameScene::applyAdjacencies(const sf::Vector2i & coordinates, game::PlayerCard & playerCard, bool reverseAdjacencies) {
	//Notify pieces of adjacenct stuff.
	//A newly placed piece must be notified of all adjacent cards and points of interest.
	//In addition, already placed cards may need to be notified of the new piece.
	GameHexagonMap * adjacencyParentMap;
	getParentMap(adjacencyParentMap, coordinates);

	GameHexagonTile & parentTile = adjacencyParentMap->queryMap(adjacencyParentMap->asLocalCoordinatesU(coordinates));

	if (parentTile.hasPointOfInterest) {
		playerCard.setHasPointOfInterest(true);
		playerCard.setPointOfInterest(adjacencyParentMap->getPointOfInterest(parentTile.pointOfInterestIndex).getType());
	}

	sf::Vector2i * relevantOffsets = resources::GAME_ADJACENCYOFFSETS[coordinates.x & 1];
	for (unsigned int i = 0U, j = 3U; i < 6U; ++i, ++j) { //i is the index of the adjacency and j is the index of the reverse adjacency
		if (j > 5U) { //Wrap the reverse adjacency around if it gets to 6.
			j = 0U;
		}

		sf::Vector2i adjacencyCoordinates = coordinates + relevantOffsets[i];
		game::GameHexagonMap * adjacencyParentMap;
		getParentMap(adjacencyParentMap, adjacencyCoordinates);

		game::PlayerCard::Adjacency & adjacency = playerCard.getAdjacency(i);
		game::GameHexagonTile & adjacentTile = adjacencyParentMap->queryMap(adjacencyParentMap->asLocalCoordinatesU(adjacencyCoordinates));

		if (adjacentTile.hasPlayerCard) {
			game::PlayerCard & adjacentCard = adjacencyParentMap->getPlayerCard(adjacentTile.playerCardIndex);
			adjacency.hasPlayerCardAdjacency = true;
			adjacency.cardType = adjacentCard.getType();

			if (reverseAdjacencies) { //If we need to add reverse adjacencies to the map.
				game::PlayerCard::Adjacency & reverseAdjacency = adjacentCard.getAdjacency(j);

				reverseAdjacency.hasPlayerCardAdjacency = true;
				reverseAdjacency.cardType = playerCard.getType();
			}
		}

		if (adjacentTile.hasPointOfInterest) {
			adjacency.hasPointOfInterestAdjacency = true;
			adjacency.poiType = adjacencyParentMap->getPointOfInterest(adjacentTile.pointOfInterestIndex).getType();
		}
	}
}

unsigned int game::GameScene::calculatePotentialScore(const sf::Vector2i & coordinates, PlayerCard::CardType cardType) { //Pretends to place a piece to calculate it's potential score.
	game::PlayerCard playerCard(cardType);
	applyAdjacencies(coordinates, playerCard, false);

	return playerCard.getScore();
}

game::GameHexagonMap::GameHexagonMap(int xCoordinate, int yCoordinate, unsigned int coordinateWidth, unsigned int coordinateHeight, const engine::Hexagon * hexagon) : HexagonMap(xCoordinate, yCoordinate, coordinateWidth, coordinateHeight, hexagon) {
	sf::Vector2f unitLocation = hexagon->worldCoordinates(xCoordinate, yCoordinate);
	sf::Vector2f offset;

	float y0 = unitLocation.y;
	float y1 = unitLocation.y + hexagon->getHalfHeight();

	bool n2 = xCoordinate & 1;
	offset.x = unitLocation.x; //Calculate the unit location (where sideLength = 1) of each heaxgon in the map.
	for (unsigned int x = 0U; x < coordinateWidth; ++x, offset.x += hexagon->getOneAHalfSideLength()) {
		offset.y = n2 ? y0 : y1;

		for (unsigned int y = 0U; y < coordinateHeight; ++y, offset.y += hexagon->getHeight()) {
			map[x][y].unitMapLocation = offset;
		}

		n2 = !n2;
	}
}

game::GameHexagonMap::GameHexagonMap(const GameHexagonMap & gameHexagonMap) : HexagonMap(gameHexagonMap) { //Copies an already existing hexagon map.
	pointsOfInterest = std::vector<game::PointOfInterest>(gameHexagonMap.pointsOfInterest);
	playerCards = std::vector<game::PlayerCard>(gameHexagonMap.playerCards);
}

game::PointOfInterest & game::GameHexagonMap::addPointOfInterest(const sf::Vector2u & coordinate, unsigned short poiID) {
	GameHexagonTile & gameHexagon = queryMap(coordinate);
	gameHexagon.hasPointOfInterest = true;
	gameHexagon.pointOfInterestIndex = pointsOfInterest.size();

	return pointsOfInterest.emplace_back(poiID);
}

game::PlayerCard & game::GameHexagonMap::addPlayerCard(const sf::Vector2u & coordinate, unsigned short cardID) {
	GameHexagonTile & gameHexagon = queryMap(coordinate);
	gameHexagon.hasPlayerCard = true;
	gameHexagon.playerCardIndex = playerCards.size();

	return playerCards.emplace_back(cardID);
}