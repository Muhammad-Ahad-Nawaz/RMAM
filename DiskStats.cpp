
#include <iostream>
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <DiskArbitration/DADisk.h>
// #include <IOKit/IOKitLib.h>
// #include <IOKit/IOMessage.h>
// #include <IOKit/IOCFPlugIn.h>
// #include <IOKit/usb/IOUSBLib.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include <sys/mount.h>

using namespace ::std;

class DiskStats
{

public:
    void allDrives()
    {
        struct statfs *mntbufp;
        int num_of_mnts = 0;
        int i;

        /* get our mount infos */
        num_of_mnts = getmntinfo(&mntbufp, MNT_WAIT);
        if (num_of_mnts == 0) /* no mounts returned, something is drastically wrong. */
        {
            fprintf(stderr, "No mounts???\n");
            // return false;
        }
        /* go though the mounts */
        for (i = 0; i < num_of_mnts; i++)
        {
            fprintf(stdout, "[INFO Mount: %i (%s on %s)]\n", i, mntbufp[i].f_mntfromname, mntbufp[i].f_mntonname);
        }
    }

    string getSerial(const string &path_in)
    {
        DASessionRef session = DASessionCreate(NULL);
        DADiskRef disk;

        // We need to find the mount point- strip last component:
        string path(path_in);
        size_t nPath = path_in.find_last_of("/");
        if (nPath != string::npos)
        {
            path = path_in.substr(0, nPath);
        }
        else
            return "";

        if (session)
        {
            CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)path.c_str(), path.length(), TRUE);
            disk = DADiskCreateFromVolumePath(NULL, session, url);
            CFRelease(url);
            if (disk)
            {
                // We found a mount point...
                io_service_t ioService = DADiskCopyIOMedia(disk);
                CFStringRef key = CFSTR("USB Serial Number");
                CFTypeRef sResult;

                sResult = IORegistryEntrySearchCFProperty(ioService, kIOServicePlane,
                                                          key, NULL,
                                                          kIORegistryIterateParents |
                                                              kIORegistryIterateRecursively);
                if (sResult)
                {
                    string sValue(CFStringRefToStdString((CFStringRef)sResult));

                    cerr << "GetSerialBetter got " << sValue << endl;
                    return sValue;
                }
            }
            else
            {
                // recurse down path until we find a mount point ...
                path = path.substr(0, path.length() - 1); // remove trailing "/"
                return getSerial(path);
            }
        }

        return "";
    }

    void read()
    {

        DADiskRef disk;
        CFDictionaryRef descDict;

        descDict = DADiskCopyDescription(disk);
        CFURLRef fspath = (CFURLRef)CFDictionaryGetValue(descDict, kDADiskDescriptionVolumePathKey);
        CFShow(fspath);
        // char buf[MAXPATHLEN];
        // long long size = CFDictionaryGetCount(descDict);
        // char buf[size];
        // if (CFURLGetFileSystemRepresentation(fspath, false, (UInt8 *)buf, sizeof(buf))) {
        //  printf("Disk %s mounted at %s\n",
        //      DADiskGetBSDName(disk),
        //     buf);

        // /* Print the complete dictionary for debugging. */
        // CFShow(descDict);
        // } else {
        //     /* Something is *really* wrong. */

        // }
    }

    void read3()
    {
        struct statvfs diskData;
        statvfs("/", &diskData);
        //     unsigned long long available = ((diskData.f_favail + diskData.f_bavail) * diskData.f_frsize) / (1024 * 1024 * 1024);
        //  std::cout << "Free Space : " << available << std::endl;

        float f_bavail = diskData.f_bavail;
        float f_frsize = diskData.f_frsize;
        float available3 = f_bavail * f_frsize / (1024 * 1024 * 1024);
        float capacity = f_frsize / (1024 * 1024 * 1024);
        std::cout << "Free Space : " << available3 << std::endl;
        std::cout << "Free Space : " << diskData.f_bfree << std::endl;
        std::cout << " capacity : " << capacity << std::endl;

        //  struct statvfs diskData2;
        // statvfs("/", &diskData2);
        // unsigned long available2 = diskData2.f_bavail * diskData2.f_bsize;
        // printf("Free Space : %luG\n", rounddiv(available2, 1024*1024*1024));
    }

    void partitions()
    {
        DADiskRef disk;
        CFDictionaryRef descDict;
        DASessionRef session = DASessionCreate(NULL);

        std::string mountPoint = "/";
        CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)mountPoint.c_str(), mountPoint.length(), TRUE);
        CFShow(url);
        disk = DADiskCreateFromVolumePath(NULL, session, url);
        if (disk)
        {
            DADiskRef wholedisk = DADiskCopyWholeDisk(disk);
            descDict = DADiskCopyDescription(wholedisk);
            if (descDict)
            {
                // CFTypeRef value = (CFTypeRef)CFDictionaryGetValue(descDict,
                //                                                   CFSTR("DAVolumeUUID"));
                // CFStringRef strValue = CFStringCreateWithFormat(NULL, NULL,
                //                                                 CFSTR("%@"), value);
                // printf("Volume UUID: %s\n" ,CFStringGetCStringPtr(strValue, kCFStringEncodingMacRoman));
                CFDictionaryApplyFunction(descDict, printKeys, NULL);
            }
        }
    }

    void read2()
    {
        DADiskRef disk;
        CFDictionaryRef descDict;
        DASessionRef session = DASessionCreate(NULL);

        std::string mountPoint = "/";
        CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *)mountPoint.c_str(), mountPoint.length(), TRUE);
        CFShow(url);
        disk = DADiskCreateFromVolumePath(NULL, session, url);
        if (disk)
        {
            descDict = DADiskCopyDescription(disk);
            if (descDict)
            {
                CFTypeRef value = (CFTypeRef)CFDictionaryGetValue(descDict,
                                                                  CFSTR("DAVolumeUUID"));
                CFStringRef strValue = CFStringCreateWithFormat(NULL, NULL,
                                                                CFSTR("%@"), value);
                printf("Volume UUID: %s\n", CFStringGetCStringPtr(strValue, kCFStringEncodingMacRoman));
                printf("Disk Name: %s\n", DADiskGetBSDName(disk));
                printf("Disk Id: %lu\n", DADiskGetTypeID());

                CFTypeRef volumeNameValue = (CFTypeRef)CFDictionaryGetValue(descDict,
                                                                            kDADiskDescriptionVolumeNameKey);
                CFStringRef volumeName = CFStringCreateWithFormat(NULL, NULL,
                                                                  CFSTR("%@"), volumeNameValue);
                printf("Volume name: %s \n", CFStringGetCStringPtr(volumeName, kCFStringEncodingMacRoman));
                // DAVolumeName
                //  printf("Disk IO Media: %u\n", DADiskCopyIOMedia(disk));

                //  CFTypeRef value2 = (CFTypeRef)CFDictionaryGetValue(descDict,
                //                                                   CFSTR("DAMediaEncrypted"));
                // CFStringRef strValue2 = CFStringCreateWithFormat(NULL, NULL,
                //                                                 CFSTR("%@"), value2);
                // printf("Volume UUID: %s\n" ,CFStringGetCStringPtr(strValue2, kCFStringEncodingMacRoman));
                //  printf("Disk IO Media: %u\n", DADiskCopyIOMedia(disk));

                // print whole dictionary
                // CFDictionaryApplyFunction(descDict, printKeys, NULL);
                ///
                CFRelease(strValue);
                CFRelease(descDict);
            }
            CFRelease(disk);
        }

        CFRelease(url);
        CFRelease(session);
    }
    string CFStringRefToStdString(const CFStringRef pCFStringRef)
    {
        const char *pCStr = NULL;

        string sRet;

        if (pCFStringRef)
        {
            pCStr = CFStringGetCStringPtr(pCFStringRef, kCFStringEncodingMacRoman);
            if (pCStr)
                sRet.assign(pCStr);
        }
        return sRet;
    }

    static void printKeys(const void *key, const void *value, void *context)
    {
        CFShow(key);
        CFShow(value);
    }

    unsigned long rounddiv(unsigned long num, unsigned long divisor)
    {
        return (num + (divisor / 2)) / divisor;
    }
};
