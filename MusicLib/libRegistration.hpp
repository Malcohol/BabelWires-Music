/**
 * Register factories etc. for BabelWires-Music into BabelWires.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

namespace babelwires {
    class Context;
}

namespace bw_music {
    /// This is not a true plugin model, because everything is statically linked.
    MUSICLIB_API void registerLib(babelwires::Context& context);
} // namespace bw_music
