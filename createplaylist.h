#ifndef CREATEPLAYLIST_H
#define CREATEPLAYLIST_H

#include "playlistmodel.h"

#include <QDialog>

namespace Ui {
class CreatePlaylist;
}

class CreatePlaylist : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlaylist(std::vector<PlaylistModel> *playlists, QWidget *parent = nullptr);
    ~CreatePlaylist();

private:
    Ui::CreatePlaylist *ui;
    std::vector<PlaylistModel> *_playlist_ptr;

private slots:
    void create(bool);
};

#endif // CREATEPLAYLIST_H
