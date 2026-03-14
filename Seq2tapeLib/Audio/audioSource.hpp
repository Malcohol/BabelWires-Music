/**
 * An interface for objects that provide audio data.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/common.hpp>

namespace babelwires {

    /// An interface for objects that provide audio data.
    struct AudioSource {
        virtual ~AudioSource();

        /// Return the number of channels in this audio source.
        virtual int getNumChannels() const = 0;

        /// Return the frequency (sample rate) of this audio source.
        virtual Duration getFrequency() const = 0;

        /// Tell the source to get bytes and put them in given buffer.
        /// Returns the number of bytes that was put in the buffer.
        virtual unsigned long getMoreAudioData(AudioSample* buffer, unsigned long bufSize) = 0;
    };

} // namespace babelwires
