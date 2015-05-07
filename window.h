#pragma once

#include <QSystemTrayIcon>
#include <QDialog>
#include <libusb.h>


QT_BEGIN_NAMESPACE
class ACtion;
QT_END_NAMESPACE

class Window : public QDialog
{
    Q_OBJECT
public:
    Window();

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

private slots:
    void showMessage();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

private:
    QAction* quitAction;

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
};

