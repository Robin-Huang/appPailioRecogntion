#include "mainMenu.h"
#include "ui_mainMenu.h"
#include "clientMenu.h"
#include <QDebug>
#include <QDir>
#include <QIcon>

mainMenu::mainMenu(QWidget *parent) : QWidget(parent), ui(new Ui::mainMenu)
{
    ui->setupUi(this);

    QString path = "D:/Robin/Picture/Papilionidae/Byasa polyeuctes termessus";
    path = "/sdcard/DCIM"; // android picture path

    curPage = 0;
    maxPage = 0;

    btnList.append(ui->imBtn1);
    btnList.append(ui->imBtn2);
    btnList.append(ui->imBtn3);
    btnList.append(ui->imBtn4);

    numImBtn = btnList.size();    // number of image buttons

    btnMapper = new QSignalMapper(this);

    ui->prevPage->setEnabled(false);
    ui->okButton->setEnabled(false);

    // Connection
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okBtnPush()));
    connect(ui->prevPage, SIGNAL(clicked()), this, SLOT(prevBtnPush()));
    connect(ui->nextPage, SIGNAL(clicked()), this, SLOT(nextBtnPush()));

    findImages(path);   // find images in direction
    imageButtonsMenu(); // create images seleection menu
}

void mainMenu::findImages(QString path)
{
    QDir dir(path);
    dir.setFilter(QDir::AllDirs|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time);

    // ********** Set File Extension ------->>
    QStringList nameFilters("*.jpg");
    nameFilters << "*.png" << "*.bmp" << "*.tif";

    // ********** Find Image Files --------->>
    fileList = dir.entryInfoList(nameFilters);

    // ********** Check Direction ---------->>
    int i = 0;
    while(i < fileList.size()){
        if(fileList.at(i).isDir()){
            dir.setPath(fileList.at(i).filePath());
            QFileInfoList tmpList = dir.entryInfoList(nameFilters);

            // Add new images info to list
            for(int j = 0; j < tmpList.size(); j++)
                fileList.append(tmpList.at(j));

            // Remove dir
            fileList.removeAt(i);
            continue;
        }
        i++;
    }

    // No Image
    if(fileList.size() != 0)
        maxPage = (fileList.size()-1) / numImBtn;

    // Only One Page
    if(maxPage == 0)
        ui->nextPage->setEnabled(false);
}

void mainMenu::imageButtonsMenu()
{
    int maxNumIcons = numImBtn;                 // maximum icon number in one page
    int startIndex  = curPage * maxNumIcons;    // first icon index in current page
    if(fileList.size() - startIndex < maxNumIcons)
        maxNumIcons = fileList.size() - startIndex;

    for(int i = 0; i < maxNumIcons; i++){
        // Image Path
        fileInfo = fileList.at(startIndex + i);

        // Icon Setting
        btnList[i]->setIcon(QIcon(fileInfo.filePath()));
        btnList[i]->setIconSize(QSize(200,200));
        btnList[i]->icon().setThemeName(fileInfo.filePath());

        btnList[i]->setEnabled(true);

        // Button Connnection
        connect(btnList[i], SIGNAL(clicked()), btnMapper, SLOT(map()));
        btnMapper->setMapping(btnList[i], btnList[i]->icon().themeName());
    }

    connect(btnMapper, SIGNAL(mapped(QString)), this, SLOT(setImagePath(QString)));
    ui->page->setText("< " + QString::number(curPage+1) + "/" + QString::number(maxPage+1) + " >");
}

void mainMenu::setImagePath(QString path)
{
    imPathInfo = path;
    ui->okButton->setEnabled(true);
}

void mainMenu::prevBtnPush()
{
    curPage--;

    if(curPage == 0)
        ui->prevPage->setEnabled(false);

    ui->nextPage->setEnabled(true);

    for(int i = 0; i < numImBtn; i++){
        btnList[i]->setIcon(QIcon());
        btnList[i]->setEnabled(false);
    }
    imageButtonsMenu();
}

void mainMenu::nextBtnPush()
{
    curPage++;

    if(curPage == maxPage)
        ui->nextPage->setEnabled(false);

    ui->prevPage->setEnabled(true);

    for(int i = 0; i < numImBtn; i++){
        btnList[i]->setIcon(QIcon());
        btnList[i]->setEnabled(false);
    }
    imageButtonsMenu();
}

void mainMenu::okBtnPush()
{
    // ********** Display Client Menu ------>>
    clientMenu* client = new clientMenu(imPathInfo);
    client->setAttribute(Qt::WA_ShowModal, true);
    client->show();
}

mainMenu::~mainMenu()
{
    delete ui;
}
