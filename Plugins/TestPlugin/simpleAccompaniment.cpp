#include <Domains/Music/Plugins/TestPlugin/simpleAccompaniment.hpp>

#include <MusicLib/Functions/fitToChordFunction.hpp>
#include <MusicLib/Functions/transposeFunction.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/chord.hpp>

#include <BabelWiresLib/Types/Record/recordValue.hpp>

bw_music_testplugin::SimpleAccompaniment::SimpleAccompaniment(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{bw_music::ChordType::getIdentifierFromValue(bw_music::ChordType::Value::M),
                               TestTrackContainer::getThisIdentifier()},
                              {bw_music::ChordType::getIdentifierFromValue(bw_music::ChordType::Value::m),
                               TestTrackContainer::getThisIdentifier()},
                              {bw_music::ChordType::getIdentifierFromValue(bw_music::ChordType::Value::dim),
                               TestTrackContainer::getThisIdentifier()}}) {}

bw_music::Track bw_music_testplugin::SimpleAccompaniment::getCMajorArpeggioTrack() {
    bw_music::TrackBuilder trackBuilder;
    const bw_music::ModelDuration duration(1, 4);
    trackBuilder.addEvent(bw_music::NoteOnEvent(0, 60)); // C4
    trackBuilder.addEvent(bw_music::NoteOffEvent(duration, 60));
    trackBuilder.addEvent(bw_music::NoteOnEvent(0, 64)); // E4
    trackBuilder.addEvent(bw_music::NoteOffEvent(duration, 64));
    trackBuilder.addEvent(bw_music::NoteOnEvent(0, 67)); // G4
    trackBuilder.addEvent(bw_music::NoteOffEvent(duration, 67));
    trackBuilder.addEvent(bw_music::NoteOnEvent(0, 72)); // C5
    trackBuilder.addEvent(bw_music::NoteOffEvent(duration, 72));
    bw_music::Track result = trackBuilder.finishAndGetTrack();
    assert(result.getNumEvents() == getNumEventsInTrack());
    assert(result.getDuration() == getTrackDuration());
    return result;
}

unsigned int bw_music_testplugin::SimpleAccompaniment::getNumEventsInTrack() {
    return 8;
}

bw_music::ModelDuration bw_music_testplugin::SimpleAccompaniment::getTrackDuration() {
    return 1;
}

babelwires::NewValueHolder
bw_music_testplugin::SimpleAccompaniment::createValue(const babelwires::TypeSystem& typeSystem) const {
    auto newValue = RecordType::createValue(typeSystem);
    const auto& testTrackContainer = typeSystem.getEntryByType<TestTrackContainer>();

    auto& recordValue = newValue.m_nonConstReference.is<babelwires::RecordValue>();
    {
        auto& cmajor = recordValue.getValue(bw_music::ChordType::getIdentifierFromValue(bw_music::ChordType::Value::M));
        bw_music::Track notes = getCMajorArpeggioTrack();
        {
            auto [track1, _] = testTrackContainer->getChildByIdNonConst(cmajor, TestTrackContainer::getTrack1Id());
            track1.copyContentsAndGetNonConst().is<bw_music::Track>() = notes;
        }
        {
            auto [track2, _] = testTrackContainer->getChildByIdNonConst(cmajor, TestTrackContainer::getTrack2Id());
            track2.copyContentsAndGetNonConst().is<bw_music::Track>() = transposeTrack(notes, 12); // One octave up
        }
        {
            auto [other, _] = testTrackContainer->getChildByIdNonConst(cmajor, TestTrackContainer::getOtherId());
            other.copyContentsAndGetNonConst().is<babelwires::IntValue>() = 1;
        }
    }
    {
        auto& cminor = recordValue.getValue(bw_music::ChordType::getIdentifierFromValue(bw_music::ChordType::Value::m));
        bw_music::Track notes = getCMajorArpeggioTrack();
        notes =
            fitToChordFunction(notes, bw_music::Chord(bw_music::PitchClass::Value::C, bw_music::ChordType::Value::m));
        {
            auto [track1, _] = testTrackContainer->getChildByIdNonConst(cminor, TestTrackContainer::getTrack1Id());
            track1.copyContentsAndGetNonConst().is<bw_music::Track>() = notes;
        }
        {
            auto [track2, _] = testTrackContainer->getChildByIdNonConst(cminor, TestTrackContainer::getTrack2Id());
            track2.copyContentsAndGetNonConst().is<bw_music::Track>() =
                transposeTrack(notes, 12); // One octave up, C minor
        }
        {
            auto [other, _] = testTrackContainer->getChildByIdNonConst(cminor, TestTrackContainer::getOtherId());
            other.copyContentsAndGetNonConst().is<babelwires::IntValue>() = 2;
        }
    }
    {
        auto& diminished =
            recordValue.getValue(bw_music::ChordType::getIdentifierFromValue(bw_music::ChordType::Value::dim));
        bw_music::Track notes = getCMajorArpeggioTrack();
        notes =
            fitToChordFunction(notes, bw_music::Chord(bw_music::PitchClass::Value::C, bw_music::ChordType::Value::dim));
        {
            auto [track1, _] = testTrackContainer->getChildByIdNonConst(diminished, TestTrackContainer::getTrack1Id());
            track1.copyContentsAndGetNonConst().is<bw_music::Track>() = notes;
        }
        {
            auto [track2, _] = testTrackContainer->getChildByIdNonConst(diminished, TestTrackContainer::getTrack2Id());
            track2.copyContentsAndGetNonConst().is<bw_music::Track>() = transposeTrack(notes, 12); // One octave up
        }
        {
            auto [other, _] = testTrackContainer->getChildByIdNonConst(diminished, TestTrackContainer::getOtherId());
            other.copyContentsAndGetNonConst().is<babelwires::IntValue>() = 3;
        }
    }

    return newValue;
}
