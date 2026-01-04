/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/TestPlugin/libRegistration.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Domains/Music/Plugins/TestPlugin/testTrackContainer.hpp>
#include <Domains/Music/Plugins/TestPlugin/simpleAccompaniment.hpp>

void bw_music_testplugin::registerLib(babelwires::ProjectContext& context) {
    // Types
    context.m_typeSystem.addEntry<bw_music_testplugin::TestTrackContainer>(context.m_typeSystem);
    context.m_typeSystem.addEntry<bw_music_testplugin::SimpleAccompaniment>(context.m_typeSystem);
}