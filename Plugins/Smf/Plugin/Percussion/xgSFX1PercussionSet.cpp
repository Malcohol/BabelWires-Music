/**
 * A PercussionSet corresponding to the instruments of XG SFX 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/Percussion/xgSFX1PercussionSet.hpp>

smf::XgSFX1PercussionSet::XgSFX1PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments)
    : PercussionSetWithPitchMap(
          getThisIdentifier(), std::vector<InstrumentBlock>{
              {{bw_music::BuiltInPercussionInstruments::Value::GCutUp, // Guitar Cutting Noise
                bw_music::BuiltInPercussionInstruments::Value::GCutDn, // Guitar Cutting Noise 2
                bw_music::BuiltInPercussionInstruments::Value::DCutNs,
                bw_music::BuiltInPercussionInstruments::Value::DBsSlp,
                bw_music::BuiltInPercussionInstruments::Value::BssSld,
                bw_music::BuiltInPercussionInstruments::Value::PckScr},
               36,
               &builtInInstruments},
              {{bw_music::BuiltInPercussionInstruments::Value::FKyClk}, 52, &builtInInstruments},
              {{bw_music::BuiltInPercussionInstruments::Value::Rain,
                bw_music::BuiltInPercussionInstruments::Value::Thnder,
                bw_music::BuiltInPercussionInstruments::Value::Wind,
                bw_music::BuiltInPercussionInstruments::Value::Stream,
                bw_music::BuiltInPercussionInstruments::Value::Bubble,
                BW_SHORT_ID("XgFeed", "XG Feed", "68af6288-ccda-41f8-b651-d703f5fa56eb")},
               68,
               &builtInInstruments},
              {{bw_music::BuiltInPercussionInstruments::Value::Dog,
                bw_music::BuiltInPercussionInstruments::Value::Gallop,
                bw_music::BuiltInPercussionInstruments::Value::Birds,
                bw_music::BuiltInPercussionInstruments::Value::Kitty,
                bw_music::BuiltInPercussionInstruments::Value::Growl,
                bw_music::BuiltInPercussionInstruments::Value::Ghost,
                BW_SHORT_ID("XgMaou", "XG Maou", "8545a36b-2dfc-4826-8873-76989550d7c7")},
               84,
               &builtInInstruments},
          },
          36) {}
