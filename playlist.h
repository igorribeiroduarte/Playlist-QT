#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "trackmodel.h"

#include <QOAuth2AuthorizationCodeFlow>
#include <QWidget>
#include <QListWidget>
#include <QMediaPlayer>
#include <QHBoxLayout>

namespace Ui {
class Playlist;
}

class Playlist : public QWidget
{
    Q_OBJECT

public:
    explicit Playlist(QWidget *parent = nullptr);
    ~Playlist();

    static void play_song(QUrl song);

private slots:
    void play_new_song(QListWidgetItem * item);
    void populate(std::vector<TrackModel> tracks);

private:
    Ui::Playlist *ui;

    static QMediaPlayer player;

};

#endif // PLAYLIST_H
