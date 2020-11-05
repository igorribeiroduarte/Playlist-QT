#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include <QListWidgetItem>

class Player
{
public:
    Player();

    static void play_song(QUrl song);
    static void stop_song();

private:
    static QMediaPlayer player;
};

#endif // PLAYER_H
