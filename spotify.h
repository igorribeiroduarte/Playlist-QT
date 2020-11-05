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
    static Spotify &get_instance();
    static void search_track(const QString &text);

private:
    Spotify(QObject *parent = nullptr);
    ~Spotify();

    static QOAuth2AuthorizationCodeFlow _auth;
    static QOAuthHttpServerReplyHandler *_reply_handler;

    static bool _granted;

    static const QString _auth_uri;
    static const QString _client_id;
    static const QString _access_token_uri;
    static const quint16 _port;

    static std::string RandStr(uint32_t size);
    static void login();

private slots:
    static void granted();

public slots:
    static void generate_track_list(QNetworkReply *reply);

signals:
    void ready_to_populate(std::vector<TrackModel> tracks);
};

#endif // SPOTIFY_H
