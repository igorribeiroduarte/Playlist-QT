#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QString playlist_name)
{
    _id = 0;
    _name = playlist_name;
}

PlaylistModel::PlaylistModel(const QSqlQuery &query)
{
    _id = query.value(0).toInt();
    _name = query.value(1).toString();
}

quint32 PlaylistModel::id()
{
    return _id;
}

const QString PlaylistModel::name()
{
    return _name;
}

const std::vector<TrackModel> PlaylistModel::tracks()
{
    return _tracks;
}

void PlaylistModel::add_track(TrackModel track)
{
    _tracks.push_back(track);
}

void PlaylistModel::delete_track(uint32_t id)
{
    _tracks.erase(_tracks.begin() + id);
}
