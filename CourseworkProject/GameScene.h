#pragma once

#include "stdafx.h"

#include "SimpleGUIScene.h"

#include "qjson.h"
#include "Engine.h"
#include "ParticleManager.h"
#include "HexagonMap.h"

#include <algorithm>

namespace game {
	class PointOfInterest {
	public:
		enum POIType : unsigned short {
			POINTOFINTEREST_MOUNTAIN,
			POINTOFINTEREST_TIMBER,
			POINTOFINTEREST_STONE,
			POINTOFINTEREST_ARABLE, //Agriculture.
			POINTOFINTEREST_FISH,
			POINTOFINTEREST_IRON,
			POINTOFINTEREST_COAL,
			POINTOFINTEREST_SILICON,
			POINTOFINTEREST_ALUMINIUM,
			POINTOFINTEREST_MARBLE,
			POINTOFINTEREST_GOLD,
			POINTOFINTEREST_GEMS,
			POINTOFINTEREST_GEOTHERMAL, //Geothermally active.
			POINTOFINTEREST_OIL,
			POINTOFINTEREST_URANIUM,

			POINTOFINTEREST_COUNT
		};

	private:
		POIType type = static_cast<POIType>(0U);
		std::size_t textureCoordinateIndex = 0U;

	public:
		PointOfInterest() = default;
		PointOfInterest(unsigned short poiID);
		PointOfInterest(POIType poiType);
		~PointOfInterest() = default;

		inline unsigned short getID() const { return static_cast<unsigned short>(type); }
		inline POIType getType() const { return type; }
		inline std::size_t getTextureCoordinateIndex() const { return textureCoordinateIndex; }
	};

	class PlayerCard {
	public:
		enum CardType : unsigned short {
			PLAYERCARD_SPECIAL_HEADQUARTERS,
			PLAYERCARD_SPECIAL_OUTPOST,
			PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY,
			PLAYERCARD_PRODUCER_LOGGINGCAMP,
			PLAYERCARD_PRODUCER_FISHINGWHARF,
			PLAYERCARD_PRODUCER_FARM,
			PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX,
			PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY,
			PLAYERCARD_PRODUCER_MINE,
			PLAYERCARD_PRODUCER_OILJACK,
			PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX,
			PLAYERCARD_POWERPLANT_WINDTURBINES,
			PLAYERCARD_INTERMEDIATE_FACTORY_FOOD,
			PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL,
			PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL,
			PLAYERCARD_POWERPLANT_COAL,
			PLAYERCARD_POWERPLANT_GEOTHERMAL,
			PLAYERCARD_POWERPLANT_NUCLEAR,
			PLAYERCARD_CONSUMER_AIRPORT,
			PLAYERCARD_SPECIAL_LABORATORY,

			PLAYERCARD_COUNT
		};

		struct Adjacency {
		public:
			bool hasPointOfInterestAdjacency = false;
			game::PointOfInterest::POIType poiType = static_cast<game::PointOfInterest::POIType>(0U);

			bool hasPlayerCardAdjacency = false;
			game::PlayerCard::CardType cardType = static_cast<game::PlayerCard::CardType>(0U);

			Adjacency() = default;
		};

	private:
		CardType type = static_cast<CardType>(0U);
		std::size_t textureCoordinateIndex = 0U;

		bool hasPOI = false;
		PointOfInterest::POIType poiType = static_cast<PointOfInterest::POIType>(0U);

		Adjacency northWestAdjacency = Adjacency();
		Adjacency northAdjacency = Adjacency();
		Adjacency northEastAdjacency = Adjacency();
		Adjacency southEastAdjacency = Adjacency();
		Adjacency southAdjacency = Adjacency();
		Adjacency southWestAdjacency = Adjacency();

	public:
		PlayerCard() = default;
		PlayerCard(unsigned short cardID);
		PlayerCard(CardType cardType);
		~PlayerCard() = default;

		inline unsigned short getID() const { return type; }
		inline CardType getType() const { return type; }
		inline std::size_t getTextureCoordinateIndex() const { return textureCoordinateIndex; }

		inline bool hasPointOfInterest() const { return hasPOI; }
		inline PointOfInterest::POIType getPointOfInterest() { return poiType; }

		inline void setHasPointOfInterest(bool hasPOI) { this->hasPOI = hasPOI; }
		inline void setPointOfInterest(PointOfInterest::POIType poiType) { this->poiType = poiType; }

		inline Adjacency & getNorthWestAdjacency() { return northWestAdjacency; }
		inline Adjacency & getNorthAdjacency() { return northAdjacency; }
		inline Adjacency & getNorthEastAdjacency() { return northEastAdjacency; }
		inline Adjacency & getSouthEastAdjacency() { return southEastAdjacency; }
		inline Adjacency & getSouthAdjacency() { return southAdjacency; }
		inline Adjacency & getSouthWestAdjacency() { return southWestAdjacency; }
		Adjacency & getAdjacency(unsigned short adjacencyID);
		Adjacency getAdjacency(unsigned short adjacencyID) const;

		std::wstring & getStringName() const;
		sf::Color getBackColor() const;
		bool emitsSmoke() const;

		unsigned int getScore() const;

	private:
		bool hasCardAdjacency(PlayerCard::CardType cardType) const;
		bool hasPOIAdjacency(PointOfInterest::POIType pointOfInterest) const;

		bool multiplyPerCard(float & fractionalScore, float multiplier, PlayerCard::CardType playerCard) const;
		bool multiplyPerPOI(float & fractionalScore, float multiplier, PointOfInterest::POIType pointOfInterest) const;
	};

	struct GameHexagonTile {
		bool discovered = false;
		sf::Vector2f unitMapLocation = sf::Vector2f();

		bool hasPointOfInterest = false;
		std::size_t pointOfInterestIndex = 0U;

		bool hasPlayerCard = false;
		std::size_t playerCardIndex = 0U;

		GameHexagonTile() = default;
	};

	class GameHexagonMap : public mapping::HexagonMap<GameHexagonTile> {
	private:
		std::vector<game::PointOfInterest> pointsOfInterest = std::vector<game::PointOfInterest>();
		std::vector<game::PlayerCard> playerCards = std::vector<game::PlayerCard>();

	public:
		GameHexagonMap(int xCoordinate, int yCoordinate, unsigned int coordinateWidth, unsigned int coordinateHeight, const engine::Hexagon * hexagon);
		GameHexagonMap(const GameHexagonMap & gameHexagonMap);

		game::PointOfInterest & addPointOfInterest(const sf::Vector2u & coordinate, unsigned short poiID);
		game::PlayerCard & addPlayerCard(const sf::Vector2u & coordinate, unsigned short cardID);

		inline game::PointOfInterest & getPointOfInterest(std::size_t index) { return pointsOfInterest[index]; }
		inline game::PlayerCard & getPlayerCard(std::size_t index) { return playerCards[index]; }

		inline std::size_t getPointOfInterestCount() const { return pointsOfInterest.size(); }
		inline std::size_t getPlayerCardCount() const { return playerCards.size(); }
	};

	class Deck : public tgui::Widget {
	public:
		struct DeckCard {
			game::PlayerCard::CardType cardType = static_cast<game::PlayerCard::CardType>(0U);
			unsigned int textSize = 0U;
			std::size_t cardID = 0U;

			DeckCard() = default;
		};

	private:
		std::vector<DeckCard> cards = std::vector<DeckCard>();

		bool hoveringOverCard = false;
		bool clickedCard = false;
		std::size_t selectedCardIndex = 0U;

		std::shared_ptr<tgui::Text> deckNameText = std::make_shared<tgui::Text>();
		std::shared_ptr<tgui::Text> cardIDText = std::make_shared<tgui::Text>();

		float cardWidth = 0.F;
		float cardTextSpace = 0.F;
		unsigned int cardTextSize = 1U;
		float xWidth = 0.F;
		float xStart = 0.F;

	public:
		Deck(); 

		void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const override;
		inline bool mouseOnWidget(tgui::Vector2f position) const override { return sf::FloatRect(getPosition(), getSize()).contains(position); }
		virtual void setSize(const tgui::Layout2d & size);
		inline void setSize(tgui::Layout width, tgui::Layout height) { setSize( { std::move(width), std::move(height) } ); }

		virtual void mouseMoved(tgui::Vector2f position) override;
		virtual void leftMouseReleased(tgui::Vector2f position) override;
		inline virtual void mouseNoLongerOnWidget() override { hoveringOverCard = false; }
		Widget::Ptr clone() const override { return std::make_shared<Deck>(*this); }

		void shuffleDeck(std::mt19937_64 & randomEngine);
		void addCard(game::PlayerCard::CardType cardType);
		game::PlayerCard::CardType useCard();
		
		void clearDeck();
		inline bool hasSelectedCard() const { return clickedCard; }
		inline bool isEmpty() const { return cards.empty(); }
		inline game::PlayerCard::CardType getSelectedCardType() const { return cards[selectedCardIndex].cardType; }

		inline std::vector<DeckCard>::const_iterator begin() const { return cards.cbegin(); }
		inline std::vector<DeckCard>::const_iterator end() const { return cards.cend(); }
	};

	class GameScene : public scene::SimpleGUIScene {
	private:
		std::size_t gameScore = 0U;
		std::size_t maximumDeckCount = 0U;
		std::unique_ptr<std::vector<engine::CardPlacement>[]> cardPlacementHistory = std::make_unique<std::vector<engine::CardPlacement>[]>(0U);

		sf::VertexBuffer hoverVertexBuffer;
		sf::VertexBuffer playerCardBackgroundVertexBuffer;
		sf::VertexBuffer playerCardImageVertexBuffer;
		sf::VertexBuffer pointOfInterestVertexBuffer;

		sf::Vector2i mouseOffset = sf::Vector2i();
		float mouseMultiplier = 1.F;
		sf::Vector2f mouseWorldLocation = sf::Vector2f();

		std::vector<engine::ZoomLevel> calculatedZoomLevels = std::vector<engine::ZoomLevel>();
		engine::ZoomLevel * currentZoomLevel = nullptr;

		engine::Hexagon unitHexagon;
		engine::Hexagon backgroundOutsideHexagon;
		engine::Hexagon backgroundInsideHexagon;
		sf::Vector2f outInTranslation = sf::Vector2f();

		engine::CombinedRenderStates backgroundRenderStates;
		engine::CombinedRenderStates playerCardBackgroundRenderStates;
		engine::CombinedRenderStates playerCardImageRenderStates;
		engine::CombinedRenderStates hoverRenderStates;
		engine::CombinedRenderStates pointOfInterestRenderStates;

		particles::ParticleManager backgroundParticleManager;
		particles::ParticleManager foregroundParticleManager;

		sf::Color hoverColor;
		sf::Color goodHoverColor;
		sf::Color badHoverColor;
		sf::Color backgroundColor;
		sf::Color backgroundOverlayColor;
		sf::Color highlightColor;
		sf::Color lowlightColor;
		sf::Color midlightColor;
		sf::Color transparentHighlightColor;
		sf::Color transparentLowlightColor;
		sf::Color transparentMidlightColor;

		sf::Vector2f oldHoverLocation = sf::Vector2f();
		sf::Vector2f partialScroll = sf::Vector2f();
		sf::Vector2f fullScroll = sf::Vector2f();
		sf::Vector2i coordinateHoverLocation = sf::Vector2i();
		sf::Vector2f screenHoverLocation = sf::Vector2f();
		sf::Vector2f scrollStart = sf::Vector2f();

		std::shared_ptr<Deck> deck = std::make_shared<Deck>();
		std::size_t decksDealt = 0U;

		sf::Vector2i headquartersCoordinates = sf::Vector2i();
		std::uniform_real_distribution<float> cardDealerDistribution;

		std::vector<GameHexagonMap> hexagonMaps;
		GameHexagonMap ** visibleHexagonMaps;
		std::size_t hexagonMapCount;

		sf::Font font;
		sf::Text placementScoreIndicator;

		float defaultSideLength = 0.F;
		float cumulativeZoom = 0.F;
		float hoverHoldTime = 0.F;
		bool validPlacementHover = false;

		bool seedSet = false;
		std::string seed;
		std::size_t worldSeed = 0U;
		std::mt19937_64 sceneEngine;
		std::uniform_real_distribution<float> poiGeneratorDistribution;

		std::shared_ptr<tgui::Panel> hudTopPanelLeft;
		std::shared_ptr<tgui::Panel> hudTopPanelRight;
		std::shared_ptr<tgui::Panel> hudBottomPanel;

		std::shared_ptr<tgui::Label> scoreLabel;
		std::shared_ptr<tgui::Label> decksDealtLabel;
		std::shared_ptr<tgui::Button> menuButton;

		std::shared_ptr<tgui::Label> menuTitleLabel;
		std::shared_ptr<tgui::Panel> menuUnderlayPanel;
		std::shared_ptr<tgui::Panel> menuBackgroundPanel;
		std::shared_ptr<tgui::Label> menuSeedLabel;
		std::shared_ptr<tgui::Button> menuReturnToGameButton;
		std::shared_ptr<tgui::Button> menuSaveButton;
		std::shared_ptr<tgui::Button> menuGotoMainMenuButton;

	public:
		GameScene();
		GameScene(const std::string & seed, std::size_t deckCount);
		~GameScene() = default;

		void update(float deltaSeconds) override;
		void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const override;
		void mouseMoved(int x, int y) override;
		void mousePressed(sf::Mouse::Button buttonPressed) override;
		void mouseReleased(sf::Mouse::Button buttonReleased) override;
		void mouseWheelScrolled(float scrollDelta) override;
		inline void keyPressed(const sf::Event::KeyEvent & pressedEvent) override {}
		void keyReleased(const sf::Event::KeyEvent & releasedEvent) override;
		void letterBoxAdjusted(const sf::View & view, const sf::IntRect & letterBox) override;

		void pumpEvent(const sf::Event & event) override;

		void activated(sf::RenderTarget * renderTarget, const sf::IntRect & viewport) override;
		void deactivated(sf::RenderTarget * renderTarget) override;

		inline std::string getWorldStringSeed() const { return seed; }
		inline std::size_t getDeckCount() const { return maximumDeckCount; }
		inline std::size_t getGameScore() const { return gameScore; }

		bool saveGame(const std::wstring & fileName);
		bool loadGame(const std::wstring & fileName);

	private:
		bool hoveringOverValidPlacement();

		void generateZoomLevel();
		bool generateVisibleWorld();

		void updateBackgroundVertices();
		void updateHoverVertices();
		void updateBackgroundColors(const sf::Color & backgroundColor, const sf::Color & backgroundOverlayColor, const sf::Color & highlightColor, const sf::Color & lowlightColor);

		void collectWorldHexagons();
		void updateScroll();
		void drawNewDeck();

		inline void updateHoverColors() { hoverRenderStates.getShader()->setUniform("hoverOverlayColor", (leftMButtonDown && deck->hasSelectedCard() ? sf::Glsl::Vec4(validPlacementHover ? goodHoverColor : badHoverColor) : hoverColor)); }

		game::PlayerCard & placeSelectedCard(const sf::Vector2i & coordinates);
		game::PlayerCard & placePlayerCard(const sf::Vector2i & coordinates, PlayerCard::CardType cardType);
		bool getVisibleMaps(GameHexagonMap **& visibleHexagonMaps, std::size_t & hexagonMapCount); //Returns true if a new map was added.
		bool getParentMap(GameHexagonMap *& visibleHexagonMap, const sf::Vector2i & coordinate); //Returns true if a new map was added.

		void applyAdjacencies(const sf::Vector2i & coordinates, PlayerCard & playerCard, bool reverseAdjacencies = true);
		unsigned int calculatePotentialScore(const sf::Vector2i & coordinates, PlayerCard::CardType cardType);
	};
};