#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "trackmodel.h"

#include <vector>

class PlaylistModel
{
public:
    PlaylistModel(QString playlist_name);
    PlaylistModel(const QSqlQuery &query);

    quint32 id();
    const QString name();
    const std::vector<TrackModel> tracks();

    void add_track(TrackModel track);
    void delete_track(uint32_t id);

private:
    quint32 _id;
    QString _name;
    std::vector<TrackModel> _tracks;
};

#endif // PLAYLISTMODEL_H
