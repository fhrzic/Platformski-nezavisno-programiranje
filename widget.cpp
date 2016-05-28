#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QFileIconProvider>

Widget::Widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    osPrefix = getPrefix();
    if (osPrefix == "linux")
    {
        setShortcutKeysLinux();
        setIconsLinux();
    }

}

Widget::~Widget()
{
    delete ui;
}

QString Widget::getPrefix()
{
#ifdef _WIN32
    return "windows32";
#elif _WIN64
    return "windows64";
#elif __linux__
    return "linux";
#elif __unix || __unix__
    return "unix";
#elif __APPLE__ || __MACH__
    return "max";
#elif __FreeBSD__
    return "freebsd";
#else
    return "Other";
#endif
}


void Widget::on_OpenFileBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open text file"), "/home/", tr("Text Files (*.txt)"));
    std::cout<<fileName.toStdString()<<std::endl;

    QFile file(fileName);
    QString textFromFile;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream stream(&file);

        QString line;
        do {
            line = stream.readLine();
            textFromFile = textFromFile + line + '\n';
        } while(!line.isNull());

        file.close();
        ui->textEdit->setText(textFromFile);
    }

}

void Widget::on_CloseBtn_clicked()
{
    qApp->exit();
}

void Widget::on_SaveFileBtn_clicked()
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << ui->textEdit->toPlainText();

        file.close();
    }
}

void Widget::on_SaveAsFileBtn_clicked()
{
    QString saveFileName =  QFileDialog::getSaveFileName(this,
                                                         tr("Save file as"), "/home/", tr("Text Files (*.txt)"));
    saveFileName += ".txt";
    QFile file(saveFileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << ui->textEdit->toPlainText();

        file.close();
    }
}

void Widget::setIconsLinux()
{
    std::string search = QIcon::themeSearchPaths()[0].toStdString();
    std::string out = GetStdoutFromCommand("gsettings list-recursively | grep icon-theme ");
    std::vector <int> positions;
    for (int i = 0; i < out.size();++i)
        if (out[i] == '\'') positions.push_back(i);

    int p1 = positions[positions.size()-1], p2 = positions[positions.size()-2];
    p2=p2+1;
    std::string theme = out.substr(p2,p1-p2);
    search +="/";
    std::string path;

    path = "find "+search+theme+"/* -iname document-open.png | grep '32'";
    std::string icon_open = GetStdoutFromCommand(path);

    path = "find "+search+theme+"/* -iname document-save.png | grep '32'";
    std::string icon_save = GetStdoutFromCommand(path);

    path = "find "+search+theme+"/* -iname document-save-as.png | grep '32'";
    std::string icon_save_as = GetStdoutFromCommand(path);

    path = "find "+search+theme+"/* -iname exit.png | grep '32'";
    std::string icon_close = GetStdoutFromCommand(path);
    QIcon ico;

    QString icon_open_q = QString::fromStdString(icon_open);
    QString icon_save_q = QString::fromStdString(icon_save);
    QString icon_save_as_q = QString::fromStdString(icon_save_as);
    QString icon_close_q = QString::fromStdString(icon_close);
    icon_open_q = icon_open_q.left(icon_open_q.length() - 1);
    icon_save_q = icon_save_q.left(icon_save_q.length() - 1);
    icon_save_as_q = icon_save_as_q.left(icon_save_as_q.length() - 1);
    icon_close_q = icon_close_q.left(icon_close_q.length() - 1);

    ico.addPixmap(QPixmap(icon_save_as_q),QIcon::Normal,QIcon::Off);
    ui->SaveAsFileBtn->setIcon(ico);
    ui->SaveAsFileBtn->setIconSize(QSize(32,32));

    ico.addPixmap(QPixmap(icon_open_q),QIcon::Normal,QIcon::Off);
    ui->OpenFileBtn->setIcon(ico);
    ui->OpenFileBtn->setIconSize(QSize(32,32));

    ico.addPixmap(QPixmap(icon_close_q),QIcon::Normal,QIcon::Off);
    ui->CloseBtn->setIcon(ico);
    ui->CloseBtn->setIconSize(QSize(32,32));


    ico.addPixmap(QPixmap(icon_save_q),QIcon::Normal,QIcon::Off);
    ui->SaveFileBtn->setIcon(ico);
    ui->SaveFileBtn->setIconSize(QSize(32,32));

}

std::string Widget::GetStdoutFromCommand(std::string cmd)
{

    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

void Widget::setShortcutKeysLinux()
{
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->OpenFileBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Ctrl+S"),this );
    QObject::connect(shortcut, SIGNAL(activated()), ui->SaveFileBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Ctrl+F4"),this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->CloseBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Alt+F4"),this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->CloseBtn, SIGNAL(clicked()));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N), this, SLOT(newDocument()));
}

void Widget::setShortcutKeysWindows()
{
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->OpenFileBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Ctrl+S"),this );
    QObject::connect(shortcut, SIGNAL(activated()), ui->SaveFileBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Shift+F12"),this );
    QObject::connect(shortcut, SIGNAL(activated()), ui->SaveFileBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Ctrl+F4"),this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->CloseBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("F12"),this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->SaveAsFileBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Alt+F4"),this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->CloseBtn, SIGNAL(clicked()));

    shortcut = new QShortcut(QKeySequence("Ctrl+W"),this);
    QObject::connect(shortcut, SIGNAL(activated()), ui->CloseBtn, SIGNAL(clicked()));

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(newDocument()));

}

void Widget::newDocument()
{
    ui->textEdit->clear();
    fileName="";
}
