#ifndef SPOTIFY_H
#define SPOTIFY_H

#include "trackmodel.h"

#include <string>
#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QListWidget>

class Spotify : public QObject
{
Q_OBJECT

public:
    Spotify(QObject *parent = nullptr);
    ~Spotify();

    void search_track(const QString &text);

private:
    QOAuth2AuthorizationCodeFlow _auth;
    QOAuthHttpServerReplyHandler *_reply_handler;

    bool _granted;

    const QString _auth_uri = "https://accounts.spotify.com/authorize";
    const QString _client_id = "e78f9888d48b49668abcc484afc5edab";
    const QString _access_token_uri = "https://accounts.spotify.com/api/token";
    const QString _code_challenge_method = "S256";
    const quint16 _port = static_cast<quint16>(8080);

    static std::string RandStr(uint32_t size);
    void login();

private slots:
    void granted();
    void generate_track_list(QNetworkReply *reply);

signals:
    void search_finished(QNetworkReply *reply, QListWidget *list);
    void ready_to_populate(std::vector<TrackModel> tracks);
    void spotify_granted();
};

#endif // SPOTIFY_H
