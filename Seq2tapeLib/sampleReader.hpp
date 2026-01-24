/**
 * A SampleReader manages the buffered reading of samples from an AudioSource.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/common.hpp>

namespace babelwires {
    struct AudioSource;
}

namespace seq2tape {
    /// A SampleReader manages the buffered reading of samples from an AudioSource.
    class SampleReader {
      public:
        SampleReader(babelwires::AudioSource& audioSource);

        enum SampleResult { SAMPLE_EOF, SAMPLE_OK };

        /// Multiple channels are flattened into one monophonic sample value.
        SampleResult getNextSample(babelwires::AudioSample& sample);

        /// Returns the position of the last sample read in seconds.
        babelwires::Duration getPositionInStream() const;

        /// Return the total number of samples read since construction.
        unsigned long getNumSamplesRead() const;

      private:
        enum { BUFSIZE = 1024 };

        babelwires::AudioSource& m_audioSource;
        const unsigned int m_numChannels;
        babelwires::AudioSample m_buffer[BUFSIZE];
        unsigned long m_numSamplesInBuffer;
        unsigned long m_cursorInBuffer;
        unsigned long m_numSamplesRead;
        babelwires::Duration m_samplePeriod;
    };
} // namespace seq2tape