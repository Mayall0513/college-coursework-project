#pragma once

#include "stdafx.h"

#include <random>
#include <filesystem>

namespace game {
	namespace resources {
		constexpr float PI = 3.1415926535897932384626433832795F;
		constexpr float  e = 2.7182818284590452353602874713527F;

		extern sf::String STRING_WINDOWTITLE;

		extern sf::String STRING_TEXTURE_POINTSOFINTEREST_LOCATION;
		extern sf::String STRING_TEXTURE_PLAYERCARDS_LOCATION;
		extern sf::String STRING_TEXTURE_PARTICLES_LOCATION;

		extern std::filesystem::path PATH_SETTINGSFILE;
		extern std::filesystem::path PATH_SAVEGAME;

		extern std::wstring STRING_PLAYERCARD_SPECIAL_HEADERQUARTERS_NAME;
		extern std::wstring STRING_PLAYERCARD_SPECIAL_OUTPOST_NAME;
		extern std::wstring STRING_PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY_NAME;
		extern std::wstring STRING_PLAYERCARD_PRODUCER_LOGGINGCAMP_NAME;
		extern std::wstring STRING_PLAYERCARD_PRODUCER_FISHINGWHARF_NAME;
		extern std::wstring STRING_PLAYERCARD_PRODUCER_FARM_NAME;
		extern std::wstring STRING_PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX_NAME;
		extern std::wstring STRING_PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY_NAME;
		extern std::wstring STRING_PLAYERCARD_PRODUCER_MINE_NAME;
		extern std::wstring STRING_PLAYERCARD_PRODUCER_OILJACK_NAME;
		extern std::wstring STRING_PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX_NAME;
		extern std::wstring STRING_PLAYERCARD_POWERPLANT_WINDTURBINE_NAME;
		extern std::wstring STRING_PLAYERCARD_INTERMEDIATE_FACTORY_FOOD_NAME;
		extern std::wstring STRING_PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL_NAME;
		extern std::wstring STRING_PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL_NAME;
		extern std::wstring STRING_PLAYERCARD_POWERPLANT_COAL_NAME;
		extern std::wstring STRING_PLAYERCARD_POWERPLANT_GEOTHERMAL_NAME;
		extern std::wstring STRING_PLAYERCARD_POWERPLANT_NUCLEAR_NAME;
		extern std::wstring STRING_PLAYERCARD_CONSUMER_AIRPORT_NAME;
		extern std::wstring STRING_PLAYERCARD_SPECIAL_LABORATORY_NAME;

		extern std::uniform_real_distribution<float> ZERO_TWOPI_DISTRIBUTION;
		extern std::uniform_real_distribution<float> ZERO_ONE_DISTRIBUTION;

		extern std::string SHADERVERT_IDENTITY_MEM;
		extern std::string SHADERVERT_OFFSET_MEM;
		extern std::string SHADERFRAG_HOVER_MEM;
		extern std::string SHADERFRAG_VERTEXCOLOR_MEM;

		extern std::unique_ptr<sf::Vector2f[]> GAME_TEXTURECOORDINATES_PARTICLE;
		extern std::unique_ptr<sf::Vector2f[]> GAME_TEXTURECOORDINATES_PLAYERCARD;
		extern std::unique_ptr<sf::Vector2f[]> GAME_TEXTURECOORDINATES_POINTOFINTEREST;
		extern sf::Vector2i ** GAME_ADJACENCYOFFSETS;

		extern std::ranlux24_base GAME_RANDOMENGINE;

		extern sf::Color COLOR_CARDTYPE_CONSUMER; //Azure.
		extern sf::Color COLOR_CARDTYPE_PRODUCER; //Venetian Red.
		extern sf::Color COLOR_CARDTYPE_POWERPLANT; //Electric Purple.
		extern sf::Color COLOR_CARDTYPE_INTERMEDIATE; //Forest Green.
		extern sf::Color COLOR_CARDTYPE_SPECIAL; //Gold.

		constexpr float GAME_INSIDEHEXAGON_PERCENTAGE = 0.890625F;
		constexpr float GAME_ZOOMMULTIPLIER = 1.21875F;
		constexpr float GAME_MAXIMUMZOOM = 4.F;
		constexpr float GAME_MINIMUMZOOM = -8.F;
		constexpr float GAME_RADIALANIMATION_SECONDS = 0.1875F;
		constexpr float GAME_POI_FREQUENCY = 0.21875F; //~21.9%

		constexpr unsigned int GAME_MAPDIMENSIONS = 400U; //Hexagon maps are 400x400. 
		constexpr unsigned int GAME_DECK_SIZE = 8U;

		void commonInitialization();
	}
}

namespace resources = game::resources;