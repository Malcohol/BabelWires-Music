/**
 * Factories for creating AudioDests corresponding to sound formats.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/Audio/FileAudio/fileAudioDest.hpp>

#include <BaseLib/Identifiers/registeredIdentifier.hpp>
#include <BaseLib/exceptions.hpp>

#include <sndfile.h>

#include <assert.h>
#include <stdexcept>

namespace {

    struct SndFileAudioDest : babelwires::AudioDest {
        SndFileAudioDest(SF_INFO info, SNDFILE* sndFile)
            : m_info(info)
            , m_sndFile(sndFile) {}

        ~SndFileAudioDest() {
            if (m_sndFile) {
                sf_close(m_sndFile);
            }
        }

        int getNumChannels() const { return m_info.channels; }

        babelwires::Duration getFrequency() const { return m_info.samplerate; }

        unsigned long writeMoreAudioData(const babelwires::AudioSample* buffer, unsigned long bufSize) {
            assert(sf_error(m_sndFile) == SF_ERR_NO_ERROR);
            return sf_write_float(m_sndFile, buffer, bufSize);
        }

        std::string getErrorString() {
            assert(sf_error(m_sndFile) != SF_ERR_NO_ERROR);
            return sf_strerror(m_sndFile);
        }

        SF_INFO m_info;
        SNDFILE* m_sndFile;
    };

    struct SndFileAudioDestFactory : babelwires::FileAudioDestFactory {
        SndFileAudioDestFactory(babelwires::LongId id, const char* ext, std::uint32_t code)
            : FileAudioDestFactory(id, 1, Extensions{ext})
            , m_formatCode(code) {}

        babelwires::ResultT<std::unique_ptr<babelwires::AudioDest>>
        createFileAudioDest(const char* fileName, unsigned int numChannels) const override {
            assert(numChannels > 0);
            assert(numChannels <= 2);

            SF_INFO info;
            info.channels = numChannels;
            info.samplerate = 44100;
            info.format = m_formatCode;
            SNDFILE* sndFile = sf_open(fileName, SFM_WRITE, &info);

            if (!sndFile) {
                return babelwires::Error()
                       << "Failed to open file \"" << fileName << "\" for writing: " << sf_strerror(sndFile);
            }

            return std::make_unique<SndFileAudioDest>(info, sndFile);
        }

        std::uint32_t m_formatCode;
    };

} // namespace

babelwires::FileAudioDestFactory::FileAudioDestFactory(LongId identifier, VersionNumber version, Extensions extensions)
    : FileTypeEntry(identifier, version, std::move(extensions)) {}

babelwires::FileAudioDestRegistry::FileAudioDestRegistry()
    : FileTypeRegistry<FileAudioDestFactory>("File Audio Dest Registry") {
    addEntry(
        std::make_unique<SndFileAudioDestFactory>(BW_LONG_ID("WAV", "WAV file", "c2b2f468-2826-4ba9-bc6b-706833c0ed69"),
                                                  "wav", (SF_FORMAT_WAV | SF_FORMAT_PCM_16)));
    addEntry(std::make_unique<SndFileAudioDestFactory>(
        BW_LONG_ID("AIFF", "Aiff file", "b3bdca68-28ad-449c-a7a0-362686f7a5fc"), "aiff",
        (SF_FORMAT_AIFF | SF_FORMAT_PCM_16)));
    addEntry(std::make_unique<SndFileAudioDestFactory>(
        BW_LONG_ID("FLAC", "FLAC file", "fe9b3901-e63d-4719-a695-8a08473cf061"), "flac", SF_FORMAT_FLAC));
    addEntry(std::make_unique<SndFileAudioDestFactory>(
        BW_LONG_ID("OGG", "OGG file", "479f13ab-4830-49f0-9559-9a96233616ff"), "ogg", SF_FORMAT_OGG));
}

babelwires::ResultT<std::unique_ptr<babelwires::AudioDest>>
babelwires::FileAudioDestRegistry::createFileAudioDest(const char* fileName, unsigned int numChannels) const {
    if (const auto* factory = getEntryByFileName(fileName)) {
        return factory->createFileAudioDest(fileName, numChannels);
    } else {
        return Error() << "The file name \"" << fileName << "\" is not in a recognized audio destination format";
    }
}
