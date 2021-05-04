// // gcc -Wall -framework Foundation -framework DiskArbitration disk_arbiter.m -o disk_arbiter
// /* @file disk_arbiter.m
//  * @author Jeremy W. Sherman
//  * @date 2009-10-03
//  *
//  * Demonstrates registering for disk appeared notifications from
//  * the DiskArbitration framework.
//  *
//  * Note that disk appeared notifications are delivered for all
//  * already-appeared disks at the time of registration, and then
//  * trickle in as the events actually happen thereafter.
//  */
// #include <Foundation/Foundation.h>
// #include <DiskArbitration/DiskArbitration.h>
// #include <signal.h>

// sig_atomic_t sShouldExit = 0;

// static void RegisterInterruptHandler(void);
// static void HandleInterrupt(int);

// static void OnDiskAppeared(DADiskRef disk, void *__attribute__((__unused__)));

// int
// main(void) {
//   CFStringRef const kDARunLoopMode = kCFRunLoopDefaultMode;

//   RegisterInterruptHandler();

//   // Set up session.
//   DASessionRef session = DASessionCreate(kCFAllocatorDefault);
//   DARegisterDiskAppearedCallback(session, NULL/*all disks*/, OnDiskAppeared, (void *)NULL);
//   DASessionScheduleWithRunLoop(session, CFRunLoopGetCurrent(), kDARunLoopMode);

//   // Run event loop.
//   printf("Starting...\n(Press Ctrl-C to exit.)\n\n");
//   const Boolean kAndReturnAfterHandlingSource = TRUE;
//   const CFTimeInterval kForOneSecond = 1.0;
//   while (!sShouldExit)
//     (void)CFRunLoopRunInMode(kCFRunLoopDefaultMode,
//                              kForOneSecond, kAndReturnAfterHandlingSource);

//   // Tear down and exit.
//   printf("\nExiting...\n");
//   DASessionUnscheduleFromRunLoop(session, CFRunLoopGetCurrent(), kDARunLoopMode);
//   CFRelease(session);
//   exit(EXIT_SUCCESS);
//   return EXIT_SUCCESS;
// }

// static void
// RegisterInterruptHandler(void) {
//   struct sigaction sigact;
//   sigact.sa_handler = HandleInterrupt;
//   (void)sigaction(SIGINT, &sigact, NULL/*discard previous handler*/);
// }

// static void
// HandleInterrupt(int __attribute__((__unused__)) signo) {
//   sShouldExit = 1;
//   RegisterInterruptHandler();
// }


// static void
// OnDiskAppeared(DADiskRef disk, void *__attribute__((__unused__)) ctx) {
//   printf("Lo, a disk appears!\n");
//   CFShow(disk);
// }