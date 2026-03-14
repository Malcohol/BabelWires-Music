/**
 * AlsaAudioDest is an AudioDest for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaAudioDest.hpp>

#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/common.hpp>
#include <BaseLib/Result/resultDSL.hpp>

#include <alsa/asoundlib.h>

#include <utility>

struct babelwires_alsa::AlsaAudioDest::Impl {
    Impl(snd_pcm_t* outStream, babelwires::Rate rate, unsigned int numChannels)
        : m_outStream(outStream)
        , m_rate(rate)
        , m_numChannels(numChannels) {}

    ~Impl() {
        if (m_outStream != nullptr) {
            snd_pcm_drain(m_outStream);
            snd_pcm_close(m_outStream);
        }
    }

    snd_pcm_t* m_outStream;
    babelwires::Rate m_rate;
    unsigned int m_numChannels;
};

babelwires_alsa::AlsaAudioDest::AlsaAudioDest(std::unique_ptr<Impl> impl)
    : m_impl(std::move(impl)) {}

babelwires_alsa::AlsaAudioDest::AlsaAudioDest(AlsaAudioDest&&) = default;

babelwires_alsa::AlsaAudioDest& babelwires_alsa::AlsaAudioDest::operator=(AlsaAudioDest&&) = default;

babelwires::ResultT<babelwires_alsa::AlsaAudioDest> babelwires_alsa::AlsaAudioDest::open(const char* pcmHandleName) {
    snd_pcm_t* outStream = nullptr;
    ON_ERROR(if (outStream != nullptr) { snd_pcm_close(outStream); });

    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_open(&outStream, pcmHandleName, SND_PCM_STREAM_PLAYBACK, 0),
                                               "trying to open stream"));

    ASSIGN_OR_ERROR(auto hwParams, babelwires_alsa::createHardwareParameters());

    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params_any(outStream, hwParams),
                                               "initializing hardware parameter structure"));
    DO_OR_ERROR(babelwires_alsa::checkForError(
        snd_pcm_hw_params_set_access(outStream, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED), "setting access type"));

    const snd_pcm_format_t format = (babelwires::getPlatformEndianness() == babelwires::IS_BIG_ENDIAN)
                                        ? SND_PCM_FORMAT_FLOAT_BE
                                        : SND_PCM_FORMAT_FLOAT_LE;
    DO_OR_ERROR(
        babelwires_alsa::checkForError(snd_pcm_hw_params_set_format(outStream, hwParams, format), "setting format"));

    unsigned int rate = 44100;
    int dir = 0;
    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params_set_rate_near(outStream, hwParams, &rate, &dir),
                                               "setting rate"));
    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params(outStream, hwParams), "setting hardware parameters"));

    babelwires::Rate resolvedRate = 0.0;
    {
        rate = 0;
        dir = 0;
        DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params_get_rate(hwParams, &rate, &dir),
                                                   "extracting rate from hardware parameters"));
        resolvedRate = rate + (0.25 * dir);
    }

    unsigned int numChannels = 0;
    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params_get_channels(hwParams, &numChannels),
                                               "extracting number of channels from hardware parameters"));

    return AlsaAudioDest(std::make_unique<Impl>(outStream, resolvedRate, numChannels));
}

babelwires_alsa::AlsaAudioDest::~AlsaAudioDest() {
    // Required out-of-line, because of the unique_ptr.
}

int babelwires_alsa::AlsaAudioDest::getNumChannels() const {
    return m_impl->m_numChannels;
}

babelwires::Duration babelwires_alsa::AlsaAudioDest::getFrequency() const {
    return m_impl->m_rate;
}

unsigned long babelwires_alsa::AlsaAudioDest::writeMoreAudioData(const babelwires::AudioSample* buffer,
                                                                 unsigned long bufSize) {
    unsigned int numFramesToWrite = bufSize / m_impl->m_numChannels;
    snd_pcm_sframes_t numFramesWritten = snd_pcm_writei(m_impl->m_outStream, buffer, numFramesToWrite);
    return numFramesWritten * m_impl->m_numChannels;
}
