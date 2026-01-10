/**
 * MidiTrackAndChannel pairs a track with its MIDI channel.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

#include <MusicLib/Types/Track/trackType.hpp>

namespace {
    static const babelwires::IdentifiersSource s_extraTrackNames = {
        {"exCh0", "Extra Ch. 0", "c1776a3f-120e-4139-9576-341c4de34304"},
        {"exCh1", "Extra Ch. 1", "efd29a2a-d91c-44dc-abb3-5db296c5be43"},
        {"exCh2", "Extra Ch. 2", "10258fb9-fe2c-4d6f-8a53-5e02984af667"},
        {"exCh3", "Extra Ch. 3", "dce5df18-8f82-4dab-adbd-7448a7391115"},
        {"exCh4", "Extra Ch. 4", "f51e27bf-b9e5-452e-9e94-5f1a44813444"},
        {"exCh5", "Extra Ch. 5", "5a436af9-532b-4748-b4b7-7e6dfeb153c1"},
        {"exCh6", "Extra Ch. 6", "ad5d0f48-c794-4857-b58d-6bdbdd2267b2"},
        {"exCh7", "Extra Ch. 7", "d32f2f40-aae1-4c7f-97bf-845cd6393537"},
        {"exCh8", "Extra Ch. 8", "3c794878-13cb-4a5f-adca-a44148498348"},
        {"exCh9", "Extra Ch. 9", "efcccfa4-ba60-49ba-a763-26408d1ba410"},
        {"exCh10", "Extra Ch. 10", "804206d4-9213-490c-a51e-89ca0a74690e"},
        {"exCh11", "Extra Ch. 11", "345559d6-ba67-47f3-95d1-58057969e822"},
        {"exCh12", "Extra Ch. 12", "0882522a-539d-4132-8972-dc6da74c8eb2"},
        {"exCh13", "Extra Ch. 13", "4260ec70-c970-468f-887c-81ef55c2bf09"},
        {"exCh14", "Extra Ch. 14", "ceec1976-1322-4157-9f95-f67bd6cead78"},
        {"exCh15", "Extra Ch. 15", "9e224cdc-e0e1-4439-9aa4-739be9354c7c"}};
} // namespace

babelwires::ShortId smf::MidiTrackAndChannel::getTrackIdFromChannel(unsigned int index) {
    assert(index <= 15);
    return REGISTERED_ID_VECTOR(s_extraTrackNames)[index];
}

smf::MidiTrackAndChannel::MidiTrackAndChannel(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem,
          {{BW_SHORT_ID("Track", "Track", "a6db15c9-9f29-4fb3-92c4-771746b2b97f"),
            bw_music::DefaultTrackType::getThisIdentifier()},
           {BW_SHORT_ID("Chan", "Channel", "cad592ef-2355-4837-a265-38b49eae7599"), MidiChannel::getThisIdentifier()},
           {getTrackIdFromChannel(0), TypeOfTracks::getThisIdentifier(),
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