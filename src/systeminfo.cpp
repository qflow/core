#include "systeminfo.h"
#include <QFile>
#include <QTextStream>
#include <QThread>
#ifdef Q_OS_LINUX
#include "sys/types.h"
#include "sys/sysinfo.h"
struct sysinfo memInfo;
#endif
#ifdef Q_OS_MAC
#include <sys/resource.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
struct rusage r_usage;
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
    PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
    QThread::sleep(1);
    PdhCollectQueryData(cpuQuery);
    PDH_FMT_COUNTERVALUE counterVal;
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    double result = counterVal.doubleValue;
    return result;
#endif
#ifdef Q_OS_MACOS
	return 0;
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
#ifdef Q_OS_MACOS
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics64_data_t vm_stats;
	
	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
		KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
										  (host_info64_t)&vm_stats, &count))
	{
		long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
		
		long long used_memory = ((int64_t)vm_stats.active_count +
								 (int64_t)vm_stats.inactive_count +
								 (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
		
		return free_memory + used_memory;
	} else {
		return 0;
	}
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
#ifdef Q_OS_MACOS
	vm_size_t page_size;
	mach_port_t mach_port;
	mach_msg_type_number_t count;
	vm_statistics64_data_t vm_stats;
	
	mach_port = mach_host_self();
	count = sizeof(vm_stats) / sizeof(natural_t);
	if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
		KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
										  (host_info64_t)&vm_stats, &count))
	{
		return (int64_t)vm_stats.free_count * (int64_t)page_size;
	} else {
		return 0;
	}
#endif
}
int SystemInfo::getThreadCount() const
{
    return QThread::idealThreadCount();
}
