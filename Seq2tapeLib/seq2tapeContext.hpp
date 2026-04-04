/**
 * The context into which plugins can register formats, etc.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/Seq2tapeLib/seq2tapeLibExport.hpp>

namespace seq2tape {
    class TapeFileFormatRegistry;

    /// The context into which plugins can register formats, etc.
    struct SEQ2TAPELIB_API Seq2TapeContext {
        seq2tape::TapeFileFormatRegistry& m_tapeFileRegistry;
    };
}
