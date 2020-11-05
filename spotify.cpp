#include "spotify.h"

#include "trackmodel.h"

#include <QDesktopServices>
#include <QCryptographicHash>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QtMultimedia/QMediaPlayer>
#include <future>
#include <iostream>
#include <thread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

std::string Spotify::RandStr(uint32_t size)
{
    std::string alpha="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.-~";
    std::string ret;

    srand(time(NULL));

    for (size_t i = 0; i < size; i++) {
        ret += alpha[rand() % alpha.size()];
    }

    qInfo() << "Random Str: " << ret.c_str();

    return ret;
}

Spotify::Spotify(QObject *)
{
    _reply_handler = new QOAuthHttpServerReplyHandler(_port);
    _auth.setReplyHandler(_reply_handler);
    _granted = false;

    login();

    QTimer timer;
    QEventLoop loop;
    connect(&_auth, &QOAuth2AuthorizationCodeFlow::granted, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(1000000);
    loop.exec();

    connect(_auth.networkAccessManager(), &QNetworkAccessManager::finished, this, &Spotify::generate_track_list);
    connect(this, &Spotify::search_finished, this, &Spotify::generate_track_list);
}

Spotify::~Spotify()
{
    delete _reply_handler;
}

void Spotify::login()
{
    /* The code verifier is a cryptographically random string between 43 and 128 characters in length  */
    const uint8_t min_str_size = 43;
    const uint8_t max_str_size = 128;
    srand(time(NULL));
    const std::string code_verifier = RandStr(rand() % (max_str_size - min_str_size) + min_str_size);
    QCryptographicHash code_verifier_hash(QCryptographicHash::Sha256);
    code_verifier_hash.addData(code_verifier.c_str(), code_verifier.size());

    const QByteArray code_challenge = code_verifier_hash.result().toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    qInfo() << "Code Challenge: " << code_challenge;

    _auth.setAuthorizationUrl(_auth_uri);
    _auth.setAccessTokenUrl(_access_token_uri);
    _auth.setClientIdentifier(_client_id);
    _auth.setScope("user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private");

    connect(&_auth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, QDesktopServices::openUrl);
    connect(&_auth, &QOAuth2AuthorizationCodeFlow::granted, this, &Spotify::granted);

    qInfo() << "Callback: " << _reply_handler->callback();

    _auth.setModifyParametersFunction([this, code_verifier, code_challenge](QAbstractOAuth::Stage stage, QVariantMap* parameters) {
            if (stage == QAbstractOAuth::Stage::RequestingAccessToken)
            {
                if (parameters->toStdMap().find("code") != parameters->toStdMap().end()) {
                    qInfo() << "Requesting acess token";

                    parameters->insert("code_verifier", QByteArray::fromStdString(code_verifier));
                }
            }
            if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
                qInfo() << "Requesting authorization";

                parameters->insert("code_challenge_method", this->_code_challenge_method);
                parameters->insert("code_challenge", code_challenge);
            }
        });

    _auth.grant();

    /* TODO: Handle errors */
}

void Spotify::search_track(const QString &text)
{
    const QString query = QUrl::toPercentEncoding(text);
    qInfo() << "query: " << query;
    const QString type = "track";

    QVariantMap parameters;
    parameters.insert("q", query);
    parameters.insert("type", type);

    _auth.get(QUrl("https://api.spotify.com/v1/search"), parameters);
}

void Spotify::generate_track_list(QNetworkReply *reply)
{
    const uint32_t status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const uint32_t expected_status_code = 200;

    std::vector<TrackModel> tracks;
    if (status_code == expected_status_code) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray track_json_array = doc["tracks"].toObject()["items"].toArray();

        for (const auto &it : track_json_array) {
            tracks.push_back(TrackModel(it.toObject()));
        }
    }

    emit ready_to_populate(tracks);
}

void Spotify::granted()
{
    qInfo() << "Granted";
    _granted = true;

    emit spotify_granted();
}
