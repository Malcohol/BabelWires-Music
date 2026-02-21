/**
 * Some common functionality used for ALSA (Advanced Linux Sound System).
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <BaseLib/Utilities/result.hpp>

#include <alsa/asoundlib.h>

namespace babelwires_alsa {

    // Safe version of a snd_pcm_hw_params_t*.
    struct HardwareParameters {
        HardwareParameters() = default;
        explicit HardwareParameters(snd_pcm_hw_params_t* params);

        HardwareParameters(HardwareParameters&& other) noexcept;
        HardwareParameters& operator=(HardwareParameters&& other) noexcept;

        HardwareParameters(const HardwareParameters&) = delete;
        HardwareParameters& operator=(const HardwareParameters&) = delete;

        ~HardwareParameters();

        operator snd_pcm_hw_params_t*();

        snd_pcm_hw_params_t* m_params = nullptr;
    };

    babelwires::ResultT<HardwareParameters> createHardwareParameters();

    Result checkForError(int retCode, const char* description);
} // namespace babelwires_alsa
