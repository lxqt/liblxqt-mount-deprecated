/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - The Lightweight Desktop Environment
 * http://lxqt.org
 *
 * Copyright: 2012 Razor team
 *            2013-2014 LXQt team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef LXQTMOUNT_RZMOUNTPROVIDERS_H
#define LXQTMOUNT_RZMOUNTPROVIDERS_H

#include <QObject>
#include "lxqtmount.h"
#include <QtDBus/QDBusObjectPath>
class QDBusInterface;
class QDBusMessage;
class QDBusError;

namespace LxQt {

class MountProvider : public QObject
{
    Q_OBJECT
public:
    explicit MountProvider(QObject *parent = 0);
    virtual ~MountProvider() {}

    const MountDeviceList devices() const { return mDevices; }

    bool isValid() const { return mIsValid; }
    virtual void update()=0;

signals:
    void deviceAdded(LxQt::MountDevice *device);
    void deviceRemoved(LxQt::MountDevice *device);
    void deviceChanged(LxQt::MountDevice *device);

protected:
    MountDeviceList mDevices;
    bool mIsValid;
};



class UDiskMountDevice: public MountDevice
{
    Q_OBJECT
public:
    UDiskMountDevice(const QDBusObjectPath &path);
    bool update();
    QString udiskPath() const { return mUdiskPath; }

    virtual bool mount();
    virtual bool unmount();
    virtual bool eject();

private:
    QDBusInterface *mDbus;
    QString mUdiskPath;

    MediaType calcMediaType();
    QString calcLabel();
    bool calcIsExternal();
    QString calcIconName();

private slots:
    void dbusError(const QDBusError &err, const QDBusMessage &msg);
};



class UDiskProvider: public MountProvider
{
    Q_OBJECT
public:
    explicit UDiskProvider(QObject *parent = 0);

    virtual void update();

private slots:
    void dbusDeviceAdded(const QDBusObjectPath &path);
    void dbusDeviceRemoved(const QDBusObjectPath &path);
    void dbusDeviceChanged(const QDBusObjectPath &path);

private:
    QHash<QString, UDiskMountDevice*> mDevicesByPath;
    UDiskMountDevice *getDevice(const QDBusObjectPath &path) const;

    void addDevice(UDiskMountDevice *device);
    void delDevice(UDiskMountDevice *device);
};



class UDisks2MountDevice: public MountDevice
{
    Q_OBJECT
public:
    UDisks2MountDevice(const QDBusObjectPath &path);
    QDBusObjectPath path() { return mPath; }

    virtual bool mount();
    virtual bool unmount();
    virtual bool eject();

private:
    QDBusInterface *mBlockIface;
    QDBusInterface *mDriveIface;
    QDBusObjectPath mPath;

    MediaType calcMediaType();
    QString calcLabel();
    bool calcIsExternal();
    QString calcIconName();
    QStringList mountPoints() const;


private slots:
    void dbusError(const QDBusError &err, const QDBusMessage &msg);
    void aboutToMount();
    void aboutToUnmount();
    void aboutToEject();
    void update();
};



class UDisks2Provider: public MountProvider
{
    Q_OBJECT
public:
    explicit UDisks2Provider(QObject *parent = 0);

    virtual void update();

public slots:
    void dbusDeviceChanged(const QDBusObjectPath &path);

private slots:
    void dbusDeviceAdded(const QDBusObjectPath &path, const QVariantMap &map);
    void dbusDeviceRemoved(const QDBusObjectPath &path, const QStringList &list);

private:
    QHash<QString, UDisks2MountDevice*> mDevicesByPath;
    UDisks2MountDevice *getDevice(const QDBusObjectPath &path) const;

    void addDevice(UDisks2MountDevice *device);
    void delDevice(UDisks2MountDevice *device);
};

} // namespace LxQt

#endif // LXQTMOUNT_RZMOUNTPROVIDERS_H
