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
        void pingModel(const Player* player, const char* prefix, const char* message);
        
    };
}
#endif  