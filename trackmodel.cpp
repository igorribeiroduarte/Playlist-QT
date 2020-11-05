#include "trackmodel.h"

TrackModel::TrackModel(QString track_name, QUrl track_url)
{
    _name = track_name;
    _url = track_url;
}

TrackModel::TrackModel(const QJsonObject &track_object)
{
    _name = track_object["name"].toString();
    _url = track_object["preview_url"].toString();
}

const QString TrackModel::name()
{
    return _name;
}

const QUrl TrackModel::url()
{
    return _url;
}
