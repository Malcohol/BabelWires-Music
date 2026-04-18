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
    struct UserAdvisoryLogger;
}

namespace smf {
    /// Register factories etc. for Standard MIDI File support.
    babelwires::Result registerLib(babelwires::Context& context, babelwires::UserAdvisoryLogger& userLogger);
} // namespace smf
