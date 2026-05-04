/**
 * Registration of factories for the Music Test Plugin.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/Result/result.hpp>

namespace babelwires {
    class Context;
    struct UserAdvisoryLogger;
}

namespace bw_music_testplugin {
    babelwires::Result registerLib(babelwires::Context& context, babelwires::UserAdvisoryLogger& userLogger);
} // namespace bw_music_testplugin