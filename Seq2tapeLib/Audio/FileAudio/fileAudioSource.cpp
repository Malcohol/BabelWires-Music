/**
 * Factories for creating AudioSources corresponding to sound formats.
 *
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/Audio/FileAudio/fileAudioSource.hpp>

#include <BaseLib/Result/error.hpp>

#include <sndfile.h>

#include <assert.h>
#include <stdexcept>

struct babelwires::FileAudioSource::Impl {
    Impl(SF_INFO info, SNDFILE* sndFile)
        : m_info(info)
        , m_sndFile(sndFile) {}



    ~Impl() {
        if (m_sndFile) {
            sf_close(m_sndFile);
        }
    }

    std::string getErrorString() {
        assert(sf_error(m_sndFile) != SF_ERR_NO_ERROR);
        return sf_strerror(m_sndFile);
    }

    SF_INFO m_info;
    SNDFILE* m_sndFile;
};

babelwires::ResultT<babelwires::FileAudioSource> babelwires::FileAudioSource::open(const char* fileName) {
    SF_INFO info;
    info.format = 0;
    SNDFILE* sndFile = sf_open(fileName, SFM_READ, &info);

    if (!sndFile) {
        return babelwires::Error() << "Failed to open file \"" << fileName << "\" for reading: " << sf_strerror(sndFile);
    }

    return FileAudioSource(std::make_unique<Impl>(info, sndFile));
}

babelwires::FileAudioSource::FileAudioSource(std::unique_ptr<Impl> impl)
    : m_impl(std::move(impl)) {}

babelwires::FileAudioSource::~FileAudioSource() {
    // Required out-of-line, because of the unique_ptr.
}

int babelwires::FileAudioSource::getNumChannels() const {
    return m_impl->m_info.channels;
}

babelwires::Duration babelwires::FileAudioSource::getFrequency() const {
    return m_impl->m_info.samplerate;
}

unsigned long babelwires::FileAudioSource::getMoreAudioData(AudioSample* buffer, unsigned long bufSize) {
    assert(sf_error(m_impl->m_sndFile) == SF_ERR_NO_ERROR);
    return sf_read_float(m_impl->m_sndFile, buffer, bufSize);
}
