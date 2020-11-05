#include "player.h"

QMediaPlayer Player::player;

Player::Player()
{
    player.setVolume(80);
}

void Player::play_song(QUrl song)
{
    player.setMedia(song);
    player.play();
}
