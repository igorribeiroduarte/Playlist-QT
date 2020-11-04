#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:
    Ui::HomePage *ui;
    void populate_playlist_widget();

private slots:
    void get_playlist(QListWidgetItem *item);
    void open_search_track_page(bool checked);
};

#endif // HOMEPAGE_H
