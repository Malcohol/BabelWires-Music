/**
 * Registration of factories for the Music Test Plugin.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/Plugins/TestPlugin/testPluginLibExport.hpp>

namespace babelwires {
    struct ProjectContext;
}

namespace bw_music_testplugin {
    TESTPLUGINLIB_API void registerLib(babelwires::ProjectContext& context);
} // namespace bw_music_testplugin