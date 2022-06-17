#include "tagwriterwindow.h"
#include "ui_tagwriterwindow.h"

TagWriterWindow::TagWriterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TagWriterWindow)
{
    ui->setupUi(this);
}

TagWriterWindow::~TagWriterWindow()
{
    delete ui;
}
