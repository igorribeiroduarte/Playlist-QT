#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include <QListWidgetItem>

class Player
{
public:
    Player();

    static void play_song(QUrl song);

private:
    static QMediaPlayer player;
};

#endif // PLAYER_H
