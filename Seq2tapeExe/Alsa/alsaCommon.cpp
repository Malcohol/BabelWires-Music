/**
 * Some common functionality used for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/exceptions.hpp>

babelwires_alsa::HardwareParameters::HardwareParameters() {
    const int ret = snd_pcm_hw_params_malloc(&m_params);
    if (ret < 0) {
        throw babelwires::IoException() << "Cannot allocate hardware parameters";
    }
}

babelwires_alsa::HardwareParameters::~HardwareParameters() {
    snd_pcm_hw_params_free(m_params);
}

babelwires_alsa::HardwareParameters::operator snd_pcm_hw_params_t*() {
    return m_params;
}

void babelwires_alsa::checkForError(const char* description, int retCode) {
    if (retCode < 0) {
        throw babelwires::IoException() << "Alsa error " << description << " (" << snd_strerror(retCode) << ")";
    }
}
