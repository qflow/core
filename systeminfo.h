#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QObject>

class SystemInfoPrivate;
class SystemInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qulonglong totalMem READ getTotalMem)
    Q_PROPERTY(qulonglong freeMem READ getFreeMem)
    Q_PROPERTY(int threadCount READ getThreadCount)
public:
    explicit SystemInfo(QObject *parent = 0);
    ~SystemInfo();
Q_SIGNALS:

public Q_SLOTS:
    double getCpuUsage() const;
    qulonglong getTotalMem() const;
    qulonglong getFreeMem() const;
    int getThreadCount() const;
private:
    const QScopedPointer<SystemInfoPrivate> d_ptr;
    Q_DECLARE_PRIVATE(SystemInfo)
};

#endif // SYSTEMINFO_H
