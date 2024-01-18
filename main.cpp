/*
 
 
 - compile on APPLE:
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib

 To run on WIN32, the resource file is resource_file_windows.rc
 
 */



//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "main.h"
#include "lib.cpp"

/*
 
 notes:
 =======
 - error log on APPLE:
 
 -------------------------------------
 Translated Report (Full Report Below)
 -------------------------------------

 Process:               Thelemacus [1055]
 Path:                  /Users/USER/Desktop/Thelemacus.app/Contents/MacOS/Thelemacus
 Identifier:            org.wxwidgets.Thelemacus
 Version:               3.2 (3.2.0)
 Code Type:             ARM-64 (Native)
 Parent Process:        zsh [1044]
 Responsible:           Terminal [602]
 User ID:               501

 Date/Time:             2024-01-18 10:55:52.5773 +0100
 OS Version:            macOS 14.1 (23B2073)
 Report Version:        12
 Anonymous UUID:        18AD036A-AEA1-DD1F-6609-A69357777C39


 Time Awake Since Boot: 220 seconds

 System Integrity Protection: enabled

 Crashed Thread:        0

 Exception Type:        EXC_BAD_ACCESS (SIGKILL (Code Signature Invalid))
 Exception Codes:       UNKNOWN_0x32 at 0x000000010101c000
 Exception Codes:       0x0000000000000032, 0x000000010101c000

 Termination Reason:    Namespace CODESIGNING, Code 2 Invalid Page

 VM Region Info: 0x10101c000 is in 0x10101c000-0x101048000;  bytes after start: 0  bytes before end: 180223
       REGION TYPE                    START - END         [ VSIZE] PRT/MAX SHRMOD  REGION DETAIL
       mapped file                 10100c000-10101c000    [   64K] r--/rwx SM=COW  ...t_id=420bd0af
 --->  mapped file                 10101c000-101048000    [  176K] r-x/rwx SM=COW  ...t_id=42f713af
       VM_ALLOCATE (reserved)      101048000-101058000    [   64K] rw-/rwx SM=NUL  ...(unallocated)

 Thread 0 Crashed:
 0   dyld                                     0x181da42c4 dyld3::MachOFile::isMachO(Diagnostics&, unsigned long long) const + 40
 1   dyld                                     0x181d68308 dyld4::Loader::mapSegments(Diagnostics&, dyld4::RuntimeState&, char const*, unsigned long long, dyld4::Loader::CodeSignatureInFile const&, bool, dyld3::Array<dyld4::Loader::Region> const&, bool, bool, dyld4::Loader::FileValidationInfo const&) + 1080
 2   dyld                                     0x181d712bc invocation function for block in dyld4::JustInTimeLoader::makeJustInTimeLoaderDisk(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*) + 88
 3   dyld                                     0x181d70c50 dyld4::JustInTimeLoader::withRegions(dyld3::MachOFile const*, void (dyld3::Array<dyld4::Loader::Region> const&) block_pointer) + 292
 4   dyld                                     0x181d71200 invocation function for block in dyld4::JustInTimeLoader::makeJustInTimeLoaderDisk(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*) + 460
 5   dyld                                     0x181d77fcc dyld4::SyscallDelegate::withReadOnlyMappedFile(Diagnostics&, char const*, bool, void (void const*, unsigned long, bool, dyld4::FileID const&, char const*) block_pointer) const + 132
 6   dyld                                     0x181d71000 dyld4::JustInTimeLoader::makeJustInTimeLoaderDisk(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*) + 208
 7   dyld                                     0x181d65fe8 dyld4::Loader::makeDiskLoader(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*) + 172
 8   dyld                                     0x181d67584 invocation function for block in dyld4::Loader::getLoader(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&) + 2056
 9   dyld                                     0x181d66694 dyld4::Loader::forEachResolvedAtPathVar(dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, dyld4::ProcessConfig::PathOverrides::Type, bool&, void (char const*, dyld4::ProcessConfig::PathOverrides::Type, bool&) block_pointer) + 776
 10  dyld                                     0x181d53e24 dyld4::ProcessConfig::PathOverrides::forEachPathVariant(char const*, dyld3::Platform, bool, bool, bool&, void (char const*, dyld4::ProcessConfig::PathOverrides::Type, bool&) block_pointer) const + 1220
 11  dyld                                     0x181d661cc dyld4::Loader::forEachPath(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, void (char const*, dyld4::ProcessConfig::PathOverrides::Type, bool&) block_pointer) + 248
 12  dyld                                     0x181d669fc dyld4::Loader::getLoader(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&) + 808
 13  dyld                                     0x181d6ef2c invocation function for block in dyld4::JustInTimeLoader::loadDependents(Diagnostics&, dyld4::RuntimeState&, dyld4::Loader::LoadOptions const&) + 412
 14  dyld                                     0x181da5cd0 invocation function for block in dyld3::MachOFile::forEachDependentDylib(void (char const*, bool, bool, bool, unsigned int, unsigned int, bool&) block_pointer) const + 148
 15  dyld                                     0x181d4d2fc dyld3::MachOFile::forEachLoadCommand(Diagnostics&, void (load_command const*, bool&) block_pointer) const + 300
 16  dyld                                     0x181da5b04 dyld3::MachOFile::forEachDependentDylib(void (char const*, bool, bool, bool, unsigned int, unsigned int, bool&) block_pointer) const + 172
 17  dyld                                     0x181d6ec70 dyld4::JustInTimeLoader::loadDependents(Diagnostics&, dyld4::RuntimeState&, dyld4::Loader::LoadOptions const&) + 164
 18  dyld                                     0x181d517c0 dyld4::prepare(dyld4::APIs&, dyld3::MachOAnalyzer const*) + 1176
 19  dyld                                     0x181d50edc start + 1844


 Thread 0 crashed with ARM Thread State (64-bit):
     x0: 0x000000010101c000   x1: 0x000000016f66ddd0   x2: 0x000000000002c000   x3: 0x0000000000040012
     x4: 0x0000000000000003   x5: 0x0000000000000000   x6: 0x0000000000000000   x7: 0x0000000000000000
     x8: 0x00000001d82c5f6c   x9: 0x00000001d82c77d0  x10: 0x0000000000000012  x11: 0x0000000000000001
    x12: 0x0000000000000001  x13: 0x0800000000030000  x14: 0x00000000000097d0  x15: 0x000000016f66eb50
    x16: 0x00000000000000c5  x17: 0x0000000181d4ef90  x18: 0x0000000000000000  x19: 0x000000016f66ddd0
    x20: 0x0000000000000003  x21: 0x000000016f66ddd0  x22: 0x000000016f66d848  x23: 0x000000000003c000
    x24: 0x0000000000000000  x25: 0x000000016f66d5ec  x26: 0x0000000000000000  x27: 0x000000010101c000
    x28: 0x0000000000000030   fp: 0x000000016f66cfe0   lr: 0x7a4a000181d68308
     sp: 0x000000016f66cfd0   pc: 0x0000000181da42c4 cpsr: 0x20001000
    far: 0x000000010101c000  esr: 0x92000007 (Data Abort) byte read Translation fault

 Binary Images:
        0x100790000 -        0x10090bfff org.wxwidgets.Thelemacus (3.2) <afc972fb-7995-3f8a-9355-07916e033edb> /Users/USER/Desktop/Thelemacus.app/Contents/MacOS/Thelemacus
        0x181d4b000 -        0x181ddf317 dyld (*) <efab0dc4-ad82-3527-8267-158fa446599c> /usr/lib/dyld
                0x0 - 0xffffffffffffffff ??? (*) <00000000-0000-0000-0000-000000000000> ???

 External Modification Summary:
   Calls made by other processes targeting this process:
     task_for_pid: 0
     thread_create: 0
     thread_set_state: 0
   Calls made by this process:
     task_for_pid: 0
     thread_create: 0
     thread_set_state: 0
   Calls made by all processes on this machine:
     task_for_pid: 5
     thread_create: 0
     thread_set_state: 104

 VM Region Summary:
 ReadOnly portion of Libraries: Total=891.8M resident=0K(0%) swapped_out_or_unallocated=891.8M(100%)
 Writable regions: Total=12.4M written=0K(0%) resident=0K(0%) swapped_out=0K(0%) unallocated=12.4M(100%)

                                 VIRTUAL   REGION
 REGION TYPE                        SIZE    COUNT (non-coalesced)
 ===========                     =======  =======
 STACK GUARD                       56.0M        1
 Stack                             8176K        1
 VM_ALLOCATE (reserved)             160K        3         reserved VM address space (unallocated)
 __DATA                              27K        2
 __DATA_CONST                       134K        2
 __DATA_DIRTY                         7K        1
 __LINKEDIT                       889.7M        2
 __TEXT                            2116K        2
 dyld private memory               4384K        4
 mapped file                       21.2M       26
 ===========                     =======  =======
 TOTAL                            981.6M       44
 TOTAL, minus reserved VM space   981.4M       44



 -----------
 Full Report
 -----------

 {"app_name":"Thelemacus","timestamp":"2024-01-18 10:55:52.00 +0100","app_version":"3.2","slice_uuid":"afc972fb-7995-3f8a-9355-07916e033edb","build_version":"3.2.0","platform":1,"bundleID":"org.wxwidgets.Thelemacus","share_with_app_devs":0,"is_first_party":0,"bug_type":"309","os_version":"macOS 14.1 (23B2073)","roots_installed":0,"name":"Thelemacus","incident_id":"B9D5EEF8-6EDE-4DFB-9A12-68FEC7BBD249"}
 {
   "uptime" : 220,
   "procRole" : "Unspecified",
   "version" : 2,
   "userID" : 501,
   "deployVersion" : 210,
   "modelCode" : "Mac15,3",
   "coalitionID" : 726,
   "osVersion" : {
     "train" : "macOS 14.1",
     "build" : "23B2073",
     "releaseType" : "User"
   },
   "captureTime" : "2024-01-18 10:55:52.5773 +0100",
   "codeSigningMonitor" : 1,
   "incident" : "B9D5EEF8-6EDE-4DFB-9A12-68FEC7BBD249",
   "pid" : 1055,
   "translated" : false,
   "cpuType" : "ARM-64",
   "roots_installed" : 0,
   "bug_type" : "309",
   "procLaunch" : "2024-01-18 10:55:48.4834 +0100",
   "procStartAbsTime" : 5418229136,
   "procExitAbsTime" : 5516475408,
   "procName" : "Thelemacus",
   "procPath" : "\/Users\/USER\/Desktop\/Thelemacus.app\/Contents\/MacOS\/Thelemacus",
   "bundleInfo" : {"CFBundleShortVersionString":"3.2","CFBundleVersion":"3.2.0","CFBundleIdentifier":"org.wxwidgets.Thelemacus"},
   "storeInfo" : {"deviceIdentifierForVendor":"84B6408C-6E33-51E9-9A61-23E0EF6E6612","thirdParty":true},
   "parentProc" : "zsh",
   "parentPid" : 1044,
   "coalitionName" : "com.apple.Terminal",
   "crashReporterKey" : "18AD036A-AEA1-DD1F-6609-A69357777C39",
   "responsiblePid" : 602,
   "responsibleProc" : "Terminal",
   "codeSigningID" : "Thelemacus",
   "codeSigningTeamID" : "",
   "codeSigningFlags" : 587334144,
   "codeSigningValidationCategory" : 0,
   "codeSigningTrustLevel" : 4294967295,
   "instructionByteStream" : {"beforePC":"fyMD1fRPvqn9ewGp\/UMAkfMDAapfbADxiAAAVKEBAJAhODWRGwAAFA==","atPC":"CABAuemsn1LJ7q9yPwVIa+EBAFTJWZ9Sqd2\/cgoUQLkpBQARHwEJaw=="},
   "sip" : "enabled",
   "vmRegionInfo" : "0x10101c000 is in 0x10101c000-0x101048000;  bytes after start: 0  bytes before end: 180223\n      REGION TYPE                    START - END         [ VSIZE] PRT\/MAX SHRMOD  REGION DETAIL\n      mapped file                 10100c000-10101c000    [   64K] r--\/rwx SM=COW  ...t_id=420bd0af\n--->  mapped file                 10101c000-101048000    [  176K] r-x\/rwx SM=COW  ...t_id=42f713af\n      VM_ALLOCATE (reserved)      101048000-101058000    [   64K] rw-\/rwx SM=NUL  ...(unallocated)",
   "exception" : {"codes":"0x0000000000000032, 0x000000010101c000","rawCodes":[50,4311859200],"type":"EXC_BAD_ACCESS","signal":"SIGKILL (Code Signature Invalid)","subtype":"UNKNOWN_0x32 at 0x000000010101c000"},
   "termination" : {"flags":0,"code":2,"namespace":"CODESIGNING","indicator":"Invalid Page"},
   "vmregioninfo" : "0x10101c000 is in 0x10101c000-0x101048000;  bytes after start: 0  bytes before end: 180223\n      REGION TYPE                    START - END         [ VSIZE] PRT\/MAX SHRMOD  REGION DETAIL\n      mapped file                 10100c000-10101c000    [   64K] r--\/rwx SM=COW  ...t_id=420bd0af\n--->  mapped file                 10101c000-101048000    [  176K] r-x\/rwx SM=COW  ...t_id=42f713af\n      VM_ALLOCATE (reserved)      101048000-101058000    [   64K] rw-\/rwx SM=NUL  ...(unallocated)",
   "extMods" : {"caller":{"thread_create":0,"thread_set_state":0,"task_for_pid":0},"system":{"thread_create":0,"thread_set_state":104,"task_for_pid":5},"targeted":{"thread_create":0,"thread_set_state":0,"task_for_pid":0},"warnings":0},
   "faultingThread" : 0,
   "threads" : [{"triggered":true,"id":12922,"threadState":{"x":[{"value":4311859200},{"value":6163979728},{"value":180224},{"value":262162},{"value":3},{"value":0},{"value":0},{"value":0},{"value":7921753964,"symbolLocation":172,"symbol":"_main_thread"},{"value":7921760208,"symbolLocation":0,"symbol":"errno"},{"value":18},{"value":1},{"value":1},{"value":576460752303620096},{"value":38864},{"value":6163983184},{"value":197},{"value":6473183120,"symbolLocation":56,"symbol":"fcntl"},{"value":0},{"value":6163979728},{"value":3},{"value":6163979728},{"value":6163978312},{"value":245760},{"value":0},{"value":6163977708},{"value":0},{"value":4311859200},{"value":48}],"flavor":"ARM_THREAD_STATE64","lr":{"value":8811855627377083144},"cpsr":{"value":536875008},"fp":{"value":6163976160},"sp":{"value":6163976144},"esr":{"value":2449473543,"description":"(Data Abort) byte read Translation fault"},"pc":{"value":6473532100,"matchesCrashFrame":1},"far":{"value":4311859200}},"frames":[{"imageOffset":365252,"symbol":"dyld3::MachOFile::isMachO(Diagnostics&, unsigned long long) const","symbolLocation":40,"imageIndex":1},{"imageOffset":119560,"symbol":"dyld4::Loader::mapSegments(Diagnostics&, dyld4::RuntimeState&, char const*, unsigned long long, dyld4::Loader::CodeSignatureInFile const&, bool, dyld3::Array<dyld4::Loader::Region> const&, bool, bool, dyld4::Loader::FileValidationInfo const&)","symbolLocation":1080,"imageIndex":1},{"imageOffset":156348,"symbol":"invocation function for block in dyld4::JustInTimeLoader::makeJustInTimeLoaderDisk(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*)","symbolLocation":88,"imageIndex":1},{"imageOffset":154704,"symbol":"dyld4::JustInTimeLoader::withRegions(dyld3::MachOFile const*, void (dyld3::Array<dyld4::Loader::Region> const&) block_pointer)","symbolLocation":292,"imageIndex":1},{"imageOffset":156160,"symbol":"invocation function for block in dyld4::JustInTimeLoader::makeJustInTimeLoaderDisk(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*)","symbolLocation":460,"imageIndex":1},{"imageOffset":184268,"symbol":"dyld4::SyscallDelegate::withReadOnlyMappedFile(Diagnostics&, char const*, bool, void (void const*, unsigned long, bool, dyld4::FileID const&, char const*) block_pointer) const","symbolLocation":132,"imageIndex":1},{"imageOffset":155648,"symbol":"dyld4::JustInTimeLoader::makeJustInTimeLoaderDisk(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*)","symbolLocation":208,"imageIndex":1},{"imageOffset":110568,"symbol":"dyld4::Loader::makeDiskLoader(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, bool, unsigned int, mach_o::Layout const*)","symbolLocation":172,"imageIndex":1},{"imageOffset":116100,"symbol":"invocation function for block in dyld4::Loader::getLoader(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&)","symbolLocation":2056,"imageIndex":1},{"imageOffset":112276,"symbol":"dyld4::Loader::forEachResolvedAtPathVar(dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, dyld4::ProcessConfig::PathOverrides::Type, bool&, void (char const*, dyld4::ProcessConfig::PathOverrides::Type, bool&) block_pointer)","symbolLocation":776,"imageIndex":1},{"imageOffset":36388,"symbol":"dyld4::ProcessConfig::PathOverrides::forEachPathVariant(char const*, dyld3::Platform, bool, bool, bool&, void (char const*, dyld4::ProcessConfig::PathOverrides::Type, bool&) block_pointer) const","symbolLocation":1220,"imageIndex":1},{"imageOffset":111052,"symbol":"dyld4::Loader::forEachPath(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&, void (char const*, dyld4::ProcessConfig::PathOverrides::Type, bool&) block_pointer)","symbolLocation":248,"imageIndex":1},{"imageOffset":113148,"symbol":"dyld4::Loader::getLoader(Diagnostics&, dyld4::RuntimeState&, char const*, dyld4::Loader::LoadOptions const&)","symbolLocation":808,"imageIndex":1},{"imageOffset":147244,"symbol":"invocation function for block in dyld4::JustInTimeLoader::loadDependents(Diagnostics&, dyld4::RuntimeState&, dyld4::Loader::LoadOptions const&)","symbolLocation":412,"imageIndex":1},{"imageOffset":371920,"symbol":"invocation function for block in dyld3::MachOFile::forEachDependentDylib(void (char const*, bool, bool, bool, unsigned int, unsigned int, bool&) block_pointer) const","symbolLocation":148,"imageIndex":1},{"imageOffset":8956,"symbol":"dyld3::MachOFile::forEachLoadCommand(Diagnostics&, void (load_command const*, bool&) block_pointer) const","symbolLocation":300,"imageIndex":1},{"imageOffset":371460,"symbol":"dyld3::MachOFile::forEachDependentDylib(void (char const*, bool, bool, bool, unsigned int, unsigned int, bool&) block_pointer) const","symbolLocation":172,"imageIndex":1},{"imageOffset":146544,"symbol":"dyld4::JustInTimeLoader::loadDependents(Diagnostics&, dyld4::RuntimeState&, dyld4::Loader::LoadOptions const&)","symbolLocation":164,"imageIndex":1},{"imageOffset":26560,"symbol":"dyld4::prepare(dyld4::APIs&, dyld3::MachOAnalyzer const*)","symbolLocation":1176,"imageIndex":1},{"imageOffset":24284,"symbol":"start","symbolLocation":1844,"imageIndex":1}]}],
   "usedImages" : [
   {
     "source" : "P",
     "arch" : "arm64",
     "base" : 4302897152,
     "CFBundleShortVersionString" : "3.2",
     "CFBundleIdentifier" : "org.wxwidgets.Thelemacus",
     "size" : 1556480,
     "uuid" : "afc972fb-7995-3f8a-9355-07916e033edb",
     "path" : "\/Users\/USER\/Desktop\/Thelemacus.app\/Contents\/MacOS\/Thelemacus",
     "name" : "Thelemacus",
     "CFBundleVersion" : "3.2.0"
   },
   {
     "source" : "P",
     "arch" : "arm64e",
     "base" : 6473166848,
     "size" : 607000,
     "uuid" : "efab0dc4-ad82-3527-8267-158fa446599c",
     "path" : "\/usr\/lib\/dyld",
     "name" : "dyld"
   },
   {
     "size" : 0,
     "source" : "A",
     "base" : 0,
     "uuid" : "00000000-0000-0000-0000-000000000000"
   }
 ],
   "sharedCache" : {
   "base" : 6472417280,
   "size" : 4055072768,
   "uuid" : "f6dd368f-4e5e-3550-b73d-0d747cc10c97"
 },
   "vmSummary" : "ReadOnly portion of Libraries: Total=891.8M resident=0K(0%) swapped_out_or_unallocated=891.8M(100%)\nWritable regions: Total=12.4M written=0K(0%) resident=0K(0%) swapped_out=0K(0%) unallocated=12.4M(100%)\n\n                                VIRTUAL   REGION \nREGION TYPE                        SIZE    COUNT (non-coalesced) \n===========                     =======  ======= \nSTACK GUARD                       56.0M        1 \nStack                             8176K        1 \nVM_ALLOCATE (reserved)             160K        3         reserved VM address space (unallocated)\n__DATA                              27K        2 \n__DATA_CONST                       134K        2 \n__DATA_DIRTY                         7K        1 \n__LINKEDIT                       889.7M        2 \n__TEXT                            2116K        2 \ndyld private memory               4384K        4 \nmapped file                       21.2M       26 \n===========                     =======  ======= \nTOTAL                            981.6M       44 \nTOTAL, minus reserved VM space   981.4M       44 \n",
   "legacyInfo" : {
   "threadTriggered" : {

   }
 },
   "logWritingSignature" : "a70e4637af765147332331d8f9357f13ac1dc54f",
   "trialInfo" : {
   "rollouts" : [
     {
       "rolloutId" : "5f72dc58705eff005a46b3a9",
       "factorPackIds" : {

       },
       "deploymentId" : 240000015
     },
     {
       "rolloutId" : "60f8ddccefea4203d95cbeef",
       "factorPackIds" : {
         "NETWORK_SERVICE_PROXY_CONFIG_UPDATE" : "6401414cc679dd6bc5828183"
       },
       "deploymentId" : 240000025
     }
   ],
   "experiments" : [

   ]
 }
 }

 Model: Mac15,3, BootROM 10151.41.12, proc 8:4:4 processors, 24 GB, SMC
 Graphics: Apple M3, Apple M3, Built-In
 Display: Color LCD, 3024 x 1964 Retina, Main, MirrorOff, Online
 Memory Module: LPDDR5, Micron
 AirPort: spairport_wireless_card_type_wifi (0x14E4, 0x4388), wl0: Sep  1 2023 19:33:37 version 23.10.765.4.41.51.121 FWID 01-e2f09e46
 AirPort:
 Bluetooth: Version (null), 0 services, 0 devices, 0 incoming serial ports
 Network Service: Wi-Fi, AirPort, en0
 USB Device: USB31Bus
 USB Device: USB31Bus
 Thunderbolt Bus: MacBook Pro, Apple Inc.
 Thunderbolt Bus: MacBook Pro, Apple Inc.

 
 
 - on APPLE, the cmd (apple) key corresponds to WXK_CONTROL
 - on WIN32, to recognize what character key has been pressed, do (event.GetUnicodeKey()) and check if it is equal to, sai, 'A', not 'a'
 - on WIN32, if you press control key on runtime from within Visual Studio, it will not be detected as WXK_CONTROL, while if you run the exe file from system resources it does.
 - on WIN32, keep the output directory as $(wxIntRootDir) : if you set it to Z:\ the program will not run
 - to setup everything on Mac OS (incomplete):
     1. Download and install boost_1_83_0
     2. Download and install gsl (gnu scientifc library)
     3. Download and install wxWidgets
     4. Open  /Applications/wxWidgets-XXX/samples/minimal/minimal_cocoa.xcodeproj/
        * remove minimal.cpp and insert main.cpp, lib.cpp, main.h, lib.h and constants.h in the project.
        * select both app-static and app-dynamic -> add /usr/local/include/gsl/ /usr/local/include/ /Applications/boost_1_83_0/ $(WXROOT)/build/osx/setup/$(WXTOOLKIT)/include/ $(WXROOT)/include/ to 'User header search path' (you need to split this in multiple lines when you add)
        * select both app-static and app-dynamic -> add -lgsl -lcblas -lboost_filesystem -lboost_system to 'Other linker flags' (you need to split this in multiple lines when you add)
        * select both app-static and app-dynamic -> add '/Applications/boost_1_83_0/stage/lib/' to 'Runpath search paths' and to 'Library Search Paths'
        * select both app-static and app-dynamic -> add '/usr/local/lib' to Library Search Paths
        * select both app-static and app-dynamic  set 'Always search user paths (deprecated)' to 'Yes'
        * set Product -> scheme -> dynamic
      5. Add all necessary paths by adding, say, 'export PATH="/Applications/wxWidgets-3.2.4/build-cocoa-debug:$PATH"' to ~/.zprofile
      6. To make breakpoints in included files to work in Xcode, create file  ~/.lldbinit and write in it 'settings set target.inline-breakpoint-strategy always'
      7. To build the app with make_app.sh:
        * download the libpng library from https://sourceforge.net/projects/libpng/
 - to set up everything on Windows 10:
     0. With Virtual Box, set up a partition with Windows 10. In the partition, set 2 cores, 13387 MB of base memory, set 'Shared Clipboard' to 'Bidirectional', install 'Guest Additions' and share the folder, say, sight_reduction_program, containing main.cpp, main.h, lib.cpp, lib.h, constants.h etc. Set up 100 GB of disk on the partition, and set it to pre-allocated.
     1. Install WinZip
     2. Istall MinGW : download mingw-get-setup from https://sourceforge.net/projects/mingw/
     3. Install Visual Studio: download VisualStudioSetup from https://visualstudio.microsoft.com/downloads/
     4. Istall wxWidgets: download wxMSW-3.2.4-Setup.exe from https://wxwidgets.org/downloads/ and install it in, say,  C:\wxWidgets-3.2.4_debug\
     5. Open  C:\wxWidgets-3.2.4_debug\build\msw\wx_vc17.sln with Visual studio, set everywhere that the mode is multi-threaded debug (not dll) and build (you may get an error the first time you build, just build multiple times and the error will disappear)
     6. Open   C:\wxWidgets-3.2.4_debug\samples\minimal\minimal_vc17.sln -> set the mode to Multi Threaded Debug everywhere, replaced all existing files from Source files with main.h main.cpp lib.h constants.h init.txt, replace all existing Resource files with resource_file_winsows.rc
     7. Install gsl: download gnu-gsl-for-windows-master.zip from https://github.com/ahmadyan/gnu-gsl-for-windows/tree/master/build.vc11  -> extract it in C:\gnu-gsl-for-windows-master -> open  C:\gnu-gsl-for-windows-master\build.vc11\gsl.lib.sln with Visual Studio, set everything to Multi-Threaded Debug -> Build
     8. Open C:\wxWidgets-3.2.4_debug\samples\minimal\minimal_vc17.sln with Visual Studio -> Add C:\gnu-gsl-for-windows-master\lib\x64\Debug\gsl.lib and  C:\gnu-gsl-for-windows-master\lib\x64\Debug\cblas.lib to  -> project  -> minimal properties -> configuration properties -> linker -> input -> additional dependencies
     9. Install boost library : download boost_1_83_0.zip (not 84 or later versions!) from https://sourceforge.net/projects/boost/files/boost/1.83.0/boost_1_83_0.zip/download?use_mirror=altushost-swe&use_mirror=altushost-swe&r=https%3A%2F%2Fsourceforge.net%2Fp%2Fboost%2Factivity%2F%3Fpage%3D0%26limit%3D100 -> extract boost_1_83_0.zip to, say,  C:\boost_1_83_0 -> Open command prompt -> cd into  C:\boost_1_83_0 ->  do 'bootstrap' and 'b2 variant=debug link=static runtime-link=static runtime-debugging=on'
     10. Open C:\wxWidgets-3.2.4_debug\samples\minimal\minimal_vc17.sln with Visual Studio -> go to  project -> configuration properties -> C/C++ -> additional include directories, add C:\boost_1_83_0 , C:\gnu-gsl-for-windows-master and C:\gnu-gsl-for-windows-master\gsl
     11. Open C:\wxWidgets-3.2.4_debug\samples\minimal\minimal_vc17.sln with Visual Studio-> go to   project ->  minimal properties -> linker -> general -> additional library directories, add  C:\boost_1_83_0\stage\lib
     12. Open C:\wxWidgets-3.2.4_debug\samples\minimal\minimal_vc17.sln with Visual Studio -> go to project -> configuration properties -> general -> C++ language standard  and set ISO C++20 Standard (/std:c++20)
     13. Open C:\wxWidgets-3.2.4_debug\samples\minimal\minimal_vc17.sln with Visual Studio -> go to project -> minimal properties -> configuration properties -> general -> set target name to, say, Thelemacus_debug
 - on MSW, if you allocate 13387 MB of Base Memory, it runs by clicking on the .exe and with coastlines = y
 - sometimes the drag operation with mercator projection ends up to the original positon because you end up hitting the max min latitude when dragging
 - to make the app executable: $chmod +x /Thelemacus.app/Contents/MacOS/Thelemacus
 - to watch a variable any time it changes value, 1. set a breakpoint 2. in the lldb console type watch set variable MyClass.variable_in_the_class 3. Press play again.
 - ChartDirector uses colors in the format 0xRRGGBB, while wxWidgets in format 0xBBGGRR
 - when a new chartframe is created, call ShowCharts to re-arrange all chartframes.
 - when a new chartframe is created, position it so it is not hidden by the menu bar on top of the screen
 - per creare un progetto xcode con wxwidgets 1. decomprimi il tar.gz di wxwidgets 2. in samples/minimal/ modifical minimal_cocoa mettendoci i files C++ e le referenze in system header search path, library search path, le linking flag, etc.
 - se xcode ti crea nuove copie dei file .cpp e .h quando li aggiungi a un progetto, fai attenzione a deselezioare 'copy items if needed' nel momento in cui li aggiungi al progetto. Non mettere i file in /src/, altrimenti xcode ti crea una copie di quei file nella directory in cui si trova il file .xcodeproj
 
 
 ********** THINGS TO ADD/IMPROVE ************
 - check whether you can remove Fitall in void ListFrame::DrawAll(void) { and whether it speeds up things
 - when transporting a sight allow the user to enter speed and time instead of distance
 - add instrumental error
 - readme file
 - implement the read-from-file structure used for Data also for the other composite objects such as Body etc
 - check whether you can speed up Draw3D by tabulating stuff it i < 90  etc
 - center name of file in top bar of ListFrame
 - add popup when mouse on button
 - propose to donate to the user
 - when I enter an invalide value in a (for example) AngleField and the error message is prompted and I click ok -> make sure that the focus is set to the field so the user can directly correct its value
 - when switching to dark mode, buttons with images must have white and black inverted -> adjust color of PNG images according to app color
 - change     highlight_color = color_selected_item and     highlight_font = default_font;
 with a better  choice
 - add + / - button in chartframe to zoom in / out
 - add lambert projection and 3D sphere with no projection
 - add separator between recent items and non-recent items in BodyField->name
 - set up output to a log file
 - set parents to constructors of all frames that you defined
 - in Route::draw_3D, code the part for loxodrome curves
 - move all constants to the init file , such as  the 0.15 in  'position_plot_area = wxPoint((size_chart.GetWidth())*0.15, (size_chart.GetHeight())*0.1);'
 - when one tries to drag a route , ask the user whether he wants to disconnect it from the sight by allowing no for an answer
 - replace use of epsilon_double with more proper methods
 - Make sure that Draw is not called uselessly some times
 - in DrawPanel::draw_3D : when drawing minor ticks on the x axis : because I am drawing a loxodrome, I am using the old function Route::draw -> replace this with Route::draw_3D in the future
 - move all stuff which is general enough in the code to MyApp class
 
 ********** THINGS TO FIX ************
 - allow for messages with \n QuestionFrame and MessageFrame
 - when you transport something -> existing Route -> press cancel bbecause you changed your mind -> something is transported anyway
 for MSW:
 - esc key does not work in SightFrame and other *Frames when one does not FOCUS on a control
 - write the WIN32 part of void MyApp::OnTimer
 - handle light/dark mode on WIN32 and create resources for images in the /Dark/ folder
 */

//this function is executed reguarly over time, to check some things
void MyApp::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    
#ifdef __APPLE__
    
    
    if(dark_mode != (settings->GetAppearance().IsDark())) {
        //if the dark mode of the operating system has changed
        
        if ((settings->GetAppearance().IsDark())) {
            //the system is in dark mode -> set image path equal to the /Dark folder
            
            image_directory = root_directory.append(String("Contents/Resources/Images/Dark/"));
            
        }
        else {
            //the system is in light mode ->  set image path equal to the /Light folder
            
            image_directory = root_directory.append(String("Contents/Resources/Images/Light/"));
            
        }
        
        //I re-draw all the ChartFrames so their fore/background colors will be adapted to the new mode of the operating system.
        list_frame->DrawAll();
    }
    
    dark_mode = (settings->GetAppearance()).IsDark();
    
#endif
#ifdef _WIN32
    
#endif
    
    
}

//if the user presses Ctrl + Q to exit the app, I call this function which prompts a message frame
template<class T> void MyApp::OnPressCtrlQ([[maybe_unused]] T& event) {
    
    //    for(; 0<chart_frames.size(); ){
    //        (chart_frames[0])->OnClose(event);
    //    }
    //
    //    Destroy();
    //    Close(true);
    
    //    return this->wxApp::OnPressCtrlQ();
    UnsetIdling<ListFrame>* unset_idling;
    CloseFrame<ListFrame>* close;
    PrintQuestion<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame> >* print_question;
    unset_idling = new UnsetIdling<ListFrame>(list_frame);
    close = new CloseFrame<ListFrame>(list_frame);
    
    //    PrintMessage<ListFrame, Close<ListFrame> >* print_info_message;
    
    print_question = new PrintQuestion<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame> >(list_frame, close, unset_idling);
    
    print_question->SetAndCall(NULL, String("You pressed CTRL+Q"), String("Do you want to quit the app?"), String("Yes"), String("No"));
    
    //    QuestionFrame<Close<ListFrame>, UnsetIdling<ListFrame>>* question_frame = new QuestionFrame<Close<ListFrame>, UnsetIdling<ListFrame>>(NULL,
    //                                                                                                        close,
    //                                                                                                        String("Yes"),
    //                                                                                                        unset_idling, String("No"),
    //                                                                                                        "",
    //                                                                                                        "Do you want to close the app?",
    //                                                                                                        wxDefaultPosition,
    //                                                                                                        wxDefaultSize,
    //                                                                                                        String(""));
    //    question_frame->Show(true);
    
    
    //    print_info_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(list_frame, unset_idling);
    //    print_info_message = new PrintMessage<ListFrame, Close<ListFrame> >(list_frame, close);
    //    print_info_message->SetAndCall(NULL, String("You want to exit the app"), String("Exiting the app"));
    
    
    
    
    //    delete list_frame;
    //
    
    
}

//compute the astronomical position and updated all the GUI fields in set() and re-draws everything
template<class T> void ListFrame::ComputePosition([[maybe_unused]] T& event) {
    
    PrintQuestion<ListFrame, AllRoutes, SomeRoutes>* print_question;
    AllRoutes* all_routes;
    SomeRoutes* some_routes;
    
    all_routes = new AllRoutes(this);
    some_routes = new SomeRoutes(this);
    print_question = new PrintQuestion<ListFrame, AllRoutes, SomeRoutes>(this, all_routes, some_routes);
    
    selecting_route_for_position = true;
    
    //ask the user whether he/she wants to transport the sight with a an existing route or with a new route.
    print_question->SetAndCall(NULL, String("You want to determine the astronomical position"), String("With what route do you want to do it?"), String("All routes"), String("Some routes"));
    
    
    
    
    
}

//shows all ChartFrames and positions them properly on the screen
template<class T> void MyApp::ShowChart([[maybe_unused]] T& event) {
    
    unsigned int i;
    //the spacing between one frame and another in the x and y direction, respectively
    double delta_x, delta_y;
    
    i = (((list_frame->menu_bar)->GetSize()).GetHeight());
    
    if (((list_frame->chart_frames).size()) > 1) {
        //if ((list_frame->chart_frames).size() > 1 it makes sens to introduce delta_x, delta_y
        
        delta_x = (((double)(rectangle_display.GetWidth())) - ((double)(((((list_frame->chart_frames)[0])->GetSize()).GetWidth()) + ((((list_frame->chart_frames)[((list_frame->chart_frames).size()) - 1])->GetSize()).GetWidth()))) / 2.0 - 2.0 * ((wxGetApp().border).value)) / ((double)(((list_frame->chart_frames).size()) - 1));
        delta_y = (((double)(rectangle_display.GetHeight())) - ((double)(((((list_frame->chart_frames)[0])->GetSize()).GetHeight()) + ((((list_frame->chart_frames)[((list_frame->chart_frames).size()) - 1])->GetSize()).GetHeight()))) / 2.0 - 2.0 * ((wxGetApp().border).value)) / ((double)(((list_frame->chart_frames).size()) - 1));
        
    }
    else {
        //if ((list_frame->chart_frames).size() <= 1, it does not make sense to define delta_x, delta_y, and I set
        
        delta_x = 0.0;
        delta_y = 0.0;
        
    }
    
    
    for (i = 0; i < ((list_frame->chart_frames).size()); i++) {
        
        ((list_frame->chart_frames)[i])->Show(true);
        ((list_frame->chart_frames)[i])->Raise();
        ((list_frame->chart_frames)[i])->SetPosition(wxPoint(
                                                             
                                                             (((double)((((list_frame->chart_frames)[0])->GetSize()).GetWidth())) - ((double)((((list_frame->chart_frames)[i])->GetSize()).GetWidth()))) / 2.0 + ((wxGetApp().border).value) + delta_x * ((double)i)
                                                             ,
#ifdef __APPLE__
                                                             //I am on APPLE operating system -> there is a menu bar
                                                             /*here I shift everything down on the screen by the height of the menu_bar, because otherwise the ChartFrame on the top would be partially corvered by the menu bar and the one on the bottom would leave an empty space t the bottom of the screen */
                                                             
                                                             
                                                             (((list_frame->menu_bar)->GetSize()).GetHeight()) +
                                                             
#endif
                                                             
                                                             (((double)((((list_frame->chart_frames)[0])->GetSize()).GetHeight())) - ((double)((((list_frame->chart_frames)[i])->GetSize()).GetHeight()))) / 2.0 + ((wxGetApp().border).value) + delta_y * ((double)i)
                                                             
                                                             ));
        
    }
    
}

//shows ListFrame and positions it properly on the screen
void MyApp::ShowList([[maybe_unused]] wxCommandEvent& event) {
    
    list_frame->SetPosition(wxPoint(0, /*move down ListFrame to take account of the menu_bar*/((list_frame->menu_bar)->GetSize()).GetHeight()));
    list_frame->Raise();
    
}

//writes into this->root_directory the path where the executable is currently running
void MyApp::where_am_I([[maybe_unused]] String prefix) {
    
    stringstream ins;
    
    ins.str("");
    //note that here boost::dll::program_location() may return a path with './' at the end, but this has no effect because ./ simply says to stay in the same path
    ins << (boost::dll::program_location().parent_path());
    root_directory.value = ins.str().c_str();
    
    root_directory.print(String("Non-formatted run directory"), true, String("*******"), cout);
    
    //remove " from root_directory
    
#ifdef __APPLE__
    //if I am on APPLE operating system, I go down by two directory levels to obtain the root_directory
    
    root_directory.value.erase(std::remove(root_directory.value.begin(), root_directory.value.end(), '"'), root_directory.value.end());
    root_directory.appendto(String("/../../"));
    
#endif
#ifdef _WIN32
    //I am on WIN32 operating system: I remove the '\"'that is present in root_directory at the beginning and at the end to obatin a root_directory that can be used as a path
    
    (root_directory.value) = (root_directory.value.substr(1, root_directory.value.size() - 2));
    
#endif
    
    //root_directory.print(String("Formatted root directory"), true, String("*******"), cout);
    
    
}

bool MyApp::OnInit() {
    
    unsigned int i;
    Int n_chart_frames;
    stringstream s;
    String projection, temp;
    wxFrame* dummy_frame;
    ShowAll* show_all;
    QuestionFrame< ShowAll, CloseFrame<ListFrame> >* disclaimer;
    //this contains the current time, the time of the transition from night to day (dawn), and the time of the transition from day to night (dusk)
    //    Chrono current_time, dawn, dusk;
    
    wxInitAllImageHandlers();
    
    
    degree_symbol = String("\u00b0");
    
    
    
    //detect the operating system
#ifdef __APPLE__
    
    cout << "You have Apple Operating System" << "\n";
    
    //to build the app on mac
    where_am_I(String(""));
    //to develop the app with Xcode on OSX
    //new Mac
//    root_directory = String("/Users/michele/Documents/sight_reduction_program/");
    //old Mac
    //    root_directory = String("/Users/macbookpro/Documents/sight_reduction_program/");
    
#endif
    
#ifdef _WIN32
    
    cout << "You have Windows Operating System" << "\n";
    //degree_symbol = String((char)176);
    
    
    //to run the app with Visual Studio on Windows
    //	root_directory = String("Z:/");
    where_am_I(String(""));
    
#endif
    
    settings = new wxSystemSettings();
    timer = new wxTimer();
        
    dummy_frame = new wxFrame();
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    display.GetFromWindow(dummy_frame);
    dummy_frame->~wxFrame();
    
    rectangle_display = (display.GetClientArea());
    rectangle_display.SetWidth((int)((double)rectangle_display.GetWidth()));
    rectangle_display.SetHeight((int)((double)rectangle_display.GetHeight()));
    
    
    //directories are set dynamically from root_directory
    path_file_init = root_directory.append(String("Contents/Resources/Data/init.txt"));
    data_directory = root_directory.append(String("Contents/Resources/Data/"));
    if ((settings->GetAppearance().IsDark())) {
        //the system is in dark mode
        image_directory = root_directory.append(String("Contents/Resources/Images/Dark/"));
    }
    else {
        //the system is in light mode
        image_directory = root_directory.append(String("Contents/Resources/Images/Light/"));
    }
    default_open_directory = root_directory;
    
    //read the file names and prenend to the file name the respective directory where the file is located -> obtain the file path
    //files in code directory
    
    //files in data directory
    //	path_file_recent = data_directory.append(read_from_file(String("name file recent"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_catalog = data_directory.append(read_from_file(String("name file catalog"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_coastline_data_blocked = data_directory.append(read_from_file(String("name file coastline data blocked"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_n_line = data_directory.append(read_from_file(String("name file n line"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_sample_sight = data_directory.append(read_from_file(String("name file sample sight"), (wxGetApp().path_file_init), String("R"), String("")));
    
    //files in image directory
    path_file_app_icon = image_directory.append(read_from_file(String("name file app icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_error_icon = image_directory.append(read_from_file(String("name file error icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_warning_icon = image_directory.append(read_from_file(String("name file warning icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_question_icon = image_directory.append(read_from_file(String("name file question icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_plus_icon = image_directory.append(read_from_file(String("name file plus icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_list_icon = image_directory.append(read_from_file(String("name file list icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_position_icon = image_directory.append(read_from_file(String("name file position icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_map_icon = image_directory.append(read_from_file(String("name file map icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_pencil_icon = image_directory.append(read_from_file(String("name file pencil icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_trash_icon = image_directory.append(read_from_file(String("name file trash icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_arrow_icon = image_directory.append(read_from_file(String("name file arrow icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_disconnect_icon = image_directory.append(read_from_file(String("name file disconnect icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_michele_icon = image_directory.append(read_from_file(String("name file michele icon"), (wxGetApp().path_file_init), String("R"), String("")));
    
    
    data_precision.read_from_file_to(String("data precision"), (wxGetApp().path_file_init), String("R"), String(""));
    display_precision.read_from_file_to(String("display precision"), (wxGetApp().path_file_init), String("R"), String(""));
    time_check_light_dark.read_from_file_to(String("time check light dark"), (wxGetApp().path_file_init), String("R"), String(""));
    animation_time.read_from_file_to(String("animation time"), (wxGetApp().path_file_init), String("R"), String(""));
    time_zone.read_from_file_to(String("time zone"), (wxGetApp().path_file_init), String("R"), String(""));
    
    max_lat.read_from_file_to(String("maximal latitude coastline data"), (wxGetApp().path_file_init), String("R"), String(""));
    min_lat.read_from_file_to(String("minimal latitude coastline data"), (wxGetApp().path_file_init), String("R"), String(""));
    
    size_small_button_over_width_screen.read_from_file_to(String("size small button over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    size_large_button_over_width_screen.read_from_file_to(String("size large button over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    size_icon_over_width_screen.read_from_file_to(String("size icon over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    size_message_image_over_width_screen.read_from_file_to(String("size message image over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    point_size.read_from_file_to(String("point size"), (wxGetApp().path_file_init), String("R"), String(""));
    
    
    //----- test for Data::read_from_file_to - start
    /*
     Data* my_data;
     Catalog* my_catalog;
     
     my_catalog = new Catalog((wxGetApp().path_file_catalog), String(""));
     my_data = new Data(my_catalog, String(""));
     
     my_data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("R"), String("**"));
     
     */
    //----- test for Data::read_from_file_to - end
    
    //set size_small/large_button from size_small_button_over_width_screen and size_large_button_over_width_screen
    size_small_button = wxSize(
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_small_button_over_width_screen).value),
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_small_button_over_width_screen).value)
                               );
    size_large_button = wxSize(
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value),
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)
                               );
    
    
    length_plot_area_over_length_chart.read_from_file_to(String("length of plot area over length of chart"), (wxGetApp().path_file_init), String("R"),  String(""));
    length_chart_over_length_chart_frame.read_from_file_to(String("length of chart over length of chart frame"), (wxGetApp().path_file_init), String("R"),  String(""));
    length_border_over_length_screen.read_from_file_to(String("length of border over length of screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    
    border.set(String("border"), (rectangle_display.GetWidth()) * (length_border_over_length_screen.value), String(""));
    
    //read n_points_minor_ticks from file_init
    n_points_minor_ticks.read_from_file_to(String("number of points for minor ticks"), (wxGetApp().path_file_init), String("R"), String(""));
    //read relative_displacement from file_init
    relative_displacement.read_from_file_to(String("relative displacement"), (wxGetApp().path_file_init), String("R"), String(""));
    //read standard_thickness_over_length_screen from file_init
    standard_thickness_over_length_screen.read_from_file_to(String("standard thickness over length screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    //read large_thickness_over_length_screen from file_init
    large_thickness_over_length_screen.read_from_file_to(String("large thickness over length screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    //read color horizon from file
    color_horizon.read_from_file(String("color horizon"), (wxGetApp().path_file_init), String("R"), String(""));
    //read color selected item from file
    color_selected_item.read_from_file(String("color selected item"), (wxGetApp().path_file_init), String("R"), String(""));
    //read tick length over width plot area from file_init
    tick_length_over_width_plot_area.read_from_file_to(String("tick length over width plot area"), (wxGetApp().path_file_init), String("R"), String(""));
    //read tick length over width plot area from file_init
    tick_length_over_aperture_circle_observer.read_from_file_to(String("tick length over aperture circle observer"), (wxGetApp().path_file_init), String("R"), String(""));
    //read min_crossing_angle from file_init
    min_crossing_angle.read_from_file_to(String("minimal crossing angle between circles of equal altitude"), (wxGetApp().path_file_init), String("R"), String(""));
    
    
    
    //read the time, and set the background color to either the day or night background color, which are read from file
    //    time_zone.read_from_file(String("time zone"), (wxGetApp().path_file_init), String(""));
    //    dawn.read_from_file(String("dawn"), (wxGetApp().path_file_init), String(""));
    //    dusk.read_from_file(String("dusk"), (wxGetApp().path_file_init), String(""));
    //    current_time.set_current(time_zone, String(""));
    
    
    //    if((current_time < dawn) || (current_time > dusk)){
    //        //we are at night -> set background color to night mode
    //
    //        foreground_color.read_from_file(String("night foreground color"), (wxGetApp().path_file_init), String(""));
    //
    //    }else{
    //        //we are at day -> set background color ot day mode
    //
    //        foreground_color.read_from_file(String("day foreground color"), (wxGetApp().path_file_init), String(""));
    //
    //    }
    
    timer->Bind(wxEVT_TIMER, &MyApp::OnTimer, this);
    
    
    
    foreground_color = Color(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    background_color = Color(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
    error_color.read_from_file(String("error color"), (wxGetApp().path_file_init), String("R"), String(""));
    highlight_color = color_selected_item;
    dark_mode = (settings->GetAppearance()).IsDark();
    
    n_recent_bodies.read_from_file_to(String("number of recent bodies"), (wxGetApp().path_file_init), String("R"), String(""));
    n_recent_projections.read_from_file_to(String("number of recent projections"), (wxGetApp().path_file_init), String("R"), String(""));
    n_animation_steps.read_from_file_to(String("number of animation steps"), (wxGetApp().path_file_init), String("R"), String(""));
    
    
    list_frame = new ListFrame("Unnamed", "", wxDefaultPosition, wxDefaultSize, String(""));
    if(!(list_frame->abort)){
        //the user has not pressed cancel while charts were loading -> I proceed and start the app
        
        show_all = new ShowAll(list_frame);
        disclaimer = new QuestionFrame< ShowAll , CloseFrame<ListFrame> >(NULL, show_all, String("Yes, I know what I am doing."), (list_frame->close), String("No, I will pass."),
                                                                          "Welcome to Thelemacus!",
                                                                          //                                                                          "On December 16, 1719, Captain J. Cook perceived the first Australian aborigens from HMS Endeavour, off the coast of Perth.\n He was on a mission commissioned by King John III, designed to discover new commercial routes, and new worlds.\n His voyage had been made possible by the novel, state-of-the art astronomical positioning methods\n based on the marine chronometer built by J. Harrison, which was on board the Endeavour. \nThe reliability of the positioning method allowed the british realm to trace and map the coasts of new, unknonw lands, \nand paved the way to a new way to sail which lasted until the invention of GPS.\n With this application, you will bring back to life astronomical positioning methods, in a way that no other existing application allows for, and entering in a novel historical path. "
                                                                          "This is the state-of-the art application for celestial navigation, I hope you will enjoy it! \t Remember that this software comes with no warranty, use at your own risk!\t Do you want to proceed? \t Fair winds, following seas ..."
                                                                          ,
                                                                          
                                                                          wxGetApp().path_file_warning_icon,
                                                                          wxDefaultPosition,
                                                                          wxDefaultSize,
                                                                          String(""));
        disclaimer->Show(true);
        disclaimer->Raise();
        //    list_frame->Show(true);
        
        
        
        
        //allocate and show the chart frames
        n_chart_frames.read_from_file_to(String("number chart frames"), (wxGetApp().path_file_init), String("R"), String(""));
        (list_frame->chart_frames).resize(n_chart_frames.value);
        for (i = 0; i < (list_frame->chart_frames).size(); i++) {
            
            projection = String((((i % 2) == 0) ? "Mercator" : "3D"));
            
            //open a Mercator projection for even is and a 3D projection for odd is
            s.str("");
            s << "Chart #" << i + 1 << " - " << projection.value << " projection";
            
            string dummy = s.str();
            
            (list_frame->chart_frames)[i] = new ChartFrame(
                                                           list_frame,
                                                           projection,
                                                           s.str(),
                                                           wxDefaultPosition,
                                                           wxDefaultSize,
                                                           String("")
                                                           );
            //        ((list_frame->chart_frames)[i])->Show(true);
            //        ((list_frame->chart_frames)[i])->Raise();
            
        }
        
        
        //bring either of these wxFrames to front
        //    list_frame->Raise();
        
        //fore/background color is determined from the default background color of extract_colors
        foreground_color = Color(list_frame->extract_colors->GetForegroundColour());
        background_color = Color(list_frame->extract_colors->GetBackgroundColour());
        
        
        //extracts the default font and creates a error_font, obtained from default font by setting its weight to wxFONTWEIGHT_BOLD
        default_font = (list_frame->extract_colors->GetFont());
        error_font = (list_frame->extract_colors->GetFont());
        highlight_font = default_font;
        error_font.SetWeight(wxFONTWEIGHT_BOLD);
        
        timer->Start(/*time_check_light_dark is converted in milliseconds, because Start() takes its first argument in milliseconds*/((time_check_light_dark.h) * 60.0 * 60.0 + (time_check_light_dark.m) * 60.0 + (time_check_light_dark.s)) * 1000.0, wxTIMER_CONTINUOUS);
        
        
        return true;
        
    }else{
        //the user pressed cancel -> I quit the app
        
        wxCommandEvent dummy;
        
        (*(list_frame->close))(dummy);
        
        return false;
        
    }
    
    
}
