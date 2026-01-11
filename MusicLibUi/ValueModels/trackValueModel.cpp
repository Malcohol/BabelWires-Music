/**
 * The TrackRowModel provides the UI for a Track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLibUi/ValueModels/trackValueModel.hpp>

#include <MusicLib/Types/Track/track.hpp>

#include <QString>

#include <cassert>

QString bw_musicUi::TrackValueModel::getRichText() const {
    const bw_music::Track& value = getValue()->as<bw_music::Track>();
    const babelwires::Rational duration = value.getDuration();
    return duration.toHtmlString().c_str();
}

QString bw_musicUi::TrackValueModel::getTooltip() const {
    const bw_music::Track& track = getValue()->as<bw_music::Track>();
    const auto& numEventGroupsByCategory = track.getNumEventGroupsByCategory();
    if (!numEventGroupsByCategory.empty()) {
        QString summary;
        const char* delim = "";
        for (auto p : numEventGroupsByCategory) {
            summary += delim;
            summary += QString("%1: %2").arg(p.first, QString::number(p.second));
            delim = "\n";
        }
        return summary;
    } else {
        return QString("No events");
    }
}
