#include "search.h"
#include "ui_search.h"

#include "player.h"
#include "spotify.h"
#include "init_db.h"

#include <QUrl>

Search::Search(std::shared_ptr<PlaylistModel> playlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);

    ui->commandLinkButton->setText("Add song to playlist " + playlist->name());

    connect(ui->commandLinkButton, &QCommandLinkButton::clicked, this, [this, playlist](bool){
        if (ui->listWidget->currentItem() != nullptr) {
            TrackModel track(ui->listWidget->currentItem()->text(), ui->listWidget->currentItem()->data(Qt::UserRole).value<QUrl>(), playlist->id());
            add_track(track);
            qInfo() << ui->listWidget->currentItem()->text();
        }
    });

    connect(ui->listWidget, &QListWidget::itemClicked, [](QListWidgetItem *item) { Player::play_song(item->data(Qt::UserRole).value<QUrl>()); });

    connect(ui->lineEdit, &QLineEdit::textChanged, &Spotify::search_track);
    connect(&Spotify::get_instance(), &Spotify::ready_to_populate, this, &Search::populate);
}

void Search::populate(std::vector<TrackModel> tracks)
{
    ui->listWidget->clear();
    for (auto &it : tracks) {
        QListWidgetItem *item = new QListWidgetItem(it.name());
        item->setData(Qt::UserRole, it.url());
        ui->listWidget->addItem(item);
    }
}

Search::~Search()
{
    Player::stop_song();
    delete ui;
}
