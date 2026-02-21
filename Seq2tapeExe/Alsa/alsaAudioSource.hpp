/**
 * AlsaAudioSource is an AudioSource for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <Seq2tapeLib/Audio/audioSource.hpp>
#include <BaseLib/Utilities/result.hpp>

#include <memory>
#include <string>

namespace babelwires_alsa {

    /// AlsaAudioSource is an AudioSource for ALSA (Advanced Linux Sound System).
    class AlsaAudioSource : public babelwires::AudioSource {
      public:
        static babelwires::ResultT<AlsaAudioSource> open(const char* pcmHandleName);

        AlsaAudioSource(AlsaAudioSource&&) = default;
        AlsaAudioSource& operator=(AlsaAudioSource&&) = default;
        AlsaAudioSource(const AlsaAudioSource&) = delete;
        AlsaAudioSource& operator=(const AlsaAudioSource&) = delete;

        ~AlsaAudioSource();

        virtual int getNumChannels() const override;

        virtual babelwires::Duration getFrequency() const override;

        virtual unsigned long getMoreAudioData(babelwires::AudioSample* buffer, unsigned long bufSize) override;

      protected:
        struct Impl;

        explicit AlsaAudioSource(std::unique_ptr<Impl> impl);

      private:
        std::unique_ptr<Impl> m_impl;
    };
} // namespace babelwires_alsa
