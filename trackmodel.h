#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <QSqlQuery>

class TrackModel
{
public:
    TrackModel(QString track_name, QUrl track_url, quint32 playlist_id = 0);
    TrackModel(const QJsonObject &track_object, quint32 playlist_id = 0);
    TrackModel(const QSqlQuery &query);

    quint32 id();
    const QString name();
    const QUrl url();
    quint32 playlist_id();

private:
    quint32 _id;
    QString _name;
    QUrl _url;
    quint32 _playlist_id;
};

#endif // TRACKMODEL_H
