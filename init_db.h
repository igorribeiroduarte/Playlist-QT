#ifndef INIT_DB_H
#define INIT_DB_H

#include <QtSql>
#include <QSqlTableModel>

#include "trackmodel.h"
#include "playlistmodel.h"

const auto PLAYLISTS_SQL =  QLatin1String(R"(
    create table playlists(id integer primary key, name varchar)
    )");

const auto TRACKS_SQL = QLatin1String(R"(
    create table tracks(id integer primary key, name varchar,  url varchar, playlist integer, FOREIGN KEY (playlist) REFERENCES playlists(id) ON DELETE CASCADE)
    )");

const auto INSERT_TRACK_SQL = QLatin1String(R"(
    insert into tracks(name, url, playlist) values(?, ?, ?)
    )");


const auto INSERT_PLAYLIST_SQL = QLatin1String(R"(
    insert into playlists(name) values(?)
    )");


const auto SELECT_PLAYLIST_SQL = QLatin1String(R"(
    select * from playlists
    )");

static void add_playlist(PlaylistModel playlist)
{
    QSqlQuery q;

    qInfo() << "entrei";

    if (!q.prepare(INSERT_PLAYLIST_SQL)) {
        return;
    }

    q.addBindValue(playlist.name());

    q.exec();
}

static void add_track(TrackModel track)
{
    QSqlQuery q;

    qInfo() << "entrei";

    if (!q.prepare(INSERT_TRACK_SQL)) {
        return;
    }

    q.addBindValue(track.name());
    q.addBindValue(track.url());
    q.addBindValue(track.playlist_id());

    q.exec();
}

static std::vector<PlaylistModel> get_playlists()
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

static std::vector<TrackModel> get_tracks(uint32_t playlist_id)
{
    QSqlQuery q;
    q.exec(("select * from tracks where playlist=" + std::to_string(playlist_id)).c_str());

    std::vector<TrackModel> tracks;

    while (q.next()) {
        tracks.push_back(TrackModel(q));
    }

    return tracks;
}

static std::shared_ptr<TrackModel> get_track(uint32_t id)
{
    QSqlQuery q;
    q.exec(("select * from tracks where id=" + std::to_string(id)).c_str());

    if (q.next()) {
        return std::shared_ptr<TrackModel>(new TrackModel(q));
    }

    return nullptr;
}

static std::shared_ptr<PlaylistModel> get_playlist(uint32_t id)
{
    QSqlQuery q;
    q.exec(("select * from playlists where id=" + std::to_string(id)).c_str());

    if (q.next()) {
        return std::shared_ptr<PlaylistModel>(new PlaylistModel(q));
    }

    return nullptr;
}

static void delete_playlist(uint32_t id)
{
    QSqlQuery q;
    q.exec(("delete from playlists where id=" + std::to_string(id)).c_str());
}

static void delete_track(uint32_t id)
{
    QSqlQuery q;
    q.exec(("delete from tracks where id=" + std::to_string(id)).c_str());
}

static QSqlError init_db()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("uafe2");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    QSqlQuery q;

    q.exec("PRAGMA foreign_keys=ON");

    if (!tables.contains("playlists")) {
        if (!q.exec(PLAYLISTS_SQL)){
            return q.lastError();
        }
    }

    if (!tables.contains("tracks")) {
        if (!q.exec(TRACKS_SQL)){
            return q.lastError();
        }
    }

    return QSqlError();
}

#endif // INIT_DB_H
