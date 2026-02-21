/**
 * AlsaAudioInterface is an AudioInterface for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <Seq2tapeLib/Audio/audioInterface.hpp>

namespace babelwires_alsa {

    /// This provides an AudioInterface implementation for ALSA (Advanced Linux Sound System).
    class AlsaAudioInterface : public babelwires::AudioInterface {
      public:
        AlsaAudioInterface();
        static babelwires::LongId getThisIdentifier();

        virtual std::vector<std::string> getDestinationNames() const override;
        virtual babelwires::ResultT<std::unique_ptr<babelwires::AudioDest>> getDestination(
            std::string_view destinationName) const override;
        virtual std::vector<std::string> getSourceNames() const override;
        virtual babelwires::ResultT<std::unique_ptr<babelwires::AudioSource>> getSource(
            std::string_view sourceName) const override;
    };

} // namespace babelwires_alsa
