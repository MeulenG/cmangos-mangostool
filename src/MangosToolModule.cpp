#include "MangosToolModule.h"

#include "Entities/Player.h"
#include "Globals/ObjectMgr.h"
#include "Globals/ObjectAccessor.h"
#include "Spells/SpellMgr.h"
#include "SystemConfig.h"
#include "World/World.h"

#include <stdio.h>
#include <curl/curl.h>
#include <string>


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
			{ "mt", std::bind(&MangostoolModule::pingModel, this, std::placeholders::_1, std::placeholders::_2), SEC_ADMINISTRATOR },
		};

		return &commandTable;
	}

	bool MangostoolModule::pingModel(WorldSession* session, const std::string& args)
	{
		std::string cleanArgs = args;
		if (cleanArgs.rfind("mt ", 0) == 0)
			cleanArgs = cleanArgs.substr(3);
		std::string commandMap = "{\"command\":\"" + args + "\"}";
		printf("CommandMap: %s\n", commandMap.c_str());

		CURL *curl;
		CURLcode res;
		struct curl_slist *slist1 = nullptr;

		curl_global_init(CURL_GLOBAL_ALL);
		slist1 = curl_slist_append(slist1, "Content-Type: application/json");

		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, commandMap.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();

		return true;
	}
}