/**
 * AlsaAudioDest is an AudioDest for ALSA (Advanced Linux Sound System).
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaAudioDest.hpp>

#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/exceptions.hpp>
#include <BaseLib/common.hpp>

#include <alsa/asoundlib.h>

#include <sstream>

struct babelwires_alsa::AlsaAudioDest::Impl {
    Impl(const char* pcmHandleName) {
        {
            const int ret = snd_pcm_open(&m_outStream, pcmHandleName, SND_PCM_STREAM_PLAYBACK, 0);
            if (ret < 0) {
                babelwires_alsa::checkForError("trying to open stream", ret);
            }
        }
        babelwires_alsa::HardwareParameters hwParams;
        {
            const int ret = snd_pcm_hw_params_any(m_outStream, hwParams);
            babelwires_alsa::checkForError("initializing hardware parameter structure", ret);
        }
        {
            const int ret = snd_pcm_hw_params_set_access(m_outStream, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
            babelwires_alsa::checkForError("setting access type", ret);
        }
        {
            const snd_pcm_format_t format = (babelwires::getPlatformEndianness() == babelwires::IS_BIG_ENDIAN)
                                                ? SND_PCM_FORMAT_FLOAT_BE
                                                : SND_PCM_FORMAT_FLOAT_LE;
            const int ret = snd_pcm_hw_params_set_format(m_outStream, hwParams, format);
            babelwires_alsa::checkForError("setting format", ret);
        }
        {
            // TODO find minimum.
            unsigned int rate = 44100;
            int dir;
            const int ret = snd_pcm_hw_params_set_rate_near(m_outStream, hwParams, &rate, &dir);
            babelwires_alsa::checkForError("setting rate", ret);
        }
        {
            const int ret = snd_pcm_hw_params(m_outStream, hwParams);
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
        snd_pcm_drain(m_outStream);
        snd_pcm_close(m_outStream);
    }

    snd_pcm_t* m_outStream;
    babelwires::Rate m_rate;
    unsigned int m_numChannels;
};

babelwires_alsa::AlsaAudioDest::AlsaAudioDest(const char* pcmHandleName)
    : m_impl(new Impl(pcmHandleName)) {}

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
