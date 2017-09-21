#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QFileInfoList>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QSignalMapper>

namespace Ui {
class mainMenu;
}

class mainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit mainMenu(QWidget *parent = 0);
    ~mainMenu();
    void findImages(QString path);
    void imageButtonsMenu();

protected slots:
    void setImagePath(QString path);
    void prevBtnPush();
    void nextBtnPush();
    void okBtnPush();


private:
    Ui::mainMenu *ui;

    QFileInfoList fileList;
    QFileInfoList tmpList;
    QFileInfo     fileInfo;

    // Menu
    QLabel *title;
    int     curPage;
    int     maxPage;
    QLabel *pageNum;

    // Image Button
    int                  numImBtn;
    QList<QPushButton *> btnList;
    QSignalMapper       *btnMapper;

    QString imPathInfo;
};

#endif // MAINMENU_H
