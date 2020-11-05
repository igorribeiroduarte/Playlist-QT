#include "databaseutil.h"

const QLatin1String DatabaseUtil::PLAYLISTS_SQL =  QLatin1String(R"(
    create table playlists(id integer primary key, name varchar)
    )");

const QLatin1String DatabaseUtil::TRACKS_SQL = QLatin1String(R"(
    create table tracks(id integer primary key, name varchar,  url varchar, playlist integer, FOREIGN KEY (playlist) REFERENCES playlists(id) ON DELETE CASCADE)
    )");

const QLatin1String DatabaseUtil::INSERT_TRACK_SQL = QLatin1String(R"(
    insert into tracks(name, url, playlist) values(?, ?, ?)
    )");


const QLatin1String DatabaseUtil::INSERT_PLAYLIST_SQL = QLatin1String(R"(
    insert into playlists(name) values(?)
    )");


const QLatin1String DatabaseUtil::SELECT_PLAYLIST_SQL = QLatin1String(R"(
    select * from playlists
    )");

DatabaseUtil &DatabaseUtil::get_instance()
{
    static DatabaseUtil instance;

    return instance;
}

DatabaseUtil::DatabaseUtil()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("spotify_testing");

    if (!db.open()) {
        return;
    }

    QStringList tables = db.tables();
    QSqlQuery q;

    q.exec("PRAGMA foreign_keys=ON");

    if (!tables.contains("playlists")) {
        if (!q.exec(PLAYLISTS_SQL)){
            return;
        }
    }

    if (!tables.contains("tracks")) {
        if (!q.exec(TRACKS_SQL)){
            return;
        }
    }
}

void DatabaseUtil::add_playlist(PlaylistModel playlist)
{
    QSqlQuery q;

    if (!q.prepare(INSERT_PLAYLIST_SQL)) {
        return;
    }

    q.addBindValue(playlist.name());

    q.exec();
}

void DatabaseUtil::add_track(TrackModel track)
{
    QSqlQuery q;

    if (!q.prepare(INSERT_TRACK_SQL)) {
        return;
    }

    q.addBindValue(track.name());
    q.addBindValue(track.url());
    q.addBindValue(track.playlist_id());

    q.exec();
}

std::vector<PlaylistModel> DatabaseUtil::get_playlists()
{
    QSqlQuery q;
    q.exec(SELECT_PLAYLIST_SQL);

    std::vector<PlaylistModel> playlists;

    while (q.next()) {
        playlists.push_back(PlaylistModel(q));
        qInfo() << "Playlist criada: " << playlists.back().id();
    }

    return playlists;
}

std::vector<TrackModel> DatabaseUtil::get_tracks(uint32_t playlist_id)
{
    QSqlQuery q;
    q.exec(("select * from tracks where playlist=" + std::to_string(playlist_id)).c_str());

    std::vector<TrackModel> tracks;

    while (q.next()) {
        tracks.push_back(TrackModel(q));
    }

    return tracks;
}

std::shared_ptr<TrackModel> DatabaseUtil::get_track(uint32_t id)
{
    QSqlQuery q;
    q.exec(("select * from tracks where id=" + std::to_string(id)).c_str());

    if (q.next()) {
        return std::shared_ptr<TrackModel>(new TrackModel(q));
    }

    return nullptr;
}

std::shared_ptr<PlaylistModel> DatabaseUtil::get_playlist(uint32_t id)
{
    QSqlQuery q;
    q.exec(("select * from playlists where id=" + std::to_string(id)).c_str());

    if (q.next()) {
        return std::shared_ptr<PlaylistModel>(new PlaylistModel(q));
    }

    return nullptr;
}

void DatabaseUtil::delete_playlist(uint32_t id)
{
    QSqlQuery q;
    q.exec(("delete from playlists where id=" + std::to_string(id)).c_str());
}

void DatabaseUtil::delete_track(uint32_t id)
{
    QSqlQuery q;
    q.exec(("delete from tracks where id=" + std::to_string(id)).c_str());
}
