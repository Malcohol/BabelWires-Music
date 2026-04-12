/**
 * Register factories etc. for the MusicLibUi into BabelWiresUi.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/MusicLibUi/musicLibUiExport.hpp>

namespace babelwires {
    struct Context;
}

namespace bw_musicUi {
    /// This is not a true plugin model, because everything is statically linked.
    MUSICLIBUI_API void registerLib(babelwires::Context& context);
} // namespace bw_musicUi
