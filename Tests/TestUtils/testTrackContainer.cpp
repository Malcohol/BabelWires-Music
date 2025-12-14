#include <Domains/Music/Tests/TestUtils/testTrackContainer.hpp>


testUtils::TestTrackContainer::TestTrackContainer()
    : babelwires::RecordType({{BW_SHORT_ID("track1", "Track 1", "747a4575-7e38-4408-8499-8427064dda4a"),
                               bw_music::DefaultTrackType::getThisType()},
                              {BW_SHORT_ID("track2", "Track 2", "753b5995-5172-4e32-af78-5acc0af978b1"),
                               bw_music::DefaultTrackType::getThisType()},
                              {BW_SHORT_ID("other", "Not a track", "052b89f7-1e0f-4c18-82d4-4c4536d44245"),
                               babelwires::DefaultIntType::getThisType()}}) {}

