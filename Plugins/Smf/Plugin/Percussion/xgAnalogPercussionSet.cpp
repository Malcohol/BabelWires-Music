/**
 * A PercussionSet corresponding to the instruments of XG Analog Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/xgAnalogPercussionSet.hpp>

smf::XgAnalogPercussionSet::XgAnalogPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(
          getThisType(), InstrumentBlock{{bw_music::BuiltInPercussionInstruments::Value::MSurdo,
                           bw_music::BuiltInPercussionInstruments::Value::OSurdo,
                           bw_music::BuiltInPercussionInstruments::Value::HighQ,
                           bw_music::BuiltInPercussionInstruments::Value::Slap,
                           bw_music::BuiltInPercussionInstruments::Value::ScrPsh,
                           bw_music::BuiltInPercussionInstruments::Value::ScrPll,
                           bw_music::BuiltInPercussionInstruments::Value::FngSnp,
                           bw_music::BuiltInPercussionInstruments::Value::SqClck,
                           bw_music::BuiltInPercussionInstruments::Value::MtrClk,
                           bw_music::BuiltInPercussionInstruments::Value::MtrBll,
                           bw_music::BuiltInPercussionInstruments::Value::BeepLo, // Seq Click L
                           bw_music::BuiltInPercussionInstruments::Value::BeepHi, // Seq Click H
                           bw_music::BuiltInPercussionInstruments::Value::BrTap,
                           bw_music::BuiltInPercussionInstruments::Value::BrSwrL,
                           bw_music::BuiltInPercussionInstruments::Value::BrSlap,
                           bw_music::BuiltInPercussionInstruments::Value::RevCym, // Swirl H
                           bw_music::BuiltInPercussionInstruments::Value::SnrRll,
                           bw_music::BuiltInPercussionInstruments::Value::HighQ,
                           bw_music::BuiltInPercussionInstruments::Value::AcSnr, // SD Rock H
                           bw_music::BuiltInPercussionInstruments::Value::Stcks,
                           bw_music::BuiltInPercussionInstruments::Value::Bass1, // Bass Drum M
                           bw_music::BuiltInPercussionInstruments::Value::RimSht,
                           bw_music::BuiltInPercussionInstruments::Value::AnBass, // BD Analog L
                           bw_music::BuiltInPercussionInstruments::Value::AnBass, // BD Analog H
                           bw_music::BuiltInPercussionInstruments::Value::SStick, // Analog Side Stick
                           bw_music::BuiltInPercussionInstruments::Value::AcSnr,  // Analog Snare L
                           bw_music::BuiltInPercussionInstruments::Value::Clap,
                           bw_music::BuiltInPercussionInstruments::Value::ElSnr, // Analog Snare H
                           bw_music::BuiltInPercussionInstruments::Value::ALwTm2,
                           bw_music::BuiltInPercussionInstruments::Value::ACHH1,
                           bw_music::BuiltInPercussionInstruments::Value::ALwTm1,
                           bw_music::BuiltInPercussionInstruments::Value::ACHH2,
                           bw_music::BuiltInPercussionInstruments::Value::AMdTm2,
                           bw_music::BuiltInPercussionInstruments::Value::AOHH,
                           bw_music::BuiltInPercussionInstruments::Value::AMdTm1,
                           bw_music::BuiltInPercussionInstruments::Value::AHiTm2,
                           bw_music::BuiltInPercussionInstruments::Value::AnCym,
                           bw_music::BuiltInPercussionInstruments::Value::AHiTm1,
                           bw_music::BuiltInPercussionInstruments::Value::Ride1,
                           bw_music::BuiltInPercussionInstruments::Value::ChnCym,
                           bw_music::BuiltInPercussionInstruments::Value::RideBl,
                           bw_music::BuiltInPercussionInstruments::Value::Tamb,
                           bw_music::BuiltInPercussionInstruments::Value::SplCym,
                           bw_music::BuiltInPercussionInstruments::Value::ACwbll,
                           bw_music::BuiltInPercussionInstruments::Value::Crash2,
                           bw_music::BuiltInPercussionInstruments::Value::VibraS,
                           bw_music::BuiltInPercussionInstruments::Value::Ride2,
                           bw_music::BuiltInPercussionInstruments::Value::HBongo,
                           bw_music::BuiltInPercussionInstruments::Value::LBongo,
                           bw_music::BuiltInPercussionInstruments::Value::AHCnga,
                           bw_music::BuiltInPercussionInstruments::Value::AMCnga,
                           bw_music::BuiltInPercussionInstruments::Value::ALCnga,
                           bw_music::BuiltInPercussionInstruments::Value::HTimbl,
                           bw_music::BuiltInPercussionInstruments::Value::LTimbl,
                           bw_music::BuiltInPercussionInstruments::Value::HAgogo,
                           bw_music::BuiltInPercussionInstruments::Value::LAgogo,
                           bw_music::BuiltInPercussionInstruments::Value::Cabasa,
                           bw_music::BuiltInPercussionInstruments::Value::AMrcas,
                           bw_music::BuiltInPercussionInstruments::Value::SWhisl,
                           bw_music::BuiltInPercussionInstruments::Value::LWhisl,
                           bw_music::BuiltInPercussionInstruments::Value::SGuiro,
                           bw_music::BuiltInPercussionInstruments::Value::LGuiro,
                           bw_music::BuiltInPercussionInstruments::Value::AClavs,
                           bw_music::BuiltInPercussionInstruments::Value::HWoodB,
                           bw_music::BuiltInPercussionInstruments::Value::LWoodB,
                           bw_music::BuiltInPercussionInstruments::Value::ScrPsh,
                           bw_music::BuiltInPercussionInstruments::Value::ScrPll,
                           bw_music::BuiltInPercussionInstruments::Value::MTrian,
                           bw_music::BuiltInPercussionInstruments::Value::OTrian,
                           bw_music::BuiltInPercussionInstruments::Value::Shaker,
                           bw_music::BuiltInPercussionInstruments::Value::JngBll,
                           bw_music::BuiltInPercussionInstruments::Value::BlTree},
                          13,
                          &builtInInstruments},
          13) {}
