/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
#include "system.h"
#include <string.h>

FRESULT ret = FR_OK;
uint32_t byteswritten, bytesread;                     /* File write/read counts */
uint8_t wtext[] = "This is STM32 working with FatFs\n\r"; /* File write buffer */
uint8_t rtext[100];
uint8_t workBuffer[2*_MAX_SS];
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */

  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

static void translate_fatfs_error_code_to_string(FRESULT error_code, char *string)
{
    switch (error_code) {
    case FR_OK:
        strcpy(string, "FR_OK");
        break;
    case FR_DISK_ERR:
        strcpy(string, "FR_DISK_ERR");
        break;
    case FR_INT_ERR:
        strcpy(string, "FR_INT_ERR");
        break;
    case FR_NOT_READY:
        strcpy(string, "FR_NOT_READY");
        break;
    case FR_NO_FILE:
        strcpy(string, "FR_NO_FILE");
        break;
    case FR_NO_PATH:
        strcpy(string, "FR_NO_PATH");
        break;
    case FR_INVALID_NAME:
        strcpy(string, "FR_INVALID_NAME");
        break;
    case FR_DENIED:
        strcpy(string, "FR_DENIED");
        break;
    case FR_EXIST:
        strcpy(string, "FR_EXIST");
        break;
    case FR_INVALID_OBJECT:
        strcpy(string, "FR_INVALID_OBJECT");
        break;
    case FR_WRITE_PROTECTED:
        strcpy(string, "FR_WRITE_PROTECTED");
        break;
    case FR_INVALID_DRIVE:
        strcpy(string, "FR_INVALID_DRIVE");
        break;
    case FR_NOT_ENABLED:
        strcpy(string, "FR_NOT_ENABLED");
        break;
    case FR_NO_FILESYSTEM:
        strcpy(string, "FR_NO_FILESYSTEM");
        break;
    case FR_MKFS_ABORTED:
        strcpy(string, "FR_MKFS_ABORTED");
        break;
    case FR_TIMEOUT:
        strcpy(string, "FR_TIMEOUT");
        break;
    case FR_LOCKED:
        strcpy(string, "FR_LOCKED");
        break;
    case FR_NOT_ENOUGH_CORE:
        strcpy(string, "FR_NOT_ENOUGH_CORE");
        break;
    case FR_TOO_MANY_OPEN_FILES:
        strcpy(string, "FR_TOO_MANY_OPEN_FILES");
        break;
    case FR_INVALID_PARAMETER: 
        strcpy(string, "FR_INVALID_PARAMETER");
        break;
    default:
        break;
    }
}

void create_file_example(void)
{
  char string_err[32];

  printf("[FAT]: Mounting file system\n\r");
  ret = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
  if (ret != FR_OK) {
    translate_fatfs_error_code_to_string(ret, string_err);
    printf("[FAT]: ERROR: %s\n\r", string_err);
    return;
  }
  
  printf("[FAT]: Formating file system\n\r");
  ret = f_mkfs(SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));
  if (ret != FR_OK) {
    translate_fatfs_error_code_to_string(ret, string_err);
    printf("[FAT]: ERROR: %s\n\r", string_err);
    return;
  }

  printf("[FAT]: Creating file\n\r");
  ret = f_open(&SDFile, "jon.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (ret != FR_OK) {
    translate_fatfs_error_code_to_string(ret, string_err);
    printf("[FAT]: ERROR: %s\n\r", string_err);
    return;
  }

  printf("[FAT]: Wrinting file\n\r");
//   ret = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);
  byteswritten = f_puts(wtext, &SDFile);
  if((byteswritten == 0) || (ret != FR_OK)){
    translate_fatfs_error_code_to_string(ret, string_err);
    printf("[FAT]: ERROR: %s\n\r", string_err);
    return;
  }

  printf("[FAT]: Closing file\n\r");
  f_close(&SDFile);

  /*##-11- Unlink the micro SD disk I/O driver ###############################*/
  FATFS_UnLinkDriver(SDPath);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
