/**
 * Register factories etc. for the MusicLibUi into BabelWiresUi.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLibUi/libRegistration.hpp>

#include <BabelWiresQtUi/ValueModels/valueModelRegistry.hpp>
#include <BaseLib/Context/context.hpp>

#include <MusicLib/Types/Track/trackType.hpp>

#include <MusicLibUi/ValueModels/trackValueModel.hpp>

void bw_musicUi::registerLib(babelwires::Context& context) {
    context.get<babelwires::ValueModelRegistry>().registryHandler<bw_music::TrackType, TrackValueModel>();
}
