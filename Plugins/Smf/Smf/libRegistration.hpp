/**
 * Registration of factories for Standard MIDI File support.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/Result/result.hpp>

namespace babelwires {
    class Context;
    struct UserLogger;
}

namespace smf {
    /// Register factories etc. for Standard MIDI File support.
    babelwires::Result registerLib(babelwires::Context& context, babelwires::UserLogger& userLogger);
} // namespace smf
