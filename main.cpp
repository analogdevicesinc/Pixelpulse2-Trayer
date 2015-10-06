#include <QtGui>
#include <QMessageBox>
#include <QApplication>
#include <QSystemTrayIcon>
#include <iostream>
#include <thread>
#include <libusb.h>
#include <string.h>
#include <sys/time.h>

#include "window.h"
#include "pixelpulse2_wrap.h"

using std::cerr;
using std::endl;

libusb_context* m_usb_cx;
bool m_usb_thread_loop = true;

extern "C" int LIBUSB_CALL hotplug_callback_usbthread(
    libusb_context *ctx, libusb_device *device, libusb_hotplug_event event, void *user_data);

extern "C" int LIBUSB_CALL hotplug_callback_usbthread(
    libusb_context *ctx, libusb_device *device, libusb_hotplug_event event, void *user_data) {
    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(device, &desc);
    if (r != 0) {
        cerr << "Error " << r << "in get_device_descriptor" << endl;
        return -1;
    }

    (void) ctx;
    PP2Wrapper *pp2wrapper = (PP2Wrapper *)(user_data);
    if ((event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) && (desc.idVendor == 0x064B && desc.idProduct == 0x784C))
        pp2wrapper->launchPixelpulse2IfNotRunning();
    return 0;
}


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(pp2trayer);

    QApplication app(argc, argv);
    Window window;
    PP2Wrapper pp2wrapper;
    if (int r = libusb_init(&m_usb_cx) != 0) {
        QMessageBox::critical(0, QObject::tr("SysTray"), QObject::tr("I couldn't initialise libusb on this system."));
        return 1;
    }

    if (libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
        cerr << "Using libusb hotplug" << endl;
        if (int r = libusb_hotplug_register_callback(NULL,
            (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
            (libusb_hotplug_flag) 0,
            LIBUSB_HOTPLUG_MATCH_ANY,
            LIBUSB_HOTPLUG_MATCH_ANY,
            LIBUSB_HOTPLUG_MATCH_ANY,
            hotplug_callback_usbthread,
            (void*)(&pp2wrapper),
            NULL
        ) != 0) {
        cerr << "libusb hotplug cb reg failed: " << r << endl;
    };
    } else {
        QMessageBox::critical(0, QObject::tr("SysTray"), QObject::tr("I couldn't detect libusb hotplug support on this system."));
        return 1;
    }
    static timeval t;
    t.tv_sec = 0;
    t.tv_usec = 100000;
    auto m_usb_thread = std::thread([=]() {
        while(m_usb_thread_loop) {
            libusb_handle_events_timeout_completed(m_usb_cx, &t, 0);
        }
    });

    if (getenv("LIBUSB_DEBUG")) {
        libusb_set_debug(m_usb_cx, 4);
    }

    if(!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("SysTray"), QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);
    app.exec();
    m_usb_thread_loop = false;
    libusb_exit(m_usb_cx);
    m_usb_thread.join();
    return 0;
}
