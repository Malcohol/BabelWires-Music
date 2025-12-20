/**
 * A processor which sequences accompaniment tracks according to a chord track.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Generic/genericType.hpp>

namespace bw_music {
    /// Input type: record with AccompTracks (record) and ChordTrack (track)
    class AccompanimentSequencerProcessorInput : public babelwires::GenericType {
      public:
        REGISTERED_TYPE("AccompSeqIn", "Accompaniment Sequencer Input",
                        "c5f8e3b7-8d4a-4b2e-9c1f-7a6d5e2b8a0f", 1);

        AccompanimentSequencerProcessorInput();

        static babelwires::ShortId getAccompTracksId() {
            return BW_SHORT_ID("Accomp", "Accompaniment Tracks",
                             "d8f1a7c4-6e2b-4f9a-8c3d-7b5e1a9f2d6c");
        }

        static babelwires::ShortId getChordTrackId() {
            return BW_SHORT_ID("Chords", "Chord Track",
                             "e9f2b8d5-7f3c-5a0b-9d4e-8c6f2b0a3e7d");
        }
    };

    class AccompanimentSequencerProcessorOutput : public babelwires::GenericType {
      public:
        REGISTERED_TYPE("AccompSeqOut", "Accompaniment Sequencer Output",
                        "f7e3a9c2-1b5f-4c3d-8e2a-6f4d7a1c5b9e", 1);

        AccompanimentSequencerProcessorOutput();

        static babelwires::ShortId getResultId() {
            return BW_SHORT_ID("Result", "Result track",
                             "a1d3e5f7-2b4c-6d8f-0a1a-3c5e7f9b1d3a");
        }
    };

    class AccompanimentSequencerProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("AccompSeq", "Accompaniment Sequencer",
                                          "a8d2f6e1-4c9b-4f2e-7d3a-5e1b8c6f4a2d");

        AccompanimentSequencerProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };

} // namespace bw_music
