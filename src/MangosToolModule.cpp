#include "MangosToolModule.h"

#include "Entities/GossipDef.h"
#include "Entities/Player.h"
#include "Globals/ObjectMgr.h"

#include <stdio.h>
#include <curl/curl.h>
#include <string>


namespace cmangos_module {
	void MangosToolModule::pingModel(const Player* player, const char* prefix, const char* message) {
		WorldPacket data;
		std::cout << message;
		std::cout << prefix;

        std::ostringstream out;
		out << message;

        char* buf = mangos_strdup(out.str().c_str());
        char* pos = buf;

        while (char* line = ChatHandler::LineFromMessage(pos))
        {
#if EXPANSION == 0
            ChatHandler::BuildChatPacket(data, CHAT_MSG_ADDON, line, LANG_ADDON);
#else
            ChatHandler::BuildChatPacket(data, CHAT_MSG_WHISPER, line, LANG_ADDON);
#endif
            player->GetSession()->SendPacket(data);
        }
		commandMap.insert(std::pair<std::string, std::string>(message));
		CURL *curl;
		CURLcode res;
		struct curl_slist *slist1;
		
		curl_global_init(CURL_GLOBAL_ALL);
		slist1 = NULL;
		slist1 = curl_slist_append(slist1, "Content-Type: application/json");
		
		curl = curl_easy_init();
		if(curl) {
			// Insert the URL of your local server
			curl_easy_setopt(curl, CURLOPT_URL, "");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, commandMap.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
		
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
	}
        delete[] buf;
}