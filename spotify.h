#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <string>

class Spotify
{
public:
    Spotify();

private:
    static std::string RandStr(uint32_t size);
    bool login();
};

#endif // SPOTIFY_H
