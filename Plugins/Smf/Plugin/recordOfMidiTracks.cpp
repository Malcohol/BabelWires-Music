/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/recordOfMidiTracks.hpp>

namespace {
    static const babelwires::IdentifiersSource s_trackNames = {
        {"ch0", "Ch. 0", "18cb72e5-6891-428c-beae-aa17db453852"},
        {"ch1", "Ch. 1", "f4bc5369-5d7e-4e18-83ec-44560309edb7"},
        {"ch2", "Ch. 2", "8e58bcc7-af0b-40d0-8a13-ca2c35db8c5e"},
        {"ch3", "Ch. 3", "cddd1035-ca20-4094-ae29-7947d9e71b4f"},
        {"ch4", "Ch. 4", "f4d6c484-028f-4d17-bcb0-a2ddf21904d9"},
        {"ch5", "Ch. 5", "9ba09b7c-2301-4846-9ac0-ccd3708abb7b"},
        {"ch6", "Ch. 6", "2382cc8d-7920-40e8-8813-35b75f40befb"},
        {"ch7", "Ch. 7", "18d1584d-67e4-4f50-ad5a-9d887842414f"},
        {"ch8", "Ch. 8", "6925461b-02b3-448c-a7a9-4cd7f1d5c444"},
        {"ch9", "Ch. 9", "95cfcb8f-af02-4d24-95b6-5b4c19cb3c1b"},
        {"ch10", "Ch. 10", "a445a439-2335-445e-b1f9-ae0a6c23cbd7"},
        {"ch11", "Ch. 11", "b1b96d5b-4c13-4c5d-b876-a7752cdf4694"},
        {"ch12", "Ch. 12", "0df2d856-80d7-47ee-88ba-508bdffec70c"},
        {"ch13", "Ch. 13", "3b9e4821-d6eb-45da-9118-419e190dc763"},
        {"ch14", "Ch. 14", "2742e897-3182-48bf-9558-21e6184cddec"},
        {"ch15", "Ch. 15", "46fb6f3a-15bf-4c3a-972a-78e7ba2ca5b9"}};
} // namespace

smf::RecordOfMidiTracks::RecordOfMidiTracks(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{getTrackIdFromChannel(0), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(1), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(2), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(3), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(4), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(5), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(6), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(7), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(8), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(9), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(10), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(11), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(12), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(13), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(14), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive},
                              {getTrackIdFromChannel(15), TypeOfTracks::getThisIdentifier(),
                               babelwires::RecordType::Optionality::optionalDefaultInactive}}) {}

babelwires::ShortId smf::RecordOfMidiTracks::getTrackIdFromChannel(unsigned int index) {
    assert(index <= 15);
    return REGISTERED_ID_VECTOR(s_trackNames)[index];
}
