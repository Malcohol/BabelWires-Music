/**
 * AlsaAudioSource is an AudioSource for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaAudioSource.hpp>

#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/exceptions.hpp>

#include <alsa/asoundlib.h>

#include <sstream>

struct babelwires_alsa::AlsaAudioSource::Impl {
    Impl(const char* pcmHandleName) {
        {
            const int ret = snd_pcm_open(&m_inStream, pcmHandleName, SND_PCM_STREAM_CAPTURE, 0);
            if (ret < 0) {
                babelwires_alsa::checkForError("trying to open stream", ret);
            }
        }
        babelwires_alsa::HardwareParameters hwParams;
        {
            const int ret = snd_pcm_hw_params_any(m_inStream, hwParams);
            babelwires_alsa::checkForError("initializing hardware parameter structure", ret);
        }
        {
            const int ret = snd_pcm_hw_params_set_access(m_inStream, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
            babelwires_alsa::checkForError("setting access type", ret);
        }
        {
            const snd_pcm_format_t format = (babelwires::getPlatformEndianness() == babelwires::IS_BIG_ENDIAN)
                                                ? SND_PCM_FORMAT_FLOAT_BE
                                                : SND_PCM_FORMAT_FLOAT_LE;
            const int ret = snd_pcm_hw_params_set_format(m_inStream, hwParams, format);
            babelwires_alsa::checkForError("setting format", ret);
        }
        {
            // TODO find minimum.
            unsigned int rate = 44100;
            int dir;
            const int ret = snd_pcm_hw_params_set_rate_near(m_inStream, hwParams, &rate, &dir);
            babelwires_alsa::checkForError("setting rate", ret);
        }
        {
            const int ret = snd_pcm_hw_params(m_inStream, hwParams);
            babelwires_alsa::checkForError("setting hardware parameters", ret);
        }
        {
            unsigned int rate;
            int dir;
            const int ret = snd_pcm_hw_params_get_rate(hwParams, &rate, &dir);
            babelwires_alsa::checkForError("extracting rate from hardware parameters", ret);
            m_rate = rate + (0.25 * dir);
        }
        {
            const int ret = snd_pcm_hw_params_get_channels(hwParams, &m_numChannels);
            babelwires_alsa::checkForError("extracting number of channels from hardware parameters", ret);
        }
    }

    ~Impl() {
        snd_pcm_drain(m_inStream);
        snd_pcm_close(m_inStream);
    }

    snd_pcm_t* m_inStream;
    babelwires::Rate m_rate;
    unsigned int m_numChannels;
};

babelwires_alsa::AlsaAudioSource::AlsaAudioSource(const char* pcmHandleName)
    : m_impl(new Impl(pcmHandleName)) {}

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
