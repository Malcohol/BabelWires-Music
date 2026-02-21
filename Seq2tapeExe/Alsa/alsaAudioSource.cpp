/**
 * AlsaAudioSource is an AudioSource for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaAudioSource.hpp>

#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/common.hpp>

#include <alsa/asoundlib.h>

#include <utility>

struct babelwires_alsa::AlsaAudioSource::Impl {
    Impl(snd_pcm_t* inStream, babelwires::Rate rate, unsigned int numChannels)
        : m_inStream(inStream)
        , m_rate(rate)
        , m_numChannels(numChannels) {}

    ~Impl() {
        if (m_inStream != nullptr) {
            snd_pcm_drain(m_inStream);
            snd_pcm_close(m_inStream);
        }
    }

    snd_pcm_t* m_inStream;
    babelwires::Rate m_rate;
    unsigned int m_numChannels;
};

babelwires_alsa::AlsaAudioSource::AlsaAudioSource(std::unique_ptr<Impl> impl)
    : m_impl(std::move(impl)) {}

babelwires::ResultT<babelwires_alsa::AlsaAudioSource>
babelwires_alsa::AlsaAudioSource::open(const char* pcmHandleName) {
    snd_pcm_t* inStream = nullptr;
    ON_ERROR(if (inStream != nullptr) { snd_pcm_close(inStream); });

    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_open(&inStream, pcmHandleName, SND_PCM_STREAM_CAPTURE, 0),
                                               "trying to open stream"));

    ASSIGN_OR_ERROR(auto hwParams, babelwires_alsa::createHardwareParameters());

    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params_any(inStream, hwParams),
                                               "initializing hardware parameter structure"));
    DO_OR_ERROR(babelwires_alsa::checkForError(
        snd_pcm_hw_params_set_access(inStream, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED), "setting access type"));

    const snd_pcm_format_t format = (babelwires::getPlatformEndianness() == babelwires::IS_BIG_ENDIAN)
                                        ? SND_PCM_FORMAT_FLOAT_BE
                                        : SND_PCM_FORMAT_FLOAT_LE;
    DO_OR_ERROR(
        babelwires_alsa::checkForError(snd_pcm_hw_params_set_format(inStream, hwParams, format), "setting format"));

    unsigned int rate = 44100;
    int dir = 0;
    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params_set_rate_near(inStream, hwParams, &rate, &dir),
                                               "setting rate"));
    DO_OR_ERROR(babelwires_alsa::checkForError(snd_pcm_hw_params(inStream, hwParams), "setting hardware parameters"));

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

    return AlsaAudioSource(std::make_unique<Impl>(inStream, resolvedRate, numChannels));
}

babelwires_alsa::AlsaAudioSource::~AlsaAudioSource() {
    // Required out-of-line, because of the unique_ptr.
}

int babelwires_alsa::AlsaAudioSource::getNumChannels() const {
    return m_impl->m_numChannels;
}

babelwires::Duration babelwires_alsa::AlsaAudioSource::getFrequency() const {
    return m_impl->m_rate;
}

unsigned long babelwires_alsa::AlsaAudioSource::getMoreAudioData(babelwires::AudioSample* buffer,
                                                                 unsigned long bufSize) {
    unsigned int numFramesToRead = bufSize / m_impl->m_numChannels;
    snd_pcm_sframes_t numFramesWritten = snd_pcm_readi(m_impl->m_inStream, buffer, numFramesToRead);
    return numFramesWritten * m_impl->m_numChannels;
}
