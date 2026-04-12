/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/TestPlugin/libRegistration.hpp>

#include <BaseLib/Context/context.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Domains/Music/Plugins/TestPlugin/testTrackContainer.hpp>
#include <Domains/Music/Plugins/TestPlugin/simpleAccompaniment.hpp>

void bw_music_testplugin::registerLib(babelwires::Context& context) {
    // Types
    context.getService<babelwires::TypeSystem>().addType<bw_music_testplugin::TestTrackContainer>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<bw_music_testplugin::SimpleAccompaniment>(context.getService<babelwires::TypeSystem>());
}