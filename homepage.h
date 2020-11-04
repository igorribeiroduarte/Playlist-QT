#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "playlistmodel.h"

#include <QWidget>
#include <QListWidgetItem>
#include <QMediaPlayer>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:
    Ui::HomePage *ui;

    QMediaPlayer _player;

    /* FIXME: Temp var. Remove it after adding database */
    std::vector<PlaylistModel> playlists;


    std::shared_ptr<int> _selected_playlist_id;
    std::shared_ptr<int> _selected_song_id;

    void populate_left_widget();
    void populate_right_widget(int selected_playlist);
    void play_song(TrackModel track);

private slots:
    void open_search_track_page(bool checked);
    void set_selected_playlist(QListWidgetItem *item);
    void set_selected_song(QListWidgetItem *item);
};

#endif // HOMEPAGE_H
