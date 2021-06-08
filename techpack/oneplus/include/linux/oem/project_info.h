#ifndef _PROJECT_INFO_H_
#define _PROJECT_INFO_H_ 1

/*******SECURE_BOOTn = 0x00786078+ 0x4*n, n=[1..14]******/
#define SECURE_BOOT_BASE	0x00786078
#define SECURE_BOOT1		(SECURE_BOOT_BASE + 0x4 * 1)
#define BUF_SIZE		64

#define SMEM_PROJECT_INFO	136
#define NONDEFINE		-1

#include <linux/string.h>
#include <linux/kallsyms.h>

struct project_info {
	char project_name[8];
	char project_codename[20];
	char reservename[12];
	uint32_t prj_version;
	uint32_t hw_version;
	uint32_t rf_v1;
	uint32_t rf_v2;
	uint32_t rf_v3;
	uint32_t uart_boot_mode;
	uint32_t platform_id;
	uint32_t ddr_manufacture_info;
	uint32_t ddr_row;
	uint32_t ddr_column;
	uint32_t ddr_fw_version;
	uint32_t ddr_reserve_info;
	uint32_t reserve01; /*reserve for feture use*/
	uint32_t reserve02;
	uint32_t reserve03;
	uint32_t reserve04;
	uint32_t reserve05;
	uint32_t reserve06;
	uint32_t reserve07;
	uint32_t reserve08;
	uint32_t reserve09;
};

struct component_info {
	char *version;
	char *manufacture;
};

enum {
	HW_VERSION__UNKNOWN,
	HW_VERSION__11 = 11, //all EVB
	HW_VERSION__12, //T0
};

enum COMPONENT_TYPE {
	DDR,
	EMMC,
	F_CAMERA,
	SECOND_F_CAMERA,
	R_CAMERA,
	SECOND_R_CAMERA,
	THIRD_R_CAMERA,
	FORTH_R_CAMERA,
	R_MODULE,
	F_MODULE,
	R_OIS,
	SECOND_R_OIS,
	TP,
	LCD,
	WCN,
	I_SENSOR,
	G_SENSOR,
	M_SENSOR,
	GYRO,
	BACKLIGHT,
	MAINBOARD,
	/*Add new component here*/
	FINGERPRINTS,
	TOUCH_KEY,
	UFS,
	ABOARD,
	NFC,
	FAST_CHARGE,
	WIRELESS_CHARGE,
	CPU,
	RF_VERSION,
	COMPONENT_MAX,
};

int push_component_info(enum COMPONENT_TYPE type, char *version,
			char *manufacture);
int reset_component_info(enum COMPONENT_TYPE type);
uint32_t get_hw_version(void);

#endif
