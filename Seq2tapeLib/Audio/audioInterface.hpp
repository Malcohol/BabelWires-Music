/**
 * A system for registering AudioSources and AudioDests.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/Registry/registry.hpp>
#include <BaseLib/Result/result.hpp>

namespace babelwires {

    struct AudioDest;
    struct AudioSource;

    /// A given audio interface (e.g. Alsa) must provide an implementation of this interface.
    class AudioInterface : public RegistryEntry {
      public:
        AudioInterface(LongId identifier, VersionNumber version);

        virtual std::vector<std::string> getDestinationNames() const = 0;
        virtual ResultT<std::unique_ptr<babelwires::AudioDest>> getDestination(
            std::string_view destinationName) const = 0;

        virtual std::vector<std::string> getSourceNames() const = 0;
        virtual ResultT<std::unique_ptr<babelwires::AudioSource>> getSource(std::string_view sourceName) const = 0;
    };

    /// Find available audio sources and destinations.
    class AudioInterfaceRegistry : public Registry<AudioInterface> {
      public:
        AudioInterfaceRegistry();

        /// The names of all the available audio destinations.
        std::vector<std::string> getDestinationNames() const;

        /// Get an audio destination by name.
        ResultT<std::unique_ptr<babelwires::AudioDest>> getDestination(std::string_view destinationName) const;

        /// Returns the names of all the available audio sources.
        std::vector<std::string> getSourceNames() const;

        /// Get an audio source by name.
        ResultT<std::unique_ptr<babelwires::AudioSource>> getSource(std::string_view sourceName) const;
    };

} // namespace babelwires
