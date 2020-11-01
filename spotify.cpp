#include "spotify.h"

#include <QOAuth2AuthorizationCodeFlow>
#include <QDesktopServices>
#include <QCryptographicHash>
#include <QOAuthHttpServerReplyHandler>
#include <QAbstractOAuth>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <cstdlib>

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

Spotify::Spotify()
{
    QOAuth2AuthorizationCodeFlow auth;

    /* The code verifier is a cryptographically random string between 43 and 128 characters in length  */
    const uint8_t min_str_size = 43;
    const uint8_t max_str_size = 128;
    srand(time(NULL));
    const std::string code_verifier = RandStr(rand() % (max_str_size - min_str_size) + min_str_size);
    QCryptographicHash code_verifier_hash(QCryptographicHash::Sha256);
    code_verifier_hash.addData(code_verifier.c_str(), code_verifier.size());

    const std::string auth_uri = "https://accounts.spotify.com/authorize";

    const std::string client_id = "e78f9888d48b49668abcc484afc5edab";
    const std::string access_token_uri = "https://accounts.spotify.com/api/token";
    const std::string code_challenge_method = "S256";
    const std::string code_challenge = std::to_string(code_verifier_hash.result().Base64UrlEncoding);

    const auto port = static_cast<quint16>(8080);

    auth.setAuthorizationUrl(QString::fromStdString(auth_uri));
    auth.setAccessTokenUrl(QString::fromStdString(access_token_uri));
    auth.setClientIdentifier(QString::fromStdString(client_id));
    auth.setClientIdentifierSharedKey(QString::fromStdString(code_challenge));

    QOAuthHttpServerReplyHandler *reply_handler = new QOAuthHttpServerReplyHandler(port);
    auth.setReplyHandler(reply_handler);

    QObject::connect(&auth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, QDesktopServices::openUrl);

    qInfo() << "Callback: " << reply_handler->callback();

    auth.grant();

    /* TODO: Check status code */
}
