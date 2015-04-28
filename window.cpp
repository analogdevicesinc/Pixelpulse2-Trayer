#include <QtWidgets>
#include "window.h"

// http://qt-project.org/doc/qt-4.8/desktop-systray.html
Window::Window()
{
    createActions();
    createTrayIcon();
	auto icon = QIcon(":/icons/professor.png");
    trayIcon->setIcon(icon);
    trayIcon->show();
	// use OS-specific window icon set by pp2trayer.pro
    //setWindowIcon(icon);
}

void Window::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) {
        QMessageBox::information(this, tr("SysTray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void Window::showMessage()
{
}

void Window::messageClicked()
{
    QMessageBox::information(this, tr("SysTray"),
                             tr("Sorry, I alreaady gave what help I could."));
}


void Window::createActions()
{

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}
