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

signals:
    void deactivateChanged(bool);

private slots:
    void showMessage();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void deactivate(bool);

private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

private:
    QAction* quitAction;
    QAction* deactivateAction;

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
};

