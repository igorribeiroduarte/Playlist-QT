#include "homepage.h"
#include "ui_homepage.h"
#include "playlistmodel.h"
#include "search.h"

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

    playlists[0].add_track(TrackModel("First", QUrl("https://p.scdn.co/mp3-preview/3eb16018c2a700240e9dfb8817b6f2d041f15eb1?cid=774b29d4f13844c495f206cafdad9c86")));

    playlists[1].add_track(TrackModel("Second", QUrl("https://p.scdn.co/mp3-preview/d7624ec5f93b6d92c1836a95c40ecce463584f6e?cid=774b29d4f13844c495f206cafdad9c86")));
    playlists[1].add_track(TrackModel("Third", QUrl("https://p.scdn.co/mp3-preview/4839b070015ab7d6de9fec1756e1f3096d908fba?cid=774b29d4f13844c495f206cafdad9c86")));

    playlists[2].add_track(TrackModel("Fourth", QUrl("https://p.scdn.co/mp3-preview/104ad0ea32356b9f3b2e95a8610f504c90b0026b?cid=774b29d4f13844c495f206cafdad9c86")));
    playlists[2].add_track(TrackModel("Fifth", QUrl("https://p.scdn.co/mp3-preview/b326e03624cb098d8387e17aa46669edac0d025a?cid=774b29d4f13844c495f206cafdad9c86")));



    populate_left_widget();

    connect(ui->add_song_button, &QAbstractButton::clicked, this, &HomePage::open_search_track_page);
    connect(ui->left_widget, &QListWidget::itemClicked, this, &HomePage::set_selected_playlist);
    connect(ui->right_widget, &QListWidget::itemClicked, this, &HomePage::set_selected_song);
}

int get_playlist_vector_id_from_item_row(int row)
{
   return row - 1;
}

void HomePage::set_selected_playlist(QListWidgetItem *item)
{
    int selected_id = ui->left_widget->row(item);

    /* First row is fixed (PLAYLISTS label) and therefore is not a valid playlist */
    if (selected_id != 0) {
        _selected_playlist_id = std::make_shared<int>(get_playlist_vector_id_from_item_row(selected_id));

        /* It's necessary to decrease by one to eliminate the fixed row (first) */
        populate_right_widget(*_selected_playlist_id);
    } else {
        _selected_playlist_id = nullptr;
    }
}

void HomePage::set_selected_song(QListWidgetItem *item)
{
    _selected_song_id = std::make_shared<int>(ui->right_widget->row(item));

    if (_selected_playlist_id) {
        play_song(playlists[*_selected_playlist_id].tracks()[*_selected_song_id]);
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

void HomePage::play_song(TrackModel track)
{
    _player.setMedia(track.url());
    _player.play();
}

void HomePage::open_search_track_page(bool)
{
    Search search;
    search.setModal(true);
    search.exec();
}

HomePage::~HomePage()
{
    delete ui;
}
