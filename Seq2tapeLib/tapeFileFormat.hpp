/**
 * A TapeFileFormat knows how to convert certain types of data between a TapeFile::DataFile and audio.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/Registry/fileTypeRegistry.hpp>
#include <BaseLib/productInfo.hpp>
#include <Seq2tapeLib/tapeFile.hpp>

#include <memory>
#include <vector>

namespace babelwires {
    struct AudioSource;
    struct AudioDest;
} // namespace babelwires

namespace seq2tape {

    class TapeFile;

    /// A registered format which knows how to convert between a TapeFile::DataFile and audio.
    struct TapeFileFormat : public babelwires::FileTypeEntry, babelwires::ProductInfo {
        TapeFileFormat(babelwires::LongId identifier, babelwires::VersionNumber version,
                       Extensions extensions);

        virtual std::unique_ptr<TapeFile::DataFile> loadFromAudio(babelwires::AudioSource& source) const = 0;

        virtual void writeToAudio(const TapeFile::DataFile& tapefile, babelwires::AudioDest& dest) const = 0;
    };

    struct TapeFileFormatRegistry : babelwires::FileTypeRegistry<TapeFileFormat> {
        TapeFileFormatRegistry();
    };

} // namespace seq2tape
