#ifndef TAGWRITERWINDOW_H
#define TAGWRITERWINDOW_H

#include <QMainWindow>

namespace Ui {
class TagWriterWindow;
}

class TagWriterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TagWriterWindow(QWidget *parent = nullptr);
    ~TagWriterWindow();

private:
    Ui::TagWriterWindow *ui;
};

#endif // TAGWRITERWINDOW_H
