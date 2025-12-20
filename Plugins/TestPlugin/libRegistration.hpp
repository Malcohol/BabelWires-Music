/**
 * Registration of factories for the Music Test Plugin.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

namespace babelwires {
    struct ProjectContext;
}

namespace bw_music_testplugin {
    void registerLib(babelwires::ProjectContext& context);
} // namespace bw_music_testplugin