#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QString playlist_name)
{
    _name = playlist_name;
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
