
#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>
#include <iostream>

using namespace std;

class Hardware
{

public:
    void allDisksNames()
    {
        //     var iterator: io_iterator_t = 0
        // let matching: CFDictionary = IOServiceMatching(kIOServicePlane)
        // IOServiceGetMatchingServices(kIOMasterPortDefault, matching, &iterator)
        // var child: io_object_t = IOIteratorNext(iterator)

        // while child != 0
        // {
        //     if let bsdName = IORegistryEntryCreateCFProperty(child, self.bsdNameKey as CFString, kCFAllocatorDefault, IOOptionBits(kIORegistryIterateRecursively))
        //     {
        //         Swift.print(bsdName)
        //     }

        //     child = IOIteratorNext(iterator)
        // }
    }
    void aboutMac()
    {
        char buf[1024];
        unsigned buflen = 0;
        char line[256];
        FILE *sw_vers = popen("sw_vers", "r");
        while (fgets(line, sizeof(line), sw_vers) != NULL)
        {
            int l = snprintf(buf + buflen, sizeof(buf) - buflen, "%s", line);
            buflen += l;
            assert(buflen < sizeof(buf));
        }
        cout << "Mac" << line << "\n";
        pclose(sw_vers);
    }

    void read()
    {
        CFMutableDictionaryRef matchingDict;
        io_iterator_t iter;
        kern_return_t kr;
        io_service_t device;

        /* set up a matching dictionary for the class */
        matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
        if (matchingDict == NULL)
        {
            cout << "Unable to get matching info \n";
            // return -1; // fail
        }

        /* Now we have a dictionary, get an iterator.*/
        kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
        if (kr != KERN_SUCCESS)
        {
            cout << "Unable to get info \n";
            // return -1;
        }

        /* iterate */
        while ((device = IOIteratorNext(iter)))
        {
            /* do something with device, eg. check properties */
            /* ... */
            /* And free the reference taken before continuing to the next item */
            cout << "Device : " << device << "\n";
            IOObjectRelease(device);
        }

        /* Done, release the iterator */
        IOObjectRelease(iter);
        //    return 0;
    }
};