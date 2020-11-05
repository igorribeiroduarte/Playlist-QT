#ifndef DATABASEUTIL_H
#define DATABASEUTIL_H

#include <QtSql>
#include <QSqlTableModel>

#include "trackmodel.h"
#include "playlistmodel.h"

#include <QLatin1String>

class DatabaseUtil
{
public:
    static DatabaseUtil &get_instance();

    static void add_playlist(PlaylistModel playlist);
    static void add_track(TrackModel track);
    static std::vector<PlaylistModel> get_playlists();
    static std::vector<TrackModel> get_tracks(uint32_t playlist_id);
    static std::shared_ptr<TrackModel> get_track(uint32_t id);
    static std::shared_ptr<PlaylistModel> get_playlist(uint32_t id);
    static void delete_playlist(uint32_t id);
    static void delete_track(uint32_t id);

private:
    DatabaseUtil();

    static const QLatin1String PLAYLISTS_SQL;
    static const QLatin1String TRACKS_SQL;
    static const QLatin1String INSERT_TRACK_SQL;
    static const QLatin1String INSERT_PLAYLIST_SQL;
    static const QLatin1String SELECT_PLAYLIST_SQL;
};

#endif // DATABASEUTIL_H
