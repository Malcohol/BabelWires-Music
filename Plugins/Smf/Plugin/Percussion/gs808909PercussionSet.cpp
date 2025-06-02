/**
 * A PercussionSet corresponding to the instruments of GS Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/gs808909PercussionSet.hpp>

smf::Gs808909PercussionSet::Gs808909PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(InstrumentBlock{{bw_music::BuiltInPercussionInstruments::Value::SnrRll,
                                                 bw_music::BuiltInPercussionInstruments::Value::FngSnp,
                                                 bw_music::BuiltInPercussionInstruments::Value::HighQ,
                                                 bw_music::BuiltInPercussionInstruments::Value::Slap,
                                                 bw_music::BuiltInPercussionInstruments::Value::ScrPsh,
                                                 bw_music::BuiltInPercussionInstruments::Value::ScrPll,
                                                 bw_music::BuiltInPercussionInstruments::Value::Stcks,
                                                 bw_music::BuiltInPercussionInstruments::Value::SqClck,
                                                 bw_music::BuiltInPercussionInstruments::Value::MtrClk,
                                                 bw_music::BuiltInPercussionInstruments::Value::MtrBll,
                                                 bw_music::BuiltInPercussionInstruments::Value::Bs909,
                                                 bw_music::BuiltInPercussionInstruments::Value::Bs808,
                                                 bw_music::BuiltInPercussionInstruments::Value::RimSht, // 808 Rim Shot
                                                 bw_music::BuiltInPercussionInstruments::Value::ElSnr,  // 808 Snare 1
                                                 bw_music::BuiltInPercussionInstruments::Value::Clap,
                                                 bw_music::BuiltInPercussionInstruments::Value::ElSnr2, // 909 Snare 1
                                                 bw_music::BuiltInPercussionInstruments::Value::ELwTm2, // 808 Low Tom 2
                                                 bw_music::BuiltInPercussionInstruments::Value::ClHHat, // 808 CHH
                                                 bw_music::BuiltInPercussionInstruments::Value::ELwTm1, // 808 Low Tom 1
                                                 bw_music::BuiltInPercussionInstruments::Value::PdHHat, // 808 CHH
                                                 bw_music::BuiltInPercussionInstruments::Value::EMdTm2, // 808 Mid Tom 2
                                                 bw_music::BuiltInPercussionInstruments::Value::OpHHat, // 808 OHH
                                                 bw_music::BuiltInPercussionInstruments::Value::EMdTm1, // 808 Low Tom 1
                                                 bw_music::BuiltInPercussionInstruments::Value::EHiTm2, // 808 Hi Tom 2
                                                 bw_music::BuiltInPercussionInstruments::Value::Crash1, // 808 Cymbal
                                                 bw_music::BuiltInPercussionInstruments::Value::EHiTm1, // 808 Hi Tom 1
                                                 bw_music::BuiltInPercussionInstruments::Value::Ride1,
                                                 bw_music::BuiltInPercussionInstruments::Value::ChnCym,
                                                 bw_music::BuiltInPercussionInstruments::Value::RideBl,
                                                 bw_music::BuiltInPercussionInstruments::Value::Tamb,
                                                 bw_music::BuiltInPercussionInstruments::Value::SplCym,
                                                 bw_music::BuiltInPercussionInstruments::Value::Cowbll, // 808 Cowbell
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
                                                 bw_music::BuiltInPercussionInstruments::Value::BlTree,
                                                 bw_music::BuiltInPercussionInstruments::Value::Cstnts,
                                                 bw_music::BuiltInPercussionInstruments::Value::MSurdo,
                                                 bw_music::BuiltInPercussionInstruments::Value::OSurdo},
                                                25,
                                                &builtInInstruments},
                                25) {}
