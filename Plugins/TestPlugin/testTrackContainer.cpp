#include <Domains/Music/Plugins/TestPlugin/testTrackContainer.hpp>

bw_music_testplugin::TestTrackContainer::TestTrackContainer(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisType(), typeSystem, {{getTrack1Id(), bw_music::DefaultTrackType::getThisType()},
                              {getTrack2Id(), bw_music::DefaultTrackType::getThisType()},
                              {getOtherId(), babelwires::DefaultIntType::getThisType()}}) {}

babelwires::ShortId bw_music_testplugin::TestTrackContainer::getTrack1Id() {
    return BW_SHORT_ID("track1", "Track 1", "3c4d5e6f-7a8b-9c0d-1e2f-3a4b5c6d7e8f");
}

babelwires::ShortId bw_music_testplugin::TestTrackContainer::getTrack2Id() {
    return BW_SHORT_ID("track2", "Track 2", "4d5e6f7a-8b9c-0d1e-2f3a-4b5c6d7e8f9a");
}

babelwires::ShortId bw_music_testplugin::TestTrackContainer::getOtherId() {
    return BW_SHORT_ID("other", "Not a track", "052b89f7-1e0f-4c18-82d4-4c4536d44245");
}