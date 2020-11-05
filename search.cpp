#include "search.h"
#include "ui_search.h"

#include "player.h"
#include "spotify.h"

#include <QUrl>

Search::Search(Spotify *spotify, PlaylistModel *playlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);

    ui->commandLinkButton->setText("Add song to playlist " + playlist->name());

    connect(ui->commandLinkButton, &QCommandLinkButton::clicked, this, [this, playlist](bool checked){
        if (ui->listWidget->currentItem() != nullptr) {
            playlist->add_track(TrackModel(ui->listWidget->currentItem()->text(), ui->listWidget->currentItem()->data(Qt::UserRole).value<QUrl>()));
            qInfo() << ui->listWidget->currentItem()->text();
        }
    });

    connect(ui->listWidget, &QListWidget::itemClicked, [](QListWidgetItem *item) { Player::play_song(item->data(Qt::UserRole).value<QUrl>()); });


    connect(ui->lineEdit, &QLineEdit::textChanged, spotify, &Spotify::search_track);
    connect(spotify, &Spotify::ready_to_populate, this, &Search::populate);
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
    delete ui;
}
