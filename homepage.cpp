#include "homepage.h"
#include "ui_homepage.h"
#include "playlistmodel.h"
#include "search.h"
#include "spotify.h"
#include "player.h"
#include "createplaylist.h"

#include <QResource>
#include <QDebug>

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    ui->horizontal_layout->setStretchFactor(ui->left_vertical_layout, 1);
    ui->horizontal_layout->setStretchFactor(ui->right_vertical_layout, 4);

    ui->left_widget->addItem("PLAYLISTS");
    QFont f("Arial", 10, QFont::Bold);
    ui->left_widget->item(0)->setFont(f);


    /* FIXME: Remove this after implementing the DB */


    playlists.push_back(PlaylistModel("First"));
    playlists.push_back(PlaylistModel("Second"));
    playlists.push_back(PlaylistModel("Third"));

    populate_left_widget();

    connect(ui->add_song_button, &QAbstractButton::clicked, this, &HomePage::open_search_track_page);
    connect(ui->remove_song_button, &QAbstractButton::clicked, this, &HomePage::remove_track);
    connect(ui->add_playlist_button, &QAbstractButton::clicked, this, &HomePage::open_create_playlist_page);
    connect(ui->left_widget, &QListWidget::itemClicked, this, &HomePage::set_selected_playlist);
    connect(ui->right_widget, &QListWidget::itemClicked, this, &HomePage::set_selected_song);
}

void HomePage::remove_track(bool)
{
    if (_selected_playlist_id && _selected_song_id) {
        playlists[*_selected_playlist_id].delete_track(*_selected_song_id);
    }

    _selected_song_id = nullptr;

    populate_right_widget(*_selected_playlist_id);
}

int HomePage::get_playlist_vector_id_from_item_row(int row)
{
   /* It's necessary to decrease by one to eliminate the fixed row (first) */
   return row - 1;
}

void HomePage::set_selected_playlist(QListWidgetItem *item)
{
    int selected_id = ui->left_widget->row(item);

    /* First row is fixed (PLAYLISTS label) and therefore is not a valid playlist */
    if (selected_id != 0) {
        _selected_playlist_id = std::make_shared<int>(get_playlist_vector_id_from_item_row(selected_id));

        populate_right_widget(*_selected_playlist_id);
    } else {
        _selected_playlist_id = nullptr;
    }
}

void HomePage::set_selected_song(QListWidgetItem *item)
{
    _selected_song_id = std::make_shared<int>(ui->right_widget->row(item));

    if (_selected_playlist_id) {
        TrackModel track = (playlists[*_selected_playlist_id].tracks()[*_selected_song_id]);
        Player::play_song(track.url());
    }
}

void HomePage::populate_left_widget()
{
    ui->left_widget->clear();
    ui->left_widget->addItem("PLAYLISTS");
    QFont f("Arial", 10, QFont::Bold);
    ui->left_widget->item(0)->setFont(f);

    for (auto it : playlists) {
        ui->left_widget->addItem(it.name());
    }
}

void HomePage::populate_right_widget(int selected_playlist)
{
    ui->right_widget->clear();
    for (auto it : playlists[selected_playlist].tracks()) {
        ui->right_widget->addItem(it.name());
    }
}

void HomePage::open_create_playlist_page(bool)
{
    CreatePlaylist create_playlist(&playlists);
    create_playlist.setModal(true);
    create_playlist.exec();

    populate_left_widget();
}

void HomePage::open_search_track_page(bool)
{
    if (_selected_playlist_id) {
        /* Spotify class should be static */
        Search search(new Spotify(), &playlists[*_selected_playlist_id]);
        search.setModal(true);
        search.exec();

        populate_right_widget(*_selected_playlist_id);
    }
}

HomePage::~HomePage()
{
    delete ui;
}
