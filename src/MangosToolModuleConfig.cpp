#include "MangostoolModuleConfig.h"
#include "Globals/ObjectMgr.h"
#include "Log/Log.h"

namespace cmangos_module
{
    MangostoolModuleConfig::MangostoolModuleConfig()
    : ModuleConfig("mangostool.conf")
    , enabled(true)
    {
    
    }

    bool MangostoolModuleConfig::OnLoad()
    {
        enabled = config.GetBoolDefault("MangosTool.Enable", true);

        return true;
    }
}