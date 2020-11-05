#include "homepage.h"
#include "ui_homepage.h"
#include "playlistmodel.h"
#include "search.h"
#include "spotify.h"
#include "player.h"
#include "createplaylist.h"

#include "init_db.h"

#include <QResource>
#include <QDebug>
#include <QMessageBox>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    Spotify::get_instance();

    test_db();

    ui->horizontal_layout->setStretchFactor(ui->left_vertical_layout, 1);
    ui->horizontal_layout->setStretchFactor(ui->right_vertical_layout, 4);

    ui->left_widget->addItem("PLAYLISTS");
    QFont f("Arial", 10, QFont::Bold);
    ui->left_widget->item(0)->setFont(f);

    populate_left_widget();

    connect(ui->add_song_button, &QAbstractButton::clicked, this, &HomePage::open_search_track_page);
    connect(ui->remove_song_button, &QAbstractButton::clicked, this, &HomePage::remove_track);
    connect(ui->add_playlist_button, &QAbstractButton::clicked, this, &HomePage::open_create_playlist_page);
    connect(ui->remove_playlist_button, &QAbstractButton::clicked, this, &HomePage::remove_playlist);
    connect(ui->left_widget, &QListWidget::itemClicked, this, &HomePage::set_selected_playlist);
    connect(ui->right_widget, &QListWidget::itemClicked, this, &HomePage::set_selected_song);
}

void HomePage::test_db()
{
    qInfo("ue");
    QSqlError err = init_db();
    qInfo("depois");

    if (err.type() != QSqlError::NoError) {
        qInfo() << "Erro: " << err.text();
    }
}

void HomePage::remove_track(bool)
{
    if (_selected_playlist_id && _selected_song_id) {
        delete_track(*_selected_song_id);
    }

    _selected_song_id = nullptr;

    populate_right_widget(*_selected_playlist_id);
}

void HomePage::remove_playlist(bool)
{
    if (_selected_playlist_id) {
        delete_playlist(*_selected_playlist_id);
    }

    _selected_playlist_id = nullptr;

    populate_left_widget();
}

void HomePage::set_selected_playlist(QListWidgetItem *item)
{
    int selected_id = ui->left_widget->row(item);

    /* First row is fixed (PLAYLISTS label) and therefore is not a valid playlist */
    if (selected_id != 0) {
        _selected_playlist_id = std::make_shared<int>(item->data(Qt::UserRole).value<int>());

        populate_right_widget(*_selected_playlist_id);
    } else {
        _selected_playlist_id = nullptr;
    }
}

void HomePage::set_selected_song(QListWidgetItem *item)
{
    _selected_song_id = std::make_shared<int>(item->data(Qt::UserRole).value<int>());

    if (_selected_playlist_id) {
        std::shared_ptr<TrackModel> track = get_track(*_selected_song_id);

        if (track) {
            Player::play_song(track->url());
        }
    }
}

void HomePage::populate_left_widget()
{
    ui->left_widget->clear();
    ui->left_widget->addItem("PLAYLISTS");
    QFont f("Arial", 10, QFont::Bold);
    ui->left_widget->item(0)->setFont(f);

    for (auto it : get_playlists()) {
        QListWidgetItem *item = new QListWidgetItem(it.name());
        item->setData(Qt::UserRole, it.id());
        ui->left_widget->addItem(item);
    }
}

void HomePage::populate_right_widget(int selected_playlist)
{
    ui->right_widget->clear();

    for (auto it : get_tracks(selected_playlist)) {
        QListWidgetItem *item = new QListWidgetItem(it.name());
        item->setData(Qt::UserRole, it.id());
        ui->right_widget->addItem(item);
    }
}

void HomePage::open_create_playlist_page(bool)
{
    CreatePlaylist create_playlist;
    create_playlist.setModal(true);
    create_playlist.exec();

    populate_left_widget();
}

void HomePage::open_search_track_page(bool)
{
    if (_selected_playlist_id) {
        /* FIXME: Spotify class should be static */
        std::shared_ptr<PlaylistModel> playlist = get_playlist(*_selected_playlist_id);

        qInfo("first");

        if (playlist) {
            Search search(playlist);
            search.setModal(true);
            search.exec();
        }

        populate_right_widget(*_selected_playlist_id);
    }
}

HomePage::~HomePage()
{
    delete ui;
}
