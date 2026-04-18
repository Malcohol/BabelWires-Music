/**
 * Registration of factories for the Standard MIDI File support.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

namespace babelwires {
    class Context;
}

namespace smf {
    /// Registration factories etc. for Standard MIDI File.
    void registerLib(babelwires::Context& context);
} // namespace smf
