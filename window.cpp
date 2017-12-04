#include <QtWidgets>
#include "window.h"

// http://qt-project.org/doc/qt-4.8/desktop-systray.html
Window::Window():
settings("Pixelpulse2-Trayer","Pixelpulse2-Trayer")
{
    createActions();
    createTrayIcon();
	auto icon = QIcon(":/icons/professor.png");
    trayIcon->setToolTip("Pixelpulse2 - Trayer");
    trayIcon->setIcon(icon);
    trayIcon->show();
    settings.setDefaultFormat(QSettings::defaultFormat());
	// use OS-specific window icon set by pp2trayer.pro
    //setWindowIcon(icon);
}

void Window::setDisabled(PP2Wrapper &pp2wrapper){
    bool deactiv = settings.value("deactivate").toBool();
    deactivateAction->setChecked(deactiv);
    pp2wrapper.setDisabled(deactiv);
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

void Window::deactivate(bool status)
{
    emit deactivateChanged(status);
    settings.setValue("deactivate", status);
    settings.sync();
}


void Window::createActions()
{

    quitAction = new QAction(tr("&Quit"), this);

    deactivateAction = new QAction(tr("&Deactivate"), this);
    deactivateAction->setCheckable(true);

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(deactivateAction, SIGNAL(triggered(bool)),
        this, SLOT(deactivate(bool)));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);
    trayIconMenu->addAction(deactivateAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}
