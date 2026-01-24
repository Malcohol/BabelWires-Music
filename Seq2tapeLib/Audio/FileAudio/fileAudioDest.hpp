/**
 * Factories for creating AudioDests corresponding to sound formats.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Seq2tapeLib/Audio/audioDest.hpp>
#include <BaseLib/Registry/fileTypeRegistry.hpp>

#include <cctype>
#include <memory>
#include <string>

namespace babelwires {

    /// A file extension does not give sufficient information for a generic sound writing library to choose
    /// its format, so we associate some reasonable settings with each file extension using factories.
    class FileAudioDestFactory : public FileTypeEntry {
      public:
        FileAudioDestFactory(LongId identifier, VersionNumber version, Extensions extensions);

        virtual std::unique_ptr<AudioDest> createFileAudioDest(const char* fileName,
                                                               unsigned int numChannels) const = 0;
    };

    /// A registry of factories for creating fileAudioDests, which is initialized with a few useful formats.
    class FileAudioDestRegistry : public FileTypeRegistry<FileAudioDestFactory> {
      public:
        /// Pre-populates the registry with a set of useful supported formats (wav, aiff, flac, ogg).
        FileAudioDestRegistry();

        // Convenience method which finds the factory based on the file name, and uses it to create the file.
        std::unique_ptr<AudioDest> createFileAudioDest(const char* fileName, unsigned int numChannels) const;
    };

} // namespace babelwires
