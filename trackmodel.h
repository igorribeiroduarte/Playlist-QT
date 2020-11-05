#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QString>
#include <QUrl>
#include <QJsonObject>

class TrackModel
{
public:
    TrackModel(QString track_name, QUrl track_url);
    TrackModel(const QJsonObject &track_object);

    const QString name();
    const QUrl url();

private:
    QString _name;
    QUrl _url;
};

#endif // TRACKMODEL_H
