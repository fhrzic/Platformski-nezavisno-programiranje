#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QStyle>
#include <string.h>
#include <QRegularExpressionMatch>
#include <QShortcut>

namespace Ui {
class Widget;
}

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_SaveAsFileBtn_clicked();
    void on_SaveFileBtn_clicked();
    void on_CloseBtn_clicked();
    void on_OpenFileBtn_clicked();
    void newDocument();

private:
    Ui::Widget *ui;
    QString fileName;
    QString getPrefix();
    void setIconsLinux();
    void setIconsWindows();
    void setShortcutKeysLinux();
    void setShortcutKeysWindows();

    QString osPrefix;
    std::string GetStdoutFromCommand(std::string cmd);

};

#endif // WIDGET_H
