/**
 * A PercussionSet corresponding to the instruments of GS SFX Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/gsSFXPercussionSet.hpp>

smf::GsSFXPercussionSet::GsSFXPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(
          getThisIdentifier(), InstrumentBlock{
              {
                  bw_music::BuiltInPercussionInstruments::Value::ScrPsh, // Scratch Push 2
                  bw_music::BuiltInPercussionInstruments::Value::ScrPll, // Scratch Push 1
                  bw_music::BuiltInPercussionInstruments::Value::GCutUp, // Cutting Noise Up 2
                  bw_music::BuiltInPercussionInstruments::Value::GCutDn, // Cutting Noise Down 2
                  bw_music::BuiltInPercussionInstruments::Value::GCutUp, // Distortion Guitar Cutting Noise Up
                  bw_music::BuiltInPercussionInstruments::Value::GCutDn, // Distortion Guitar Cutting Noise Down
                  bw_music::BuiltInPercussionInstruments::Value::BssSld,
                  bw_music::BuiltInPercussionInstruments::Value::PckScr,
                  bw_music::BuiltInPercussionInstruments::Value::HighQ,
                  bw_music::BuiltInPercussionInstruments::Value::Slap,
                  bw_music::BuiltInPercussionInstruments::Value::ScrPsh,
                  bw_music::BuiltInPercussionInstruments::Value::ScrPll,
                  bw_music::BuiltInPercussionInstruments::Value::Stcks,
                  bw_music::BuiltInPercussionInstruments::Value::SqClck,
                  bw_music::BuiltInPercussionInstruments::Value::MtrClk,
                  bw_music::BuiltInPercussionInstruments::Value::MtrBll,
                  bw_music::BuiltInPercussionInstruments::Value::GFret,
                  bw_music::BuiltInPercussionInstruments::Value::GCutUp,
                  bw_music::BuiltInPercussionInstruments::Value::GCutDn,
                  bw_music::BuiltInPercussionInstruments::Value::DBsSlp,
                  bw_music::BuiltInPercussionInstruments::Value::FKyClk,
                  bw_music::BuiltInPercussionInstruments::Value::Laugh,
                  bw_music::BuiltInPercussionInstruments::Value::Scream,
                  bw_music::BuiltInPercussionInstruments::Value::Punch,
                  bw_music::BuiltInPercussionInstruments::Value::HeartB,
                  bw_music::BuiltInPercussionInstruments::Value::Foot1,
                  bw_music::BuiltInPercussionInstruments::Value::Foot2,
                  bw_music::BuiltInPercussionInstruments::Value::Aplaus,
                  bw_music::BuiltInPercussionInstruments::Value::DoorCr,
                  bw_music::BuiltInPercussionInstruments::Value::Door,
                  bw_music::BuiltInPercussionInstruments::Value::Scrtch,
                  bw_music::BuiltInPercussionInstruments::Value::WChime,
                  bw_music::BuiltInPercussionInstruments::Value::CarEng,
                  bw_music::BuiltInPercussionInstruments::Value::CarStp,
                  bw_music::BuiltInPercussionInstruments::Value::CarPss,
                  bw_music::BuiltInPercussionInstruments::Value::CarCrs,
                  bw_music::BuiltInPercussionInstruments::Value::Siren,
                  bw_music::BuiltInPercussionInstruments::Value::Train,
                  bw_music::BuiltInPercussionInstruments::Value::Jetpln,
                  bw_music::BuiltInPercussionInstruments::Value::Heli,
                  bw_music::BuiltInPercussionInstruments::Value::Strshp,
                  bw_music::BuiltInPercussionInstruments::Value::GunSht,
                  bw_music::BuiltInPercussionInstruments::Value::MGun,
                  bw_music::BuiltInPercussionInstruments::Value::Lsrgun,
                  bw_music::BuiltInPercussionInstruments::Value::Explsn,
                  bw_music::BuiltInPercussionInstruments::Value::Dog,
                  bw_music::BuiltInPercussionInstruments::Value::Gallop,
                  bw_music::BuiltInPercussionInstruments::Value::Birds,
                  bw_music::BuiltInPercussionInstruments::Value::Rain,
                  bw_music::BuiltInPercussionInstruments::Value::Thnder,
                  bw_music::BuiltInPercussionInstruments::Value::Wind,
                  bw_music::BuiltInPercussionInstruments::Value::Seashr,
                  bw_music::BuiltInPercussionInstruments::Value::Stream,
                  bw_music::BuiltInPercussionInstruments::Value::Bubble,
                  bw_music::BuiltInPercussionInstruments::Value::Kitty,
                  bw_music::BuiltInPercussionInstruments::Value::Birds, // Bird 2
                  bw_music::BuiltInPercussionInstruments::Value::Growl,
                  bw_music::BuiltInPercussionInstruments::Value::Aplaus, // Applause 2
                  bw_music::BuiltInPercussionInstruments::Value::TeleRn, // Telephone 1
                  bw_music::BuiltInPercussionInstruments::Value::TeleRn  // Telephone 2
              },
              31,
              &builtInInstruments},
          31) {}
