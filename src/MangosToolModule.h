#ifndef CMANGOS_MODULE_MANGOSTOOL_H
#define CMANGOS_MODULE_MANGOSTOOL_H

#include "Module.h"
#include "MangosToolModuleConfig.h"

#include <unordered_map>
#include <map>

namespace cmangos_module
{
    typedef std::unordered_map<std::string, std::string> commandMap;

    class MangosToolModule : public Module
    {
    public:
        MangosToolModule();

    private:
        

    private:
        
    };
}
#endif