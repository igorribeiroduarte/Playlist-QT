#include "createplaylist.h"
#include "ui_createplaylist.h"

CreatePlaylist::CreatePlaylist(std::vector<PlaylistModel> *playlists, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlaylist)
{
    ui->setupUi(this);

    _playlist_ptr = playlists;

    connect(ui->pushButton, &QPushButton::clicked, this, &CreatePlaylist::create);
}

void CreatePlaylist::create(bool)
{
    if (ui->lineEdit->text().size() != 0) {
        _playlist_ptr->push_back(ui->lineEdit->text());
        close();
    }
}

CreatePlaylist::~CreatePlaylist()
{
    delete ui;
}
