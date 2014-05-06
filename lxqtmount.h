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


#ifndef LXQTMOUNT_LXQTMOUNT_H
#define LXQTMOUNT_LXQTMOUNT_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include <QtGlobal>

#ifdef COMPILE_LIBLXQT_MOUNT
#define LIBLXQT_MOUNT_API Q_DECL_EXPORT
#else
#define LIBLXQT_MOUNT_API Q_DECL_IMPORT
#endif

#define BOOL_SETTER(MEMBER) { bool res=(value != MEMBER); MEMBER = value; return res; }

namespace LxQt {

class MountProvider;
class LIBLXQT_MOUNT_API MountDevice: public QObject
{
    Q_OBJECT
public:
    enum MediaType {
        MediaTypeUnknown,
        MediaTypeDrive,
        MediaTypePartition,
        MediaTypeFdd,
        MediaTypeOptical
    };

    virtual bool mount() = 0;
    virtual bool unmount() = 0;
    virtual bool eject() = 0;

    QString devFile() const { return mDevFile; }
    QString label() const { return mLabel; }
    QString vendor() const { return mVendor; }
    QString model() const { return mModel; }
    QString fileSystem() const { return mFileSystem; }
    QString mountPath() const { return mMountPath; }
    QString iconName() const { return mIconName; }


    qulonglong size() const { return mSize; }
    MediaType mediaType() const { return mMediaType; }

    bool isValid() const { return mIsValid; }
    bool isExternal() const { return mIsExternal; }
    bool isMounted() const { return mIsMounted; }
    bool isEjectable() const { return mIsEjectable; }

    static QString sizeToString(qulonglong size);
signals:
     void changed();
     void error(const QString &msg);
     void mounted();
     void unmounted();

protected:
    explicit MountDevice();

    bool setDevFile(const QString &value)   BOOL_SETTER(mDevFile)
    bool setLabel(const QString &value)     BOOL_SETTER(mLabel)
    bool setVendor(const QString &value)    BOOL_SETTER(mVendor)
    bool setModel(const QString &value)     BOOL_SETTER(mModel)
    bool setFileSystem(const QString &value) BOOL_SETTER(mFileSystem)
    bool setMountPath(const QString &value) BOOL_SETTER(mMountPath)
    bool setIconName(const QString &value)  BOOL_SETTER(mIconName)

    bool setSize(qulonglong value)      BOOL_SETTER(mSize)
    bool setMediaType(MediaType value)  BOOL_SETTER(mMediaType)

    bool setIsValid(bool value)         BOOL_SETTER(mIsValid)
    bool setIsExternal(bool value)      BOOL_SETTER(mIsExternal)
    bool setIsMounted(bool value)       BOOL_SETTER(mIsMounted)
    bool setIsEjectable(bool value)     BOOL_SETTER(mIsEjectable)

    QString mDevFile;
    QString mLabel;
    QString mVendor;
    QString mModel;
    QString mFileSystem;
    QString mMountPath;
    QString mIconName;

    qulonglong mSize;
    MediaType mMediaType;

    bool mIsValid;
    bool mIsExternal;
    bool mIsMounted;
    bool mIsEjectable;
private:
    Q_DISABLE_COPY(MountDevice)


};

typedef QList<MountDevice*> MountDeviceList;




class LIBLXQT_MOUNT_API MountManager : public QObject
{
    Q_OBJECT
public:
    explicit MountManager(QObject *parent = 0);
    virtual ~MountManager();

    const MountDeviceList devices() const;

public slots:
    void update();

signals:
    void deviceAdded(LxQt::MountDevice *device);
    void deviceRemoved(LxQt::MountDevice *device);
    void deviceChanged(LxQt::MountDevice *device);

private:
    MountProvider *mProvider;

};

} // namespace LxQt

LIBLXQT_MOUNT_API QDebug operator<<(QDebug dbg, const LxQt::MountDevice& device);
LIBLXQT_MOUNT_API QDebug operator<<(QDebug dbg, const LxQt::MountDevice* const device);

#endif // LXQTMOUNT_LXQTMOUNT_H
