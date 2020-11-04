#include "trackmodel.h"

TrackModel::TrackModel(QString track_name, QUrl track_url)
{
    _name = track_name;
    _url = track_url;
}

const QString TrackModel::name()
{
    return _name;
}

const QUrl TrackModel::url()
{
    return _url;
}
