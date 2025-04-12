#include "TransmogModuleConfig.h"
#include "Globals/ObjectMgr.h"
#include "Log/Log.h"

namespace cmangos_module
{
    TransmogModuleConfig::TransmogModuleConfig()
    : ModuleConfig("mangostool.conf")
    , enabled(false)
    {
    
    }

    bool TransmogModuleConfig::OnLoad()
    {
        enabled = config.GetBoolDefault("MangosTool.Enable", false);

        return true;
    }
}