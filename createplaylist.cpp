#include "createplaylist.h"
#include "ui_createplaylist.h"
#include "init_db.h"

CreatePlaylist::CreatePlaylist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePlaylist)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &CreatePlaylist::create);
}

void CreatePlaylist::create(bool)
{
    if (ui->lineEdit->text().size() != 0) {
        add_playlist(PlaylistModel(ui->lineEdit->text()));
        close();
    }
}

CreatePlaylist::~CreatePlaylist()
{
    delete ui;
}
