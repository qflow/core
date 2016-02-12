#include "systeminfo.h"
#include <QFile>
#include <QTextStream>
#include <QThread>
#ifdef Q_OS_LINUX
#include "sys/types.h"
#include "sys/sysinfo.h"
struct sysinfo memInfo;
#endif
#ifdef Q_OS_WIN
#include "windows.h"
#include "TCHAR.h"
#include "pdh.h"
#endif

class SystemInfoPrivate
{
public:
    SystemInfoPrivate()
    {

    }
    ~SystemInfoPrivate()
    {

    }
};

SystemInfo::SystemInfo(QObject *parent) : QObject(parent), d_ptr(new SystemInfoPrivate())
{

}
SystemInfo::~SystemInfo()
{

}
double SystemInfo::getCpuUsage() const
{
#ifdef Q_OS_LINUX
    QFile statFile("/proc/stat");
    statFile.open(QFile::ReadOnly);
    QTextStream stream(&statFile);
    QString prevstat = stream.readLine();
    QThread::sleep(1);
    stream.seek(0);
    QString stat = stream.readLine();
    QStringList prevlist = prevstat.split(' ');
    QStringList list = stat.split(' ');

    int prevuser = prevlist[2].toInt();
    int prevnice = prevlist[3].toInt();
    int prevsystem = prevlist[4].toInt();
    int previdle = prevlist[5].toInt();
    int previowait = prevlist[6].toInt();
    int previrq = prevlist[7].toInt();
    int prevsoftirq = prevlist[8].toInt();
    int prevsteal = prevlist[9].toInt();

    int user = list[2].toInt();
    int nice = list[3].toInt();
    int system = list[4].toInt();
    int idle = list[5].toInt();
    int iowait = list[6].toInt();
    int irq = list[7].toInt();
    int softirq = list[8].toInt();
    int steal = list[9].toInt();

    int PrevIdle = previdle + previowait;
    int Idle = idle + iowait;

    int PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    int NonIdle = user + nice + system + irq + softirq + steal;

    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    int totald = Total - PrevTotal;
    int idled = Idle - PrevIdle;

    int diff = totald - idled;
    double CPU_Percentage = (double)diff/(double)totald;

    return CPU_Percentage;
#endif
#ifdef Q_OS_WIN
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
    QThread::sleep(1);
    PdhCollectQueryData(cpuQuery);
    PDH_FMT_COUNTERVALUE counterVal;
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    double result = counterVal.doubleValue;
    return result;
#endif
}
qulonglong SystemInfo::getTotalMem() const
{
#ifdef Q_OS_LINUX
    sysinfo(&memInfo);
    qulonglong total = memInfo.totalram * memInfo.mem_unit;
    return total;
#endif
#ifdef Q_OS_WIN
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    return totalPhysMem;
#endif
}
qulonglong SystemInfo::getFreeMem() const
{
#ifdef Q_OS_LINUX
    sysinfo(&memInfo);
    qulonglong free = memInfo.freeram * memInfo.mem_unit;
    return free;
#endif
#ifdef Q_OS_WIN
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG physMemFree = memInfo.ullAvailPhys;
    return physMemFree;
#endif
}
int SystemInfo::getThreadCount() const
{
    return QThread::idealThreadCount();
}
