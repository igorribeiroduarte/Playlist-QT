#include "homepage.h"
#include "ui_homepage.h"
#include "playlistmodel.h"
#include "search.h"

#include <QResource>

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

    ui->left_widget->addItem("First");
    ui->left_widget->addItem("Second");
    ui->left_widget->addItem("Third");


    ui->right_widget->addItem("First song");
    ui->right_widget->addItem("Second song");
    ui->right_widget->addItem("Third song");

    /* Show the songs from the selected playlist */
    connect(ui->left_widget, &QListWidget::itemClicked, this, &HomePage::get_playlist);

    connect(ui->add_song_button, &QAbstractButton::clicked, this, &HomePage::open_search_track_page);
}

void HomePage::populate_playlist_widget()
{
    std::vector<PlaylistModel> list;
}

void HomePage::open_search_track_page(bool checked)
{
    Search search;
    search.setModal(true);
    search.exec();
}

void HomePage::get_playlist(QListWidgetItem *item)
{
    const int first_row = 0;
    /* First row is fixed */
  /*  if (item != ui->listWidget->item(first_row)) {

    }*/
}

HomePage::~HomePage()
{
    delete ui;
}
