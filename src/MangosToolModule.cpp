#include "MangosToolModule.h"

#include "Entities/Player.h"
#include "Globals/ObjectMgr.h"
#include "Globals/ObjectAccessor.h"
#include "Spells/SpellMgr.h"
#include "SystemConfig.h"
#include "World/World.h"
#include "Chat/Chat.h"

#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <list>


namespace cmangos_module {
	MangostoolModule::MangostoolModule()
		: Module("Mangostool", new MangostoolModuleConfig())
	{

	}

	const cmangos_module::MangostoolModuleConfig* MangostoolModule::GetConfig() const
	{
		return (MangostoolModuleConfig*)Module::GetConfig();
	}
	
	std::vector<ModuleChatCommand>* MangostoolModule::GetCommandTable()
	{
		static std::vector<ModuleChatCommand> commandTable =
		{
			{ "teleport", std::bind(&MangostoolModule::pingModel, this, std::placeholders::_1, std::placeholders::_2), SEC_ADMINISTRATOR },
			{ "additem", std::bind(&MangostoolModule::pingModel, this, std::placeholders::_1, std::placeholders::_2), SEC_ADMINISTRATOR },
			{ "trackplayer", std::bind(&MangostoolModule::addData, this, std::placeholders::_1, std::placeholders::_2), SEC_ADMINISTRATOR }
		};

		return &commandTable;
	}

	bool MangostoolModule::addData(WorldSession* session, const std::string& args){
		std::ofstream file;
		file.open("mapping.csv", std::ios::app);
		if (file.is_open())
		{
			std::cout << "Successfully Opened File: mapping.csv \n";
		}
		Player* player = session->GetPlayer();
		uint32 mapId = player->GetMapId();
		float posX = player->GetPositionX();
		float posY = player->GetPositionY();
		float posZ = player->GetPositionZ();
		AreaNameInfo areaName = player->GetTerrain()->GetAreaName(posX, posY, posZ, 0);
		// Write it to the file in the format : Areaname,mapid,x,y,z
		file << areaName.areaName << "," << mapId << "," << posX << "," << posY << "," << posZ << "\n";
		sLog.outString("Player: %s, MapId: %u, X: %f, Y: %f, Z: %f\n", player->GetName(), mapId, posX, posY, posZ);
		file.close();
		sLog.outString("File closed \n");
		return true;
	}
	

	static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	bool MangostoolModule::pingModel(WorldSession* session, const std::string& args)
	{
		WorldPacket data;
		std::string commandMap = "{\"command\":\"" + args + "\"}";
		sLog.outString("CommandMap: %s\n", commandMap.c_str());

		CURL* curl;
		CURLcode res;
		std::string responseString;
		struct curl_slist* slist1 = nullptr;

		curl_global_init(CURL_GLOBAL_ALL);
		slist1 = curl_slist_append(slist1, "Content-Type: application/json");

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.0.21:5000/wow-model");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, commandMap.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			Player* player = session->GetPlayer();
			ChatHandler chatHandler(player);
			sLog.outString("Response: %s\n", responseString.c_str());
			if (responseString.find("go") != std::string::npos) {
				char input[100] = { 0 };
				std::list<std::string> teleportList;
				strncpy(input, responseString.c_str(), sizeof(input) - 1);
				const char* delimiter = " ";
				char* token = strtok(input, delimiter);
				// Model will always answer in the format: .go x y z map_id
				while (token) {
					std::cout << token << "\n";
					teleportList.push_back(token);
					token = strtok(nullptr, delimiter);
				}
				if (teleportList.size() < 4) {
					sLog.outString("Invalid teleport command format.\n");
					return false;
				}
				// Get the first three tokens as coordinates
				// First remove .go
				teleportList.pop_front();
				float x = std::stof(teleportList.front());
				teleportList.pop_front();
				float y = std::stof(teleportList.front());
				teleportList.pop_front();
				float z = std::stof(teleportList.front());
				teleportList.pop_front();
				uint32 map_id = std::stoi(teleportList.front());
				teleportList.pop_front();
				WorldLocation loc;
				loc.coord_x = x;
				loc.coord_y = y;
				loc.coord_z = z;
				loc.mapid = map_id;
				player->TeleportTo(loc);
				sLog.outString("Teleporting to: %f %f %f %u\n", x, y, z, map_id);
			}
			else if (responseString.find("additem") != std::string::npos) {
				char input[100] = { 0 };
				std::list<std::string> teleportList;
				strncpy(input, responseString.c_str(), sizeof(input) - 1);
				const char* delimiter = " ";
				char* token = strtok(input, delimiter);
				// Model will always answer in the format: .go x y z map_id
				while (token) {
					std::cout << token << "\n";
					teleportList.push_back(token);
					token = strtok(nullptr, delimiter);
				}
				if (teleportList.size() < 4) {
					sLog.outString("Invalid teleport command format.\n");
					return false;
				}
				// format is .additem item_id
				// first remove .additem
				teleportList.pop_front();
				uint32 item_id = std::stoi(teleportList.front());
				Item item;
				item.CreateItem(item_id, 1, player);
				player->AddMItem(&item);
				sLog.outString("Adding item: %s\n", responseString.c_str());
			}
			else {
				sLog.outString("Either an invalid query or Invalid command format.\n");
			}
			// Cleanup
			curl_easy_cleanup(curl);
			curl_slist_free_all(slist1);
		}
		curl_global_cleanup();

		return true;
	}

}