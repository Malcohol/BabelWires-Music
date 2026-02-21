/**
 * Some common functionality used for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeExe/Alsa/alsaCommon.hpp>

#include <BaseLib/Result/resultDSL.hpp>

babelwires_alsa::HardwareParameters::HardwareParameters(snd_pcm_hw_params_t* params)
    : m_params(params) {}

babelwires_alsa::HardwareParameters::HardwareParameters(HardwareParameters&& other) noexcept
    : m_params(other.m_params) {
    other.m_params = nullptr;
}

babelwires_alsa::HardwareParameters&
babelwires_alsa::HardwareParameters::operator=(HardwareParameters&& other) noexcept {
    if (this != &other) {
        if (m_params != nullptr) {
            snd_pcm_hw_params_free(m_params);
        }
        m_params = other.m_params;
        other.m_params = nullptr;
    }
    return *this;
}

babelwires_alsa::HardwareParameters::~HardwareParameters() {
    if (m_params != nullptr) {
        snd_pcm_hw_params_free(m_params);
    }
}

babelwires_alsa::HardwareParameters::operator snd_pcm_hw_params_t*() {
    return m_params;
}

babelwires::ResultT<babelwires_alsa::HardwareParameters> babelwires_alsa::createHardwareParameters() {
    snd_pcm_hw_params_t* params = nullptr;
    DO_OR_ERROR(checkForError(snd_pcm_hw_params_malloc(&params), "allocating hardware parameters"));
    return HardwareParameters(params);
}

babelwires::Result babelwires_alsa::checkForError(int retCode, const char* description) {
    if (retCode < 0) {
        return babelwires::Error() << "Alsa error " << description << " (" << snd_strerror(retCode) << ")";
    }
    return {};
}
