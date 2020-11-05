#include "trackmodel.h"

TrackModel::TrackModel(QString track_name, QUrl track_url, quint32 playlist_id)
{
    _name = track_name;
    _url = track_url;
    _playlist_id = playlist_id;
}

TrackModel::TrackModel(const QJsonObject &track_object, quint32 playlist_id)
{
    _id = 0;
    _name = track_object["name"].toString();
    _url = track_object["preview_url"].toString();
    _playlist_id = playlist_id;
}

TrackModel::TrackModel(const QSqlQuery &query)
{
    _id = query.value(0).toInt();
    _name = query.value(1).toString();
    _url = QString(query.value(2).toString());
    _playlist_id = query.value(3).toInt();
}

quint32 TrackModel::id()
{
    return _id;
}

const QString TrackModel::name()
{
    return _name;
}

const QUrl TrackModel::url()
{
    return _url;
}

quint32 TrackModel::playlist_id()
{
    return _playlist_id;
}
