#ifndef CONFIG_H
#define CONFIG_H

/*
	All the URLS, file names and patterns needed by the app
	Grouped here to make it easy to fix something if they change some url or file names
*/

// Program options
#define PROG_VERSION "v2.0.2"

// File used for checking if the patch has been applied
#define PATCH_VER_FILE ".unlocker"
#define LNX_PATCH_VER_PATH "/usr/lib/vmware"

// Log level
// 0 - none
// 1 - info (standard info, files being extracted, downloaded, etc.)
// 2 - debug (useful info for debugging the patch e.g. addresses, values)
// 3 - verbose (detailed info about the code)
#define LOGLEVEL 3

#define ARCH "x86_x64"

// Install - Default option
#define INSTALL_OPTION "--install" 
// Uninstall
#define UNINSTALL_OPTION "--uninstall"
// Only download tools
#define DOWNLOADONLY_OPTION "--download-tools"
// Show help message
#define HELP_OPTION "--help"

// Paths
#define TOOLS_DOWNLOAD_FOLDER "tools"
#define BACKUP_FOLDER "backup"

// Direct URLs to the tools
#define DARWIN_ISO_URL "https://packages-prod.broadcom.com/tools/frozen/darwin/darwin.iso"
#define DARWIN_PRE15_ISO_URL "https://packages-prod.broadcom.com/tools/frozen/darwin/darwinPre15.iso"

// Keep these definitions for file naming consistency
#define FUSION_ZIP_TOOLS_NAME "darwin.iso"
#define FUSION_ZIP_PRE15_TOOLS_NAME "darwinPre15.iso"

// Pattern to parse the versions/builds
#define VERSION_REGEX_PATTERN "<li><a href=\"[^\"]+\">([^<]+)<\\/a><\\/li>"

// Windows registry configuration
#define HKEY_VMWARE HKEY_LOCAL_MACHINE
#define HKEY_SUBKEY_VMWARE "SOFTWARE\\Wow6432Node\\VMware, Inc.\\VMware Player"
#define HKEY_QUERY_VALUE_INSTALLPATH "InstallPath"
#define HKEY_QUERY_VALUE_INSTALLPATH64 "InstallPath64"
#define HKEY_QUERY_VALUE_PRODUCTVERSION "ProductVersion"

// Windows services to stop
#define VM_KILL_SERVICES {"vmware-view-usbd", "VMwareHostd", "VMAuthdService", "VMUSBArbService", "VMnetDHCP", "VMware NAT Service"}
#define VM_KILL_PROCESSES {"vmware-tray.exe"}

// Files to backup (win) { source, destination }
#define VM_WIN_BACKUP_FILES { \
	{"x64\\vmware-vmx.exe", "x64"}, \
	{"x64\\vmware-vmx-debug.exe", "x64"}, \
	{"x64\\vmware-vmx-stats.exe", "x64"}, \
	{"vmwarebase.dll", ""} \
	}

// Files to backup (linux) { source, destination }
#define VM_LNX_BACKUP_FILES { \
	"/usr/lib/vmware/bin/vmware-vmx", \
	"/usr/lib/vmware/bin/vmware-vmx-debug", \
	"/usr/lib/vmware/bin/vmware-vmx-stats", \
	}

// Files to patch (have to keep the same order)
#define VM_WIN_PATCH_FILES { \
	"vmware-vmx.exe", \
	"vmware-vmx-debug.exe", \
	"vmware-vmx-stats.exe", \
	"vmwarebase.dll" \
	}

// Linux paths and files
#define VM_LNX_PATH "/usr/lib/vmware/bin"
#define VM_LNX_BINS { \
	"vmware-vmx", \
	"vmware-vmx-debug", \
	"vmware-vmx-stats", \
	}
#define VM_LNX_LIB_CANDIDATES { \
	"/usr/lib/vmware/lib/libvmwarebase.so/libvmwarebase.so", \
	"/usr/lib/vmware/lib/libvmwarebase.so.0/libvmwarebase.so.0" \
	}
#define VM_LNX_ISO_DESTPATH "/usr/lib/vmware/isoimages"

// Patch data
#define SMC_HEADER_V0 "\xF2\x00\x00\x00\xF0\x00\x00\x00"
#define SMC_HEADER_V0_SZ 8
#define SMC_HEADER_V1 "\xB4\x01\x00\x00\xB0\x01\x00\x00"
#define SMC_HEADER_V1_SZ 8
#define KEY_KEY "\x59\x45\x4B\x23\x04\x32\x33\x69\x75"
#define KEY_KEY_SZ 9
#define ADR_KEY "\x72\x64\x41\x24\x04\x32\x33\x69\x75"
#define ADR_KEY_SZ 9

#define DARWIN_REGEX "\\x10\\x00\\x00\\x00[\\x10|\\x20]\\x00\\x00\\x00[\\x01|\\x02]" \
					"\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00" \
					"\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00"

#define DARWIN_PATTERN_PERM_1 {'\x10', '\x00', '\x00', '\x00', '\x10', '\x00', '\x00', '\x00', '\x01', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',\
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					}

#define DARWIN_PATTERN_PERM_2 {'\x10', '\x00', '\x00', '\x00', '\x20', '\x00', '\x00', '\x00', '\x01', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					}

#define DARWIN_PATTERN_PERM_3 {'\x10', '\x00', '\x00', '\x00', '\x10', '\x00', '\x00', '\x00', '\x02', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					}

#define DARWIN_PATTERN_PERM_4 {'\x10', '\x00', '\x00', '\x00', '\x20', '\x00', '\x00', '\x00', '\x02', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', \
					}

#define SMC_NEW_DATA "bheuneqjbexolgurfrjbeqfthneqrqcy"
#define SMC_NEW_DATA2 "rnfrqbagfgrny(p)NccyrPbzchgreVap"

#define VMKCTL_FIND_STR "applesmc"
#define VMKCTL_REPLACE_STR "vmkernel"

#endif // CONFIG_H
