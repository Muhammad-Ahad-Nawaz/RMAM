
#include <iostream>
#include <string>
// #include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include <DiskArbitration/DADisk.h>
// #include <IOKit/IOKitLib.h>
// #include <IOKit/IOMessage.h>
// #include <IOKit/IOCFPlugIn.h>
// #include <IOKit/usb/IOUSBLib.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

class DiskStats {

    public :
    
    void read() {

    DADiskRef disk;
    CFDictionaryRef descDict;

    descDict = DADiskCopyDescription(disk);
    CFURLRef fspath = (CFURLRef)CFDictionaryGetValue(descDict, kDADiskDescriptionVolumePathKey);
 
    // char buf[MAXPATHLEN];
    long long size = CFDictionaryGetCount(descDict);
    char buf[size];
    if (CFURLGetFileSystemRepresentation(fspath, false, (UInt8 *)buf, sizeof(buf))) {
     printf("Disk %s mounted at %s\n",
         DADiskGetBSDName(disk),
        buf);
 
    /* Print the complete dictionary for debugging. */
    CFShow(descDict);
} else {
    /* Something is *really* wrong. */

}
    }
    void read3() {
        struct statvfs diskData;
        statvfs("/", &diskData);
    //     unsigned long long available = ((diskData.f_favail + diskData.f_bavail) * diskData.f_frsize) / (1024 * 1024 * 1024);
    //  std::cout << "Free Space : " << available << std::endl;

    float f_bavail = diskData.f_bavail; 
    float f_frsize = diskData.f_frsize;   
  float available3 = f_bavail * f_frsize / (1024 * 1024); 
    std::cout << "Free Space : " << available3 << std::endl;

     struct statvfs diskData2;
    statvfs("/", &diskData2);
    unsigned long available2 = diskData2.f_bavail * diskData2.f_bsize;
    printf("Free Space : %luG\n", rounddiv(available2, 1024*1024*1024));


    }

 void read2() {
     DADiskRef disk;
    CFDictionaryRef descDict;
    DASessionRef session = DASessionCreate(NULL);

    std::string mountPoint = "/";
    CFURLRef url = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8 *) mountPoint.c_str(), mountPoint.length(), TRUE);
    disk = DADiskCreateFromVolumePath(NULL, session, url);
    if (disk) {
        descDict = DADiskCopyDescription(disk);
        if (descDict) {
            CFTypeRef value = (CFTypeRef)CFDictionaryGetValue(descDict,
                                                              CFSTR("DAVolumeUUID"));
            CFStringRef strValue = CFStringCreateWithFormat(NULL, NULL,
                                                            CFSTR("%@"), value);
            printf("%s\n" ,CFStringGetCStringPtr(strValue, kCFStringEncodingMacRoman));
             printf("Disk Name: %s\n", DADiskGetBSDName(disk));

             
            //
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
static void printKeys (const void* key, const void* value, void* context) {
  CFShow(key);
  CFShow(value);
}

unsigned long rounddiv(unsigned long num, unsigned long divisor) {
    return (num + (divisor/2)) / divisor;
}


};



// int main() {
//   DiskStats disk;    // Create an object (this will call the constructor)

//     disk.read();
//   return 0;
// }
