#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "trackmodel.h"

#include <vector>

class PlaylistModel
{
public:
    PlaylistModel(QString playlist_name);

    const QString name();
    const std::vector<TrackModel> tracks();

    void add_track(TrackModel track);

private:
    QString _name;
    std::vector<TrackModel> _tracks;
};

#endif // PLAYLISTMODEL_H
