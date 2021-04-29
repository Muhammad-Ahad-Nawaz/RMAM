
#include <iostream>
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <DiskArbitration/DADisk.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <sys/stat.h>

//https://developer.apple.com/library/archive/documentation/DriversKernelHardware/Conceptual/DiskArbitrationProgGuide/ManipulatingDisks/ManipulatingDisks.html#//apple_ref/doc/uid/TP40009310-CH3-SW1
//https://stackoverflow.com/questions/32851066/volume-uuid-using-iokit-framework-from-os-x
class Disk {

    public :
    
    void read() {
        // HANDLE FindFirstVolumeW(LPWSTR lpszVolumeName, DWORD  cchBufferLength);
        printUUID();
    }

    void printUUID()
{
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
            CFRelease(strValue);
            CFRelease(descDict);
        }
        CFRelease(disk);
    }

    CFRelease(url);
    CFRelease(session);
}

// int fileSize(const char *add){
//     ifstream mySource;
//     mySource.open(add, ios_base::binary);
//     mySource.seekg(0,ios_base::end);
//     int size = mySource.tellg();
//     mySource.close();
//     return size;
// }

};

/*
#include <iostream>
#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include <DiskArbitration/DADisk.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <sys/stat.h>

using namespace::std;

// path_in is a filename on an usb disk
// This will fetch the serial for this usb disk
// provided apple does not change the usb serial key name from
// "USB Serial Number" to whatever.
//
// Usage : string sSerial = GetSerial("/Volumes/USBStick/MyDir/Myfile");
// Returns "" if file is not on a USB Disk
//

class Disk {

    public :

string  CFStringRefToStdString(const CFStringRef pCFStringRef)
{
    const char* pCStr = NULL;
    
    string sRet;
    
    if (pCFStringRef)
        {
        pCStr = CFStringGetCStringPtr(pCFStringRef, kCFStringEncodingMacRoman);
        if(pCStr)
            sRet.assign(pCStr);
        }
    return sRet;
}

string GetSerial(const string &path_in)
    {
    DASessionRef session = DASessionCreate(NULL);
    DADiskRef disk;

    // We need to find the mount point- strip last component:
    string path(path_in);
    size_t nPath = path_in.find_last_of("/");
    if(nPath != string::npos)
        {
        path=path_in.substr(0, nPath);
        }
    else
        return "";

    if (session)
        {
        CFURLRef url = CFURLCreateFromFileSystemRepresentation
        (NULL, (const UInt8 *)path.c_str(), path.length(), TRUE);
        disk = DADiskCreateFromVolumePath(NULL, session, url);
        CFRelease(url);
        if(disk)
            {
            // We found a mount point...
            io_service_t ioService = DADiskCopyIOMedia( disk );
            CFStringRef key = CFSTR("USB Serial Number");
            CFTypeRef sResult ;

            sResult = IORegistryEntrySearchCFProperty( ioService, kIOServicePlane,
                                                      key, NULL,
                                                      kIORegistryIterateParents | 
                                                      kIORegistryIterateRecursively ) ;
            if(sResult)
                {
                string sValue( CFStringRefToStdString((CFStringRef)sResult) ) ;

                cerr << "GetSerialBetter got " << sValue << endl;
                return sValue;
                }
            }
        else
            {
            // recurse down path until we find a mount point ...
            path = path.substr(0, path.length()-1); // remove trailing "/"
            return GetSerial(path);
            }
        }

    return "";
    }
};*/