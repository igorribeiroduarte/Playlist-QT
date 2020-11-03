#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <string>
#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

class Spotify : public QObject
{
Q_OBJECT

public:
    Spotify(QObject *parent = nullptr);
    ~Spotify();
    void test();

private:
    QOAuth2AuthorizationCodeFlow _auth;
    QOAuthHttpServerReplyHandler *_reply_handler;

    const QString _auth_uri = "https://accounts.spotify.com/authorize";
    const QString _client_id = "e78f9888d48b49668abcc484afc5edab";
    const QString _access_token_uri = "https://accounts.spotify.com/api/token";
    const QString _code_challenge_method = "S256";
    const quint16 _port = static_cast<quint16>(8080);


    static std::string RandStr(uint32_t size);

private slots:
    void granted();
};

#endif // SPOTIFY_H
