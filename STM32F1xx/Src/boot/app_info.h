#pragma once 

#define APP_INFO_ADDRESS               0x08020000
#define APP_INFO                       ((app_info_t *)APP_INFO_ADDRESS)

typedef struct
{
    uint32_t magic;
    uint32_t board_id;
    uint32_t app_start_address;
    uint32_t app_size;
    uint32_t app_crc;
    uint32_t build_timestamp;
    uint32_t board_version;
    uint32_t firmware_version;

    char build_time[19];
    char app_git_commit[40];
    char app_name[50];
    char device_id;
} app_info_t;