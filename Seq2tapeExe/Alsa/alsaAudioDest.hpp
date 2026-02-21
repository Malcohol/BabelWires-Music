/**
 * AlsaAudioDest is an AudioDest for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <Seq2tapeLib/Audio/audioDest.hpp>
#include <BaseLib/Utilities/result.hpp>

#include <memory>
#include <string>

namespace babelwires_alsa {

     /// AlsaAudioDest is an AudioDest for ALSA (Advanced Linux Sound System).
     class AlsaAudioDest : public babelwires::AudioDest {
     public:
       static babelwires::ResultT<AlsaAudioDest> open(const char* pcmHandleName);

       AlsaAudioDest(AlsaAudioDest&&) = default;
       AlsaAudioDest& operator=(AlsaAudioDest&&) = default;
       AlsaAudioDest(const AlsaAudioDest&) = delete;
       AlsaAudioDest& operator=(const AlsaAudioDest&) = delete;

       ~AlsaAudioDest();

       virtual int getNumChannels() const override;

       virtual babelwires::Duration getFrequency() const override;

       virtual unsigned long writeMoreAudioData(const babelwires::AudioSample* buffer, unsigned long bufSize) override;

     protected:
       struct Impl;

       explicit AlsaAudioDest(std::unique_ptr<Impl> impl);

     private:
       std::unique_ptr<Impl> m_impl;
   };

} // namespace babelwires_alsa
