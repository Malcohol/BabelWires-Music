/**
 * Register factories etc. for the MusicLibUi into BabelWiresUi.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLibUi/libRegistration.hpp>

#include <BabelWiresQtUi/ValueModels/valueModelRegistry.hpp>
#include <BabelWiresQtUi/uiProjectContext.hpp>

#include <MusicLib/Types/Track/trackType.hpp>

#include <MusicLibUi/ValueModels/trackValueModel.hpp>

void bw_musicUi::registerLib(babelwires::UiProjectContext& context) {
    context.m_valueModelReg.registryHandler<bw_music::TrackType, TrackValueModel>();
}
