/**
 * A PercussionSet corresponding to the instruments of XG BrushPercussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/xgBrushPercussionSet.hpp>

// Follow GM2 and do not distinguish Brush toms from regular toms.
// Choosing JKick2 instead of JKick1 to ensure its set of instruments differs from the XG Jazz set.
// (It needs to exist, since it has a different layout to the Jazz set, but a different instrument
// set ensures it's easier to test.)
smf::XgBrushPercussionSet::XgBrushPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(getThisIdentifier(), InstrumentBlock{{bw_music::BuiltInPercussionInstruments::Value::MSurdo,
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
                                                 bw_music::BuiltInPercussionInstruments::Value::BrSwrH,
                                                 bw_music::BuiltInPercussionInstruments::Value::SnrRll,
                                                 bw_music::BuiltInPercussionInstruments::Value::Cstnts,
                                                 bw_music::BuiltInPercussionInstruments::Value::BrSlap, // Snare L
                                                 bw_music::BuiltInPercussionInstruments::Value::Stcks,
                                                 bw_music::BuiltInPercussionInstruments::Value::Bass1, // Bass Drum L
                                                 bw_music::BuiltInPercussionInstruments::Value::RimSht,
                                                 bw_music::BuiltInPercussionInstruments::Value::AcBass,
                                                 bw_music::BuiltInPercussionInstruments::Value::JKick2, // BD Soft
                                                 bw_music::BuiltInPercussionInstruments::Value::SStick,
                                                 bw_music::BuiltInPercussionInstruments::Value::BrSlap,
                                                 bw_music::BuiltInPercussionInstruments::Value::Clap,
                                                 bw_music::BuiltInPercussionInstruments::Value::BrTap,
                                                 bw_music::BuiltInPercussionInstruments::Value::LFlTom,
                                                 bw_music::BuiltInPercussionInstruments::Value::ClHHat,
                                                 bw_music::BuiltInPercussionInstruments::Value::HFlTom,
                                                 bw_music::BuiltInPercussionInstruments::Value::PdHHat,
                                                 bw_music::BuiltInPercussionInstruments::Value::LwTom,
                                                 bw_music::BuiltInPercussionInstruments::Value::OpHHat,
                                                 bw_music::BuiltInPercussionInstruments::Value::LMTom,
                                                 bw_music::BuiltInPercussionInstruments::Value::HMTom,
                                                 bw_music::BuiltInPercussionInstruments::Value::Crash1,
                                                 bw_music::BuiltInPercussionInstruments::Value::HTom,
                                                 bw_music::BuiltInPercussionInstruments::Value::Ride1,
                                                 bw_music::BuiltInPercussionInstruments::Value::ChnCym,
                                                 bw_music::BuiltInPercussionInstruments::Value::RideBl,
                                                 bw_music::BuiltInPercussionInstruments::Value::Tamb,
                                                 bw_music::BuiltInPercussionInstruments::Value::SplCym,
                                                 bw_music::BuiltInPercussionInstruments::Value::Cowbll,
                                                 bw_music::BuiltInPercussionInstruments::Value::Crash2,
                                                 bw_music::BuiltInPercussionInstruments::Value::VibraS,
                                                 bw_music::BuiltInPercussionInstruments::Value::Ride2,
                                                 bw_music::BuiltInPercussionInstruments::Value::HBongo,
                                                 bw_music::BuiltInPercussionInstruments::Value::LBongo,
                                                 bw_music::BuiltInPercussionInstruments::Value::MHCnga,
                                                 bw_music::BuiltInPercussionInstruments::Value::OHCnga,
                                                 bw_music::BuiltInPercussionInstruments::Value::LConga,
                                                 bw_music::BuiltInPercussionInstruments::Value::HTimbl,
                                                 bw_music::BuiltInPercussionInstruments::Value::LTimbl,
                                                 bw_music::BuiltInPercussionInstruments::Value::HAgogo,
                                                 bw_music::BuiltInPercussionInstruments::Value::LAgogo,
                                                 bw_music::BuiltInPercussionInstruments::Value::Cabasa,
                                                 bw_music::BuiltInPercussionInstruments::Value::Maracs,
                                                 bw_music::BuiltInPercussionInstruments::Value::SWhisl,
                                                 bw_music::BuiltInPercussionInstruments::Value::LWhisl,
                                                 bw_music::BuiltInPercussionInstruments::Value::SGuiro,
                                                 bw_music::BuiltInPercussionInstruments::Value::LGuiro,
                                                 bw_music::BuiltInPercussionInstruments::Value::Claves,
                                                 bw_music::BuiltInPercussionInstruments::Value::HWoodB,
                                                 bw_music::BuiltInPercussionInstruments::Value::LWoodB,
                                                 bw_music::BuiltInPercussionInstruments::Value::MCuica,
                                                 bw_music::BuiltInPercussionInstruments::Value::OCuica,
                                                 bw_music::BuiltInPercussionInstruments::Value::MTrian,
                                                 bw_music::BuiltInPercussionInstruments::Value::OTrian,
                                                 bw_music::BuiltInPercussionInstruments::Value::Shaker,
                                                 bw_music::BuiltInPercussionInstruments::Value::JngBll,
                                                 bw_music::BuiltInPercussionInstruments::Value::BlTree},
                                                13,
                                                &builtInInstruments},
                                13) {}
