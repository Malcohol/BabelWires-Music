/**
 * AlsaAudioInterface is an AudioInterface for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaAudioInterface.hpp>

#include <Seq2tapeExe/Alsa/alsaAudioDest.hpp>
#include <Seq2tapeExe/Alsa/alsaAudioSource.hpp>
#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/Identifiers/registeredIdentifier.hpp>
#include <BaseLib/Log/debugLogger.hpp>

#include <alsa/asoundlib.h>

babelwires_alsa::AlsaAudioInterface::AlsaAudioInterface()
    : babelwires::AudioInterface(BW_LONG_ID("Alsa", "Alsa", "5c7e28b1-726d-4564-82dd-33a168ffab5a"), 1) {}

babelwires::LongId babelwires_alsa::AlsaAudioInterface::getThisIdentifier() {
    return "Alsa";
}

namespace {
    struct AlsaNameQuery {
        AlsaNameQuery() {
            const int ret = snd_device_name_hint(-1, "pcm", &m_hints);
            const auto result = babelwires_alsa::checkForError(ret, "Trying to query device names");
            if (!result) {
                logDebug() << result.error().toString();
                m_hints = nullptr;
            }
        }

        enum class Direction { Destination, Source };

        std::vector<std::string> getNames(Direction d) const {
            std::vector<std::string> names;
            if (m_hints == nullptr) {
                return names;
            }
            void** hintptr = m_hints;
            while (*hintptr != nullptr) {
                const char* name = snd_device_name_get_hint(*hintptr, "NAME");
                const char* ioid = snd_device_name_get_hint(*hintptr, "IOID");
                if (!ioid // Input and output
                    || ((strcmp(ioid, "Output") == 0) && (d == Direction::Destination)) ||
                    ((strcmp(ioid, "Input") == 0) && (d == Direction::Source))) {
                    names.emplace_back(name);
                }
                ++hintptr;
            }
            return names;
        }

        ~AlsaNameQuery() {
            if (m_hints != nullptr) {
                snd_device_name_free_hint(m_hints);
            }
        }

        void** m_hints = nullptr;
    };
} // namespace

std::vector<std::string> babelwires_alsa::AlsaAudioInterface::getDestinationNames() const {
    AlsaNameQuery nameQueryObject;
    return nameQueryObject.getNames(AlsaNameQuery::Direction::Destination);
}

babelwires::ResultT<std::unique_ptr<babelwires::AudioDest>>
babelwires_alsa::AlsaAudioInterface::getDestination(std::string_view destinationName) const {
    std::string destName(destinationName);
    ASSIGN_OR_ERROR(auto alsaAudioDest, AlsaAudioDest::open(destName.c_str()));
    return std::make_unique<AlsaAudioDest>(std::move(alsaAudioDest));
}

std::vector<std::string> babelwires_alsa::AlsaAudioInterface::getSourceNames() const {
    AlsaNameQuery nameQueryObject;
    return nameQueryObject.getNames(AlsaNameQuery::Direction::Source);
}

babelwires::ResultT<std::unique_ptr<babelwires::AudioSource>>
babelwires_alsa::AlsaAudioInterface::getSource(std::string_view sourceName) const {
    std::string srcName(sourceName);
    ASSIGN_OR_ERROR(auto alsaAudioSource, AlsaAudioSource::open(srcName.c_str()));
    return std::make_unique<AlsaAudioSource>(std::move(alsaAudioSource));
}
