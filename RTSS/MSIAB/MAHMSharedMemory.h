/////////////////////////////////////////////////////////////////////////////
//
// This header file defines MSI Afterburner Hardware Monitoring shared memory format
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _MAHM_SHARED_MEMORY_INCLUDED_
#define _MAHM_SHARED_MEMORY_INCLUDED_
/////////////////////////////////////////////////////////////////////////////
// v2.0 header
typedef struct MAHM_SHARED_MEMORY_HEADER
{
	DWORD	dwSignature;
		//signature allows applications to verify status of shared memory

		//The signature can be set to:
		//'MAHM'	- hardware monitoring memory is initialized and contains 
		//			valid data 
		//0xDEAD	- hardware monitoring memory is marked for deallocation and
		//			no longer contain valid data
		//otherwise the memory is not initialized
	DWORD	dwVersion;
		//header version ((major<<16) + minor)
		//must be set to 0x00020000 for v2.0
	DWORD	dwHeaderSize;
		//size of header
	DWORD	dwNumEntries;
		//number of subsequent MAHM_SHARED_MEMORY_ENTRY entries
	DWORD	dwEntrySize;
		//size of entries in subsequent MAHM_SHARED_MEMORY_ENTRY entries array
#ifdef _WIN64
	__time32_t time;
#else
	time_t time;
#endif
		//last polling time 

		//WARNING! Force 32-bit time_t usage with #define _USE_32BIT_TIME_T 
		//to provide compatibility with VC8.0 and newer compiler versions

	//WARNING! The following fields are valid for v2.0 and newer shared memory layouts only

	DWORD	dwNumGpuEntries;
		//number of subsequent MAHM_SHARED_MEMORY_GPU_ENTRY entries
	DWORD	dwGpuEntrySize;
		//size of entries in subsequent MAHM_SHARED_MEMORY_GPU_ENTRY entries array

} MAHM_SHARED_MEMORY_HEADER, *LPMAHM_SHARED_MEMORY_HEADER;
/////////////////////////////////////////////////////////////////////////////
#define	MAHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_OSD				0x00000001
	//this bitmask indicates that data source is configured to be displayed in On-Screen Display
#define	MAHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_LCD				0x00000002
	//this bitmask indicates that data source is configured to be displayed in Logitech keyboard LCD
#define	MAHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_TRAY				0x00000004
	//this bitmask indicates that data source is configured to be displayed in tray icon
/////////////////////////////////////////////////////////////////////////////
#define MONITORING_SOURCE_ID_UNKNOWN							0xFFFFFFFF
#define MONITORING_SOURCE_ID_GPU_TEMPERATURE					0x00000000
#define MONITORING_SOURCE_ID_PCB_TEMPERATURE					0x00000001
#define MONITORING_SOURCE_ID_MEM_TEMPERATURE					0x00000002
#define MONITORING_SOURCE_ID_VRM_TEMPERATURE					0x00000003
#define MONITORING_SOURCE_ID_FAN_SPEED							0x00000010
#define MONITORING_SOURCE_ID_FAN_TACHOMETER						0x00000011
#define MONITORING_SOURCE_ID_FAN_SPEED2							0x00000012
#define MONITORING_SOURCE_ID_FAN_TACHOMETER2					0x00000013
#define MONITORING_SOURCE_ID_FAN_SPEED3							0x00000014
#define MONITORING_SOURCE_ID_FAN_TACHOMETER3					0x00000015
#define MONITORING_SOURCE_ID_CORE_CLOCK							0x00000020
#define MONITORING_SOURCE_ID_SHADER_CLOCK						0x00000021
#define MONITORING_SOURCE_ID_MEMORY_CLOCK						0x00000022
#define MONITORING_SOURCE_ID_GPU_USAGE							0x00000030
#define MONITORING_SOURCE_ID_MEMORY_USAGE						0x00000031
#define MONITORING_SOURCE_ID_FB_USAGE							0x00000032
#define MONITORING_SOURCE_ID_VID_USAGE							0x00000033
#define MONITORING_SOURCE_ID_BUS_USAGE							0x00000034
#define MONITORING_SOURCE_ID_MEMORY_USAGE_PROCESS				0x00000035
#define MONITORING_SOURCE_ID_GPU_VOLTAGE						0x00000040
#define MONITORING_SOURCE_ID_AUX_VOLTAGE						0x00000041
#define MONITORING_SOURCE_ID_MEMORY_VOLTAGE						0x00000042
#define MONITORING_SOURCE_ID_AUX2_VOLTAGE						0x00000043
#define MONITORING_SOURCE_ID_FRAMERATE							0x00000050
#define MONITORING_SOURCE_ID_FRAMETIME							0x00000051
#define MONITORING_SOURCE_ID_FRAMERATE_MIN						0x00000052
#define MONITORING_SOURCE_ID_FRAMERATE_AVG						0x00000053
#define MONITORING_SOURCE_ID_FRAMERATE_MAX						0x00000054
#define MONITORING_SOURCE_ID_FRAMERATE_1DOT0_PERCENT_LOW		0x00000055
#define MONITORING_SOURCE_ID_FRAMERATE_0DOT1_PERCENT_LOW		0x00000056
#define MONITORING_SOURCE_ID_GPU_REL_POWER						0x00000060
#define MONITORING_SOURCE_ID_GPU_ABS_POWER						0x00000061
#define MONITORING_SOURCE_ID_GPU_TEMP_LIMIT						0x00000070
#define MONITORING_SOURCE_ID_GPU_POWER_LIMIT					0x00000071
#define MONITORING_SOURCE_ID_GPU_VOLTAGE_LIMIT					0x00000072
#define MONITORING_SOURCE_ID_GPU_UTIL_LIMIT						0x00000074
#define MONITORING_SOURCE_ID_GPU_SLI_SYNC_LIMIT					0x00000075
#define MONITORING_SOURCE_ID_CPU_TEMPERATURE					0x00000080
#define MONITORING_SOURCE_ID_CPU_USAGE							0x00000090
#define MONITORING_SOURCE_ID_RAM_USAGE							0x00000091
#define MONITORING_SOURCE_ID_PAGEFILE_USAGE						0x00000092
#define MONITORING_SOURCE_ID_RAM_USAGE_PROCESS					0x00000093
#define MONITORING_SOURCE_ID_CPU_CLOCK							0x000000A0
#define MONITORING_SOURCE_ID_GPU_TEMPERATURE2					0x000000B0
#define MONITORING_SOURCE_ID_PCB_TEMPERATURE2					0x000000B1
#define MONITORING_SOURCE_ID_MEM_TEMPERATURE2					0x000000B2
#define MONITORING_SOURCE_ID_VRM_TEMPERATURE2					0x000000B3
#define MONITORING_SOURCE_ID_GPU_TEMPERATURE3					0x000000C0
#define MONITORING_SOURCE_ID_PCB_TEMPERATURE3					0x000000C1
#define MONITORING_SOURCE_ID_MEM_TEMPERATURE3					0x000000C2
#define MONITORING_SOURCE_ID_VRM_TEMPERATURE3					0x000000C3
#define MONITORING_SOURCE_ID_GPU_TEMPERATURE4					0x000000D0
#define MONITORING_SOURCE_ID_PCB_TEMPERATURE4					0x000000D1
#define MONITORING_SOURCE_ID_MEM_TEMPERATURE4					0x000000D2
#define MONITORING_SOURCE_ID_VRM_TEMPERATURE4					0x000000D3
#define MONITORING_SOURCE_ID_GPU_TEMPERATURE5					0x000000E0
#define MONITORING_SOURCE_ID_PCB_TEMPERATURE5					0x000000E1
#define MONITORING_SOURCE_ID_MEM_TEMPERATURE5					0x000000E2
#define MONITORING_SOURCE_ID_VRM_TEMPERATURE5					0x000000E3
#define MONITORING_SOURCE_ID_PLUGIN_GPU							0x000000F0
#define MONITORING_SOURCE_ID_PLUGIN_CPU							0x000000F1
#define MONITORING_SOURCE_ID_PLUGIN_MOBO						0x000000F2
#define MONITORING_SOURCE_ID_PLUGIN_RAM							0x000000F3
#define MONITORING_SOURCE_ID_PLUGIN_HDD							0x000000F4
#define MONITORING_SOURCE_ID_PLUGIN_NET							0x000000F5
#define MONITORING_SOURCE_ID_PLUGIN_PSU							0x000000F6
#define MONITORING_SOURCE_ID_PLUGIN_UPS							0x000000F7
#define MONITORING_SOURCE_ID_PLUGIN_MISC						0x000000FF
#define MONITORING_SOURCE_ID_CPU_POWER							0x00000100
/////////////////////////////////////////////////////////////////////////////
// v2.0 entry (main array of entries follows immediately by the header)
typedef struct MAHM_SHARED_MEMORY_ENTRY
{
	char	szSrcName[MAX_PATH];
		//data source name (e.g. "Core clock")
	char	szSrcUnits[MAX_PATH];
		//data source units (e.g. "MHz")

	char	szLocalizedSrcName[MAX_PATH];
		//localized data source name (e.g. "„астота €дра" for Russian GUI)
	char	szLocalizedSrcUnits[MAX_PATH];
		//localized data source units (e.g. "ћ√ц" for Russian GUI)

	char	szRecommendedFormat[MAX_PATH];
		//recommended output format (e.g. "%.3f" for "Core voltage" data source) 

	float	data;
		//last polled data (e.g. 500MHz)
		//(this field can be set to FLT_MAX if data is not available at
		//the moment)
	float	minLimit; 
		//minimum limit for graphs (e.g. 0MHz)
	float	maxLimit;
		//maximum limit for graphs (e.g. 2000MHz)

	DWORD	dwFlags;
		//bitmask containing combination of MAHM_SHARED_MEMORY_ENTRY_FLAG_...

	//WARNING! The following fields are valid for v2.0 and newer shared memory layouts only

	DWORD	dwGpu;
		//data source GPU index (zero based) or 0xFFFFFFFF for global data sources (e.g. Framerate)
	DWORD	dwSrcId;
		//data source ID

} MAHM_SHARED_MEMORY_ENTRY, *LPMAHM_SHARED_MEMORY_ENTRY;
/////////////////////////////////////////////////////////////////////////////
// v2.0 GPU entry (array of GPU entries follows immediately by the main array)
typedef struct MAHM_SHARED_MEMORY_GPU_ENTRY
{
	char	szGpuId[MAX_PATH];
		//GPU identifier represented in VEN_%04X&DEV_%04X&SUBSYS_%08X&REV_%02X&BUS_%d&DEV_%d&FN_%d format
		//(e.g. VEN_10DE&DEV_0A20&SUBSYS_071510DE&BUS_1&DEV_0&FN_0)

	char	szFamily[MAX_PATH];
		//GPU family (e.g. "GT216")
		//can be empty if data is not available
	char	szDevice[MAX_PATH];
		//display device description (e.g. "GeForce GT 220")
		//can be empty if data is not available
	char	szDriver[MAX_PATH];
		//display driver description (e.g. "6.14.11.9621, ForceWare 196.21")
		//can be empty if data is not available
	char	szBIOS[MAX_PATH];
		//BIOS version (e.g. 70.16.24.00.00)
		//can be empty if data is not available
	DWORD	dwMemAmount;
		//on-board memory amount in KB (e.g. 1048576)
		//can be empty if data is not available

} MAHM_SHARED_MEMORY_GPU_ENTRY, *LPMAHM_SHARED_MEMORY_GPU_ENTRY;
/////////////////////////////////////////////////////////////////////////////
#endif //_MAHM_SHARED_MEMORY_INCLUDED_