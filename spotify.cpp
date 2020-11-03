#include "spotify.h"

#include <QDesktopServices>
#include <QCryptographicHash>

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
    _auth.setScope("user-read-private");

    _auth.setReplyHandler(_reply_handler);

    QObject::connect(&_auth, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, QDesktopServices::openUrl);
    QObject::connect(&_auth, &QOAuth2AuthorizationCodeFlow::granted, this, &Spotify::granted);

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

Spotify::~Spotify()
{
    delete _reply_handler;
}

void Spotify::granted()
{
    qInfo() << "Granted";
}
