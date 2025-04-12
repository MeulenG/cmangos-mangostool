#include "MangostoolModuleConfig.h"
#include "Globals/ObjectMgr.h"
#include "Log/Log.h"

namespace cmangos_module
{
    MangostoolModuleConfig::MangostoolModuleConfig()
    : ModuleConfig("mangostool.conf")
    , enabled(false)
    {
    
    }

    bool MangostoolModuleConfig::OnLoad()
    {
        enabled = config.GetBoolDefault("MangosTool.Enable", false);

        return true;
    }
}