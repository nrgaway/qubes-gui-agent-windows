#include <windows.h>
#include <tchar.h>
//#include <stdlib.h>
#include <stdio.h>
#include <setupapi.h>
//#include <regstr.h>
//#include <infstr.h>
#include <cfgmgr32.h>
//#include <string.h>
//#include <malloc.h>
//#include <newdev.h>
//#include <objbase.h>
#include <strsafe.h>

void usage(LPCTSTR self) {
	fprintf(stderr, "Usage: %S inf-path hardware-id\n", self);
	fprintf(stderr, "Error codes: \n");
	fprintf(stderr, "  1 - other error\n");
	fprintf(stderr, "  2 - not enough parameters\n");
	fprintf(stderr, "  3 - empty inf-path\n");
	fprintf(stderr, "  4 - empty hardware-id\n");
	fprintf(stderr, "  5 - failed to open inf-path\n");
}

int __cdecl _tmain(int argc, PZPWSTR argv) {
    HDEVINFO DeviceInfoSet = INVALID_HANDLE_VALUE;
    SP_DEVINFO_DATA DeviceInfoData;
    GUID ClassGUID;
    TCHAR ClassName[MAX_CLASS_NAME_LEN];
    TCHAR hwIdList[LINE_LEN+4];
    LPCTSTR hwid = NULL;
    LPCTSTR inf = NULL;
	int retcode = 1;

    if (argc<2) {
		usage(argv[0]);
		return 2;
    }

    inf = argv[1];
    if (!inf[0]) {
		usage(argv[0]);
        return 3;
    }

    hwid = argv[2];
    if (!hwid[0]) {
		usage(argv[0]);
        return 4;
    }

    // List of hardware ID's must be double zero-terminated
    ZeroMemory(hwIdList,sizeof(hwIdList));
    if (FAILED(StringCchCopy(hwIdList,LINE_LEN,hwid)))
        goto cleanup;

    // Use the INF File to extract the Class GUID.
    if (!SetupDiGetINFClass(inf,&ClassGUID,ClassName,sizeof(ClassName)/sizeof(ClassName[0]),0)) {
		retcode = 5;
        goto cleanup;
	}

    // Create the container for the to-be-created Device Information Element.
    DeviceInfoSet = SetupDiCreateDeviceInfoList(&ClassGUID,0);
    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
        goto cleanup;

    // Now create the element.
    // Use the Class GUID and Name from the INF file.
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	if (!SetupDiCreateDeviceInfo(DeviceInfoSet,
				ClassName,
				&ClassGUID,
				NULL,
				0,
				DICD_GENERATE_ID,
				&DeviceInfoData))
        goto cleanup;

    // Add the HardwareID to the Device's HardwareID property.
	if (!SetupDiSetDeviceRegistryProperty(DeviceInfoSet,
				&DeviceInfoData,
				SPDRP_HARDWAREID,
				(LPBYTE)hwIdList,
				(lstrlen(hwIdList)+1+1)*sizeof(TCHAR)))
		goto cleanup;

    // Transform the registry element into an actual devnode
    // in the PnP HW tree.
	if (!SetupDiCallClassInstaller(DIF_REGISTERDEVICE,
				DeviceInfoSet,
				&DeviceInfoData))
        goto cleanup;

	retcode = 0;

cleanup:

    if (DeviceInfoSet != INVALID_HANDLE_VALUE)
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);

    return retcode;
}
