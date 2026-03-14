/**
 * An interface for objects which can consume audio data.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/common.hpp>

namespace babelwires {

    /// An interface for objects that can consume audio data.
    struct AudioDest {
        virtual ~AudioDest();

        /// Return the number of channels in this audio source.
        virtual int getNumChannels() const = 0;

        /// Return the frequency (sample rate) of this audio source.
        virtual Duration getFrequency() const = 0;

        /// Tell the dest to use bytes from the given buffer.
        /// Returns the number of bytes used.
        virtual unsigned long writeMoreAudioData(const AudioSample* buffer, unsigned long bufSize) = 0;
    };

} // namespace babelwires
