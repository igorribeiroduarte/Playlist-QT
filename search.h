#ifndef SEARCH_H
#define SEARCH_H

#include "playlistmodel.h"
#include "spotify.h"

#include <QWidget>
#include <QDialog>
#include <QNetworkReply>
#include <QListWidget>

namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(Spotify *spotify, PlaylistModel *playlist, QWidget *parent = nullptr);
    ~Search();

private:
    Ui::Search *ui;


private slots:
    void populate(std::vector<TrackModel> tracks);
};

#endif // SEARCH_H
