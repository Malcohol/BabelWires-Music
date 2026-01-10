/**
 * A PercussionSet corresponding to the instruments of XG SFX 2 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/xgSFX2PercussionSet.hpp>

smf::XgSFX2PercussionSet::XgSFX2PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(
          getThisType(), std::vector<InstrumentBlock>{
              {{bw_music::BuiltInPercussionInstruments::Value::DialTn,
                bw_music::BuiltInPercussionInstruments::Value::DoorCr,
                bw_music::BuiltInPercussionInstruments::Value::Door,   // Door Slam
                bw_music::BuiltInPercussionInstruments::Value::ScrPsh, // Scratch
                bw_music::BuiltInPercussionInstruments::Value::ScrPll, // Scratch 2
                bw_music::BuiltInPercussionInstruments::Value::WChime,
                bw_music::BuiltInPercussionInstruments::Value::TeleRn},
               36,
               &builtInInstruments},
              {{bw_music::BuiltInPercussionInstruments::Value::CarEng, // Engine Start
                bw_music::BuiltInPercussionInstruments::Value::TireSc,
                bw_music::BuiltInPercussionInstruments::Value::CarPss,
                bw_music::BuiltInPercussionInstruments::Value::CarCrs,
                bw_music::BuiltInPercussionInstruments::Value::Siren,
                bw_music::BuiltInPercussionInstruments::Value::Train,
                bw_music::BuiltInPercussionInstruments::Value::Jetpln,
                bw_music::BuiltInPercussionInstruments::Value::Strshp,
                bw_music::BuiltInPercussionInstruments::Value::Siren,
                BW_SHORT_ID("XgBNse", "XG Burst Noise", "b57aa245-2f69-458d-94df-b77dccc456d8"),
                BW_SHORT_ID("XgCstr", "XG Coaster", "ccee260e-6319-49eb-b04f-c9f1ded56b26"),
                bw_music::BuiltInPercussionInstruments::Value::Submrn},
               52,
               &builtInInstruments},
              {{bw_music::BuiltInPercussionInstruments::Value::Laugh,
                bw_music::BuiltInPercussionInstruments::Value::Scream,
                bw_music::BuiltInPercussionInstruments::Value::Punch,
                bw_music::BuiltInPercussionInstruments::Value::HeartB,
                bw_music::BuiltInPercussionInstruments::Value::Foot1,
                bw_music::BuiltInPercussionInstruments::Value::Aplaus},
               68,
               &builtInInstruments},
              {{bw_music::BuiltInPercussionInstruments::Value::MGun,
                bw_music::BuiltInPercussionInstruments::Value::Lsrgun,
                bw_music::BuiltInPercussionInstruments::Value::Explsn,
                bw_music::BuiltInPercussionInstruments::Value::Frewrk},
               84,
               &builtInInstruments},
          },
          36) {}
