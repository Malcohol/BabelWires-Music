/**
 * The TrackRowModel provides the UI for a Track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/MusicLibUi/musicLibUiExport.hpp>

#include <BabelWiresQtUi/ValueModels/valueModelWithRichText.hpp>

namespace bw_musicUi {

    class MUSICLIBUI_API TrackValueModel : public babelwires::ValueModelWithRichText {
      public:
        QString getRichText() const override;
        QString getTooltip() const override;
    };

} // namespace bw_musicUi
