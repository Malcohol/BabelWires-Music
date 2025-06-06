#include <MusicLib/Functions/splitAtPitchFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>

namespace {
    struct NotesAbove : bw_music::FilteredTrackIterator<> {
        NotesAbove(const bw_music::Track& track, bw_music::Pitch pitch) : bw_music::FilteredTrackIterator<>(track), m_pitch(pitch) {}

        virtual bool isEventOfInterest(const bw_music::TrackEvent& event) override {
            if (const bw_music::NoteEvent* noteEvent = event.as<bw_music::NoteEvent>()) {
                return noteEvent->m_pitch >= m_pitch;
            }
            return false;
        }

        bw_music::Pitch m_pitch;
    };

    struct NotesBelow : bw_music::FilteredTrackIterator<> {
        NotesBelow(const bw_music::Track& track, bw_music::Pitch pitch) : bw_music::FilteredTrackIterator<>(track), m_pitch(pitch) {}

        virtual bool isEventOfInterest(const bw_music::TrackEvent& event) override {
            if (const bw_music::NoteEvent* noteEvent = event.as<bw_music::NoteEvent>()) {
                return noteEvent->m_pitch < m_pitch;
            }
            return false;
        }

        bw_music::Pitch m_pitch;
    };

    struct NotNotes : bw_music::FilteredTrackIterator<> {
        NotNotes(const bw_music::Track& track) : bw_music::FilteredTrackIterator<>(track) {}

        virtual bool isEventOfInterest(const bw_music::TrackEvent& event) override {
            return event.as<bw_music::NoteEvent>() == nullptr;
        }

        bw_music::Pitch m_pitch;
    };
}

bw_music::SplitAtPitchResult bw_music::splitAtPitch(Pitch pitch, const Track& sourceTrack) {
    SplitAtPitchResult result;
    // TODO Do this in one traversal.
    {
        NotesAbove aboveIt(sourceTrack, pitch);
        NotesAbove aboveItEnd(sourceTrack, pitch);
        aboveIt.initBegin();
        aboveItEnd.initEnd();
        for (; aboveIt != aboveItEnd; ++aboveIt) {
            result.m_equalOrAbove.addEvent(*aboveIt);
        }
    }
    {
        NotesBelow belowIt(sourceTrack, pitch);
        NotesBelow belowItEnd(sourceTrack, pitch);
        belowIt.initBegin();
        belowItEnd.initEnd();
        for (; belowIt != belowItEnd; ++belowIt) {
            result.m_below.addEvent(*belowIt);
        }
    }
    {
        NotNotes otherIt(sourceTrack);
        NotNotes otherItEnd(sourceTrack);
        otherIt.initBegin();
        otherItEnd.initEnd();
        for (; otherIt != otherItEnd; ++otherIt) {
            result.m_other.addEvent(*otherIt);
        }
    }

    result.m_equalOrAbove.setDuration(sourceTrack.getDuration());
    result.m_below.setDuration(sourceTrack.getDuration());
    result.m_other.setDuration(sourceTrack.getDuration());

    return result;
}
