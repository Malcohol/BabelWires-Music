/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <TestPlugin/libRegistration.hpp>

#include <BaseLib/Context/context.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <TestPlugin/testTrackContainer.hpp>
#include <TestPlugin/simpleAccompaniment.hpp>

babelwires::Result bw_music_testplugin::registerLib(babelwires::Context& context, babelwires::UserAdvisoryLogger& userLogger) {
    // Types
    babelwires::TypeSystem& typeSystem = context.get<babelwires::TypeSystem>();
    typeSystem.addType<bw_music_testplugin::TestTrackContainer>(typeSystem);
    typeSystem.addType<bw_music_testplugin::SimpleAccompaniment>(typeSystem);
    return {};
}
