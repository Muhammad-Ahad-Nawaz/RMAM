
#include <iostream>

#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>

#include <mach/mach_error.h>
#include <mach/vm_map.h>

#include <sys/sysctl.h>
class RAM {
    public:

    double totalRAMSize() {
        //, HW_USERMEM, HW_PHYSMEM
         int mib [] = { CTL_HW, HW_MEMSIZE};
        int64_t value = 0;
        size_t length = sizeof(value);

        if(-1 == sysctl(mib, 2, &value, &length, NULL, 0)){
        // An error occurred
            return 0;
        }
        // Physical memory is now in value
        //  printf("RAM total: %lld\n", value / 1024 / 1024 / 1024);
         return value;
    }

void read2() {
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    vm_statistics_data_t vmstat;
    if(KERN_SUCCESS != host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count)){
    // An error occurred
        printf("An error occurred");
    }
    double total = totalRAMSize();//vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count;
    double wired = vmstat.wire_count; /// total;
    double active = vmstat.active_count / total;
    double inactive = vmstat.inactive_count / total;
    double free = vmstat.free_count;// / total;
    printf("RAM total: %f, wired: %f, active: %f, inactive: %f,  free: %f\n", total, wired, active, inactive, free);
}
   
    void read() {
    double totalSize = totalRAMSize();
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO64,
                                        (host_info64_t)&vm_stats, &count))
    {
        // int64_t free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

        // int64_t used_memory = ((int64_t)vm_stats.active_count +
        //                          (int64_t)vm_stats.inactive_count +
        //                          (int64_t)vm_stats.wire_count) *  (int64_t)page_size;

        // printf("free memory: %f\nused memory: %f\n", gigabytes(free_memory), gigabytes(used_memory));
    // alternative
        int64_t active = vm_stats.active_count * page_size;
        int64_t inactive = vm_stats.inactive_count * page_size;
        int64_t wired = vm_stats.wire_count *  page_size;
        int64_t speculative = vm_stats.speculative_count *  page_size;
        int64_t compressed = vm_stats.compressor_page_count *  page_size;
        int64_t purgeable = vm_stats.purgeable_count * page_size;
        int64_t external = vm_stats.external_page_count * page_size;

        int64_t used = active + inactive + speculative + wired + compressed - purgeable - external;
        //  printf("free memory: %lld\nused memory: %lld\n", free_memory, used);
        int64_t freeSpace = totalSize - used;//free_memory/1000/1000/1000;

        // printf("wired %f, compressed %f\n", wired, compressed);
        // printf("used %llu, freeSpace %llu\n", used, freeSpace);
        // printf("purgeable %lld, external %lld\n", purgeable, external);
        // printf("totalSize %f, free memory: %lld used memory: %lld\n", totalSize, freeSpace, used);
        printf("totalSize %f, free memory: %f used memory: %f\n", gigabytes(totalSize), gigabytes(freeSpace), gigabytes(used));

        int64_t  app =  used - wired - compressed;
        int64_t cache = purgeable + external;
        int64_t pressure = 100.0 * (wired + compressed) / totalSize;

        printf("app %f, cache: %f pressure: %llu\n", gigabytes(app), gigabytes(cache), pressure);
        printf("Wired %f, compressed: %f\n", gigabytes(wired), gigabytes(compressed));
       
        size_t int_size = sizeof(uint);
        int pressureLevel = 0;
        sysctlbyname("kern.memorystatus_vm_pressure_level", &pressureLevel, &int_size, NULL, 0);
        printf("Pressure Level: %d\n",pressureLevel);

        size_t string_size = sizeof(xsw_usage);
        xsw_usage swap = xsw_usage();
        sysctlbyname("vm.swapusage", &swap, &string_size, NULL, 0);
        printf("swapusage: %lu\n",string_size);
        
        printf("swapusage:in MB's total %.2f used %.2f available %.2f\n",megabytes(swap.xsu_total), megabytes(swap.xsu_used), megabytes(swap.xsu_avail));
    }
    }

    double kilobytes(int64_t bytes) {
        return bytes / 1024;
    }
    double megabytes(int64_t bytes) {
        return kilobytes(bytes) / 1024;
    }
    double gigabytes(int64_t bytes) {
        return megabytes(bytes) / 1024;
    }
     double terabytes(int64_t bytes) {
        return gigabytes(bytes) / 1024;
    }

};

