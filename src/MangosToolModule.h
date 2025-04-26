#ifndef CMANGOS_MODULE_MANGOSTOOL_H
#define CMANGOS_MODULE_MANGOSTOOL_H

#include "Module.h"
#include "MangosToolModuleConfig.h"

#include <unordered_map>
#include <map>

namespace cmangos_module
{
    typedef std::unordered_map<std::string, std::string> commandMap;

    class MangostoolModule : public Module
    {
    public:
        MangostoolModule();
        const MangostoolModuleConfig* GetConfig() const override;
        bool addData(WorldSession* session, const std::string& args);
        bool pingModel(WorldSession* session, const std::string& args);
        std::vector<ModuleChatCommand>* GetCommandTable() override;
        const char* GetChatCommandPrefix() const override { return "mangostool"; }
        
    };
}
static cmangos_module::MangostoolModule sMangostoolModule;
#endif  // CMANGOS_MODULE_MANGOSTOOL_H