#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QString>
#include <QUrl>

class TrackModel
{
public:
    TrackModel(QString track_name, QUrl track_url);

    const QString name();
    const QUrl url();

private:
    QString _name;
    QUrl _url;
};

#endif // TRACKMODEL_H
