/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
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

#include "lxqtmount.h"
#include "rzmountproviders.h"
#include <QtCore/QDebug>

namespace LxQt {

MountDevice::MountDevice():
    QObject(0),
    mIsValid(false),
    mIsExternal(false),
    mIsMounted(false),
    mIsEjectable(false)
{
}

QString MountDevice::sizeToString(qulonglong size)
{
    double n;
    n = size / (1024.0 * 1024 * 1024);
    if (n >= 1.0)
        return QObject::tr("%1 GB").arg(n, 0, 'f', 1);

    n = size / (1024.0 * 1024);
    if (n >= 1.0)
        return QObject::tr("%1 MB").arg(n, 0, 'f', 1);

    n = size / (1024.0);
    if (n >= 1.0)
        return QObject::tr("%1 kB").arg(n, 0, 'f', 1);

    return QObject::tr("%1 B").arg(size);
}


MountManager::MountManager(QObject *parent):
    QObject(parent),
    mProvider(0)
{
    mProvider = new UDisks2Provider(this);
    if (!mProvider->isValid())
    {              
        delete mProvider;
        mProvider = 0;

        mProvider = new UDiskProvider(this);
        if (!mProvider->isValid())
        {
            delete mProvider;
            mProvider = 0;
        }
    }

    if (!mProvider)
        return;


    update();

    connect(mProvider, SIGNAL(deviceAdded(MountDevice*)),
                 this, SIGNAL(deviceAdded(MountDevice*)));

    connect(mProvider, SIGNAL(deviceChanged(MountDevice*)),
                 this, SIGNAL(deviceChanged(MountDevice*)));

    connect(mProvider, SIGNAL(deviceRemoved(MountDevice*)),
                 this, SIGNAL(deviceRemoved(MountDevice*)));
}


MountManager::~MountManager()
{
    delete mProvider;
}


void MountManager::update()
{
    if (mProvider)
        mProvider->update();
    else
        qDebug() << "MountDeviceList MountManager::update() no valid provider in use";

}


const MountDeviceList MountManager::devices() const
{
    if (mProvider)
    {
        //qDebug() << "MountManager::devices" << mProvider->devices();
        return mProvider->devices();
    }
    else
    {
        qWarning() << "MountDeviceList MountManager::devices() no valid provider in use";
        return MountDeviceList();
    }
}

} // namespace LxQt

QDebug operator<<(QDebug dbg, const LxQt::MountDevice &device)
{
    dbg << device.devFile();

    switch (device.mediaType())
    {
    case LxQt::MountDevice::MediaTypeUnknown:    dbg<<"Type: MediaTypeUnknown";  break;
    case LxQt::MountDevice::MediaTypeDrive:      dbg<<"Type: MediaTypeDrive";    break;
    case LxQt::MountDevice::MediaTypePartition:  dbg<<"Type: MediaTypePartition";break;
    case LxQt::MountDevice::MediaTypeFdd:        dbg<<"Type: MediaTypeFdd";      break;
    case LxQt::MountDevice::MediaTypeOptical:    dbg<<"Type: MediaTypeOptical";  break;
    default:                                    dbg<<"Type: "<<device.mediaType();break;
    }
    dbg << "Label: " << device.label();
    dbg << "Mount path:" << device.mountPath();
    return dbg.space();
}


QDebug operator<<(QDebug dbg, const LxQt::MountDevice *device)
{
    return operator<<(dbg, *device);
}
