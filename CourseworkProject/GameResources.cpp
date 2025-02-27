#include "stdafx.h"

#include "GameResources.h"

#include "GameScene.h"
#include "WindowsFunctions.h"

#include <fstream>

sf::String game::resources::STRING_WINDOWTITLE = sf::String("Coursework Project");

sf::String game::resources::STRING_TEXTURE_POINTSOFINTEREST_LOCATION = sf::String("content\\image\\pointsofinterest.png");
sf::String game::resources::STRING_TEXTURE_PLAYERCARDS_LOCATION = sf::String("content\\image\\playerpieces.png");
sf::String game::resources::STRING_TEXTURE_PARTICLES_LOCATION = sf::String("content\\image\\particles.png");

std::filesystem::path game::resources::PATH_SETTINGSFILE = std::filesystem::current_path().append(L"settings.json");
std::filesystem::path game::resources::PATH_SAVEGAME = std::filesystem::current_path().append(L"savegames");

std::wstring game::resources::STRING_PLAYERCARD_SPECIAL_HEADERQUARTERS_NAME = std::wstring(L"Headquarters"); //PLAYERCARD_SPECIAL_HEADQUARTERS
std::wstring game::resources::STRING_PLAYERCARD_SPECIAL_OUTPOST_NAME = std::wstring(L"Outpost"); //PLAYERCARD_SPECIAL_OUTPOST
std::wstring game::resources::STRING_PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY_NAME = std::wstring(L"Low-Density Residential Housing"); //PLAYERCARD_CONSUMER_RESIDENTIAL_LOWDENSITY
std::wstring game::resources::STRING_PLAYERCARD_PRODUCER_LOGGINGCAMP_NAME = std::wstring(L"Logging Camp"); //PLAYERCARD_PRODUCER_LOGGINGCAMP
std::wstring game::resources::STRING_PLAYERCARD_PRODUCER_FISHINGWHARF_NAME = std::wstring(L"Fishing Wharf"); //PLAYERCARD_PRODUCER_FISHINGWHARF
std::wstring game::resources::STRING_PLAYERCARD_PRODUCER_FARM_NAME = std::wstring(L"Farm"); //PLAYERCARD_PRODUCER_FARM
std::wstring game::resources::STRING_PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX_NAME = std::wstring(L"Shopping Complex"); //PLAYERCARD_CONSUMER_SHOPPINGCOMPLEX
std::wstring game::resources::STRING_PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY_NAME = std::wstring(L"High-Density Residential Housing"); //PLAYERCARD_CONSUMER_RESIDENTIAL_HIGHDENSITY
std::wstring game::resources::STRING_PLAYERCARD_PRODUCER_MINE_NAME = std::wstring(L"Mine"); //PLAYERCARD_PRODUCER_MINE
std::wstring game::resources::STRING_PLAYERCARD_PRODUCER_OILJACK_NAME = std::wstring(L"Oil Jack"); //PLAYERCARD_PRODUCER_OILJACK
std::wstring game::resources::STRING_PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX_NAME = std::wstring(L"Business Complex"); //PLAYERCARD_INTERMEDIATE_BUSINESSCOMPLEX
std::wstring game::resources::STRING_PLAYERCARD_POWERPLANT_WINDTURBINE_NAME = std::wstring(L"Wind Turbines"); //PLAYERCARD_POWERPLANT_WINDTURBINE
std::wstring game::resources::STRING_PLAYERCARD_INTERMEDIATE_FACTORY_FOOD_NAME = std::wstring(L"Factory (Food)"); //PLAYERCARD_INTERMEDIATE_FACTORY_FOOD
std::wstring game::resources::STRING_PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL_NAME = std::wstring(L"Factory (Mineral)"); //PLAYERCARD_INTERMEDIATE_FACTORY_MINERAL
std::wstring game::resources::STRING_PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL_NAME = std::wstring(L"Factory (Chemical)"); //PLAYERCARD_INTERMEDIATE_FACTORY_CHEMICAL
std::wstring game::resources::STRING_PLAYERCARD_POWERPLANT_COAL_NAME = std::wstring(L"Coal Fired Power Plant"); //PLAYERCARD_POWERPLANT_COAL
std::wstring game::resources::STRING_PLAYERCARD_POWERPLANT_GEOTHERMAL_NAME = std::wstring(L"Geothermal Power Plant"); //PLAYERCARD_POWERPLANT_GEOTHERMAL
std::wstring game::resources::STRING_PLAYERCARD_POWERPLANT_NUCLEAR_NAME = std::wstring(L"Nuclear Power Plant"); //PLAYERCARD_POWERPLANT_NUCLEAR
std::wstring game::resources::STRING_PLAYERCARD_CONSUMER_AIRPORT_NAME = std::wstring(L"Airport"); //PLAYERCARD_CONSUMER_AIRPORT
std::wstring game::resources::STRING_PLAYERCARD_SPECIAL_LABORATORY_NAME = std::wstring(L"Laboratory"); //PLAYERCARD_SPECIAL_LABORATORY

std::uniform_real_distribution<float> game::resources::ZERO_TWOPI_DISTRIBUTION = std::uniform_real_distribution<float>(0.F, 2.F * game::resources::PI);
std::uniform_real_distribution<float> game::resources::ZERO_ONE_DISTRIBUTION = std::uniform_real_distribution<float>(0.F, 1.F);

std::string game::resources::SHADERVERT_IDENTITY_MEM = std::string();
std::string game::resources::SHADERVERT_OFFSET_MEM = std::string();
std::string game::resources::SHADERFRAG_HOVER_MEM = std::string();
std::string game::resources::SHADERFRAG_VERTEXCOLOR_MEM = std::string();

std::unique_ptr<sf::Vector2f[]> game::resources::GAME_TEXTURECOORDINATES_PARTICLE = std::unique_ptr<sf::Vector2f[]>();
std::unique_ptr<sf::Vector2f[]> game::resources::GAME_TEXTURECOORDINATES_PLAYERCARD = std::unique_ptr<sf::Vector2f[]>();
std::unique_ptr<sf::Vector2f[]> game::resources::GAME_TEXTURECOORDINATES_POINTOFINTEREST = std::unique_ptr<sf::Vector2f[]>();
sf::Vector2i ** game::resources::GAME_ADJACENCYOFFSETS = nullptr;

std::ranlux24_base game::resources::GAME_RANDOMENGINE;

sf::Color game::resources::COLOR_CARDTYPE_CONSUMER = sf::Color(0U, 127U, 255U, 255U);
sf::Color game::resources::COLOR_CARDTYPE_PRODUCER = sf::Color(200U, 8U, 21U, 255U);
sf::Color game::resources::COLOR_CARDTYPE_POWERPLANT = sf::Color(191U, 0U, 255U, 255U);
sf::Color game::resources::COLOR_CARDTYPE_INTERMEDIATE = sf::Color(34U, 139U, 34U, 255U);
sf::Color game::resources::COLOR_CARDTYPE_SPECIAL = sf::Color(255U, 215U, 0U, 255U);

void game::resources::commonInitialization() {
	std::ifstream commonFileStream;
	std::ostringstream commonStringStream;

	commonFileStream.open("content\\shaders\\shader_identity.vert", std::ios::in); //Read the shader to a stream.
	if (!commonFileStream.fail()) { //Check if everything went ok.
		commonStringStream << commonFileStream.rdbuf(); //Move the stream's contents to a string stream.
		SHADERVERT_IDENTITY_MEM = commonStringStream.str(); //Get the shaders' contents as a string.
		std::ostringstream().swap(commonStringStream); //Clear the string stream.
	}

	else {
		win32::createErrorBox("ERROR", "Error when opening \"content\\shaders\\shader_identity.vert\"!");
	}

	std::ifstream().swap(commonFileStream); //Clear the filestram.
	commonFileStream.open("content\\shaders\\shader_offset.vert", std::ios::in);

	if (!commonFileStream.fail()) {
		commonStringStream << commonFileStream.rdbuf();
		SHADERVERT_OFFSET_MEM = commonStringStream.str();
		std::ostringstream().swap(commonStringStream);
	}

	else {
		win32::createErrorBox("ERROR", "Error when opening \"content\\shaders\\shader_offset.vert\"!");
	}

	std::ifstream().swap(commonFileStream);
	commonFileStream.open("content\\shaders\\shader_hover.frag", std::ios::in);

	if (!commonFileStream.fail()) {
		commonStringStream << commonFileStream.rdbuf();
		SHADERFRAG_HOVER_MEM = commonStringStream.str();
		std::ostringstream().swap(commonStringStream);
	}

	else {
		win32::createErrorBox("ERROR", "Error when opening \"content\\shaders\\shader_hover.frag\"!");
	}

	std::ifstream().swap(commonFileStream);
	commonFileStream.open("content\\shaders\\shader_vertexcolor.frag", std::ios::in);

	if (!commonFileStream.fail()) {
		commonStringStream << commonFileStream.rdbuf();
		SHADERFRAG_VERTEXCOLOR_MEM = commonStringStream.str();
		std::ostringstream().swap(commonStringStream);
	}

	else {
		win32::createErrorBox("ERROR", "Error when opening \"content\\shaders\\shader_vertexcolor.frag\"!");
	}

	std::ifstream().swap(commonFileStream);

	GAME_TEXTURECOORDINATES_PARTICLE = std::make_unique<sf::Vector2f[]>(16U);
	GAME_TEXTURECOORDINATES_PLAYERCARD = std::make_unique<sf::Vector2f[]>(4U * static_cast<unsigned short>(game::PlayerCard::CardType::PLAYERCARD_COUNT));
	GAME_TEXTURECOORDINATES_POINTOFINTEREST = std::make_unique<sf::Vector2f[]>(4U * static_cast<unsigned short>(game::PointOfInterest::POIType::POINTOFINTEREST_COUNT));

	{
		unsigned int i = 0U;
		for (float x = 0.F; x < 128.F; x += 64.F) {
			for (float y = 0.F; y < 128.F; y += 64.F, i += 4U) {
				GAME_TEXTURECOORDINATES_PARTICLE[i] = sf::Vector2f(x, y);
				GAME_TEXTURECOORDINATES_PARTICLE[i + 1U] = sf::Vector2f(x + 64.F, y);
				GAME_TEXTURECOORDINATES_PARTICLE[i + 2U] = sf::Vector2f(x + 64.F, y + 64.F);
				GAME_TEXTURECOORDINATES_PARTICLE[i + 3U] = sf::Vector2f(x, y + 64.F);
			}
		}
	}

	{
		unsigned int i = 0U;
		for (float y = 0.F; y < 128.F; y += 64.F) {
			for (float x = 0.F; x < 512.F; x += 64.F, i += 4U) {
				GAME_TEXTURECOORDINATES_PLAYERCARD[i] = sf::Vector2f(x, y);
				GAME_TEXTURECOORDINATES_PLAYERCARD[i + 1U] = sf::Vector2f(x + 64.F, y);
				GAME_TEXTURECOORDINATES_PLAYERCARD[i + 2U] = sf::Vector2f(x + 64.F, y + 64.F);
				GAME_TEXTURECOORDINATES_PLAYERCARD[i + 3U] = sf::Vector2f(x, y + 64.F);
			}
		}

		for (float x = 0.F; x < 256.F; x += 64.F, i += 4U) {
			GAME_TEXTURECOORDINATES_PLAYERCARD[i] = sf::Vector2f(x, 128.F);
			GAME_TEXTURECOORDINATES_PLAYERCARD[i + 1U] = sf::Vector2f(x + 64.F, 128.F);
			GAME_TEXTURECOORDINATES_PLAYERCARD[i + 2U] = sf::Vector2f(x + 64.F, 192.F);
			GAME_TEXTURECOORDINATES_PLAYERCARD[i + 3U] = sf::Vector2f(x, 192.F);
		}
	}

	{
		unsigned int i = 0U;
		for (float x = 0.F; x < 512.F; x += 64.F, i += 4U) {
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i] = sf::Vector2f(x, 0.F);
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i + 1U] = sf::Vector2f(x + 64.F, 0.F);
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i + 2U] = sf::Vector2f(x + 64.F, 64.F);
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i + 3U] = sf::Vector2f(x, 64.F);
		}

		for (float x = 0.F; x < 448.F; x += 64.F, i += 4U) {
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i] = sf::Vector2f(x, 64.F);
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i + 1U] = sf::Vector2f(x + 64.F, 64.F);
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i + 2U] = sf::Vector2f(x + 64.F, 128.F);
			GAME_TEXTURECOORDINATES_POINTOFINTEREST[i + 3U] = sf::Vector2f(x, 128.F);
		}
	}

	GAME_ADJACENCYOFFSETS = static_cast<sf::Vector2i **>(malloc(sizeof(sf::Vector2i *) * 2U));
	GAME_ADJACENCYOFFSETS[0U] = static_cast<sf::Vector2i *>(malloc(sizeof(sf::Vector2i) * 6U)); //Even column (x & 1 == false).
	GAME_ADJACENCYOFFSETS[1U] = static_cast<sf::Vector2i *>(malloc(sizeof(sf::Vector2i) * 6U)); //Odd column (x & 1 == true).

	GAME_ADJACENCYOFFSETS[0U][0U] = sf::Vector2i(-1, 0);  //North west.
	GAME_ADJACENCYOFFSETS[0U][1U] = sf::Vector2i(0, -1);  //North.
	GAME_ADJACENCYOFFSETS[0U][2U] = sf::Vector2i(1, 0);   //North east.
	GAME_ADJACENCYOFFSETS[0U][3U] = sf::Vector2i(1, 1);   //South east.
	GAME_ADJACENCYOFFSETS[0U][4U] = sf::Vector2i(0, 1);   //South.
	GAME_ADJACENCYOFFSETS[0U][5U] = sf::Vector2i(-1, 1);  //South west.

	GAME_ADJACENCYOFFSETS[1U][0U] = sf::Vector2i(-1, -1); //North west.
	GAME_ADJACENCYOFFSETS[1U][1U] = sf::Vector2i(0, -1);  //North.
	GAME_ADJACENCYOFFSETS[1U][2U] = sf::Vector2i(1, -1);  //North east.
	GAME_ADJACENCYOFFSETS[1U][3U] = sf::Vector2i(1, 0);   //South east.
	GAME_ADJACENCYOFFSETS[1U][4U] = sf::Vector2i(0, 1);   //South.
	GAME_ADJACENCYOFFSETS[1U][5U] = sf::Vector2i(-1, 0);  //South west.

	GAME_RANDOMENGINE.seed(std::random_device()()); //Seed the game's global random number generator with a random seed.
}
