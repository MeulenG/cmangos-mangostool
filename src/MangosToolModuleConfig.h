#pragma once
#include "ModuleConfig.h"

namespace cmangos_module
{
    class MangostoolModuleConfig : public ModuleConfig
    {
    public:
        MangostoolModuleConfig();
        bool OnLoad() override;

    public:
        bool enabled;
    };
}