
/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ***************************/

#include "flash_eeprom.h"
#include "uart.h"
extern void Delay(u16 nCount);
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

void Flash_eeprom_readwrite_Init(void)
{
    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA); 
}


void Flash_eeprom_readwrite_Test(void)
{
    TestStatus OperationStatus;
    u32 add, start_add, stop_add =0;
    u8 WriteBuffer[FLASH_BLOCK_SIZE];
    u8 new_value[FLASH_BLOCK_SIZE] = "This is a Flash_eeprom write and read example......\
This is a Flash_eeprom write and read example....---lingguansheng---2012-2-20";
    u8 block, i=0 ;
    /* Fill the buffer in RAM */
    for (i = 0; i < FLASH_BLOCK_SIZE; i++) WriteBuffer[i] =new_value[i];
        /* Program the block 0*/
    block = 0; /* block 0 is first block of Data memory: address is 0x4000 */
    FLASH_ProgramBlock(block, FLASH_MEMTYPE_DATA, FLASH_PROGRAMMODE_STANDARD, WriteBuffer);
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

    /* Check the programmed block */
    start_add = FLASH_DATA_START_PHYSICAL_ADDRESS;
    stop_add = FLASH_DATA_START_PHYSICAL_ADDRESS + (u32)FLASH_BLOCK_SIZE;
    for (add = start_add; add < stop_add; add++)
    {
        if (FLASH_ReadByte(add) != new_value[add-start_add])
        {
            /* Error */
            OperationStatus = FAILED;
            /* OperationStatus = PASSED, if the data written/read to/from DATA EEPROM memory is correct */
            /* OperationStatus = FAILED, if the data written/read to/from DATA EEPROM memory is corrupted */
            while (1);
        }
    }
    
    for (add = start_add; add < stop_add; add++)
    {

          UART1_SendByte(FLASH_ReadByte(add));
          Delay(0xffff);
    }
}

void Flash_eeprom_Erase_Test(void)
{
  u32 add, start_add, stop_add ;
  u8 block=0;
  TestStatus OperationStatus;
    /* Check the programmed block */
  start_add = FLASH_DATA_START_PHYSICAL_ADDRESS;
  stop_add = FLASH_DATA_START_PHYSICAL_ADDRESS + (u32)FLASH_BLOCK_SIZE;
      /* Erase block 0 and verify it */
  FLASH_EraseBlock(block, FLASH_MEMTYPE_DATA);
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
    for (add = start_add; add < stop_add; add++)
    {
        if (FLASH_ReadByte(add) != 0x00)
        {
            /* Error */
            OperationStatus = FAILED;
            /* OperationStatus = PASSED, if the data written/read to/from DATA EEPROM memory is correct */
            /* OperationStatus = FAILED, if the data written/read to/from DATA EEPROM memory is corrupted */
            while (1);
        }
    }

    /* Pass */
    OperationStatus = PASSED;
    /* OperationStatus = PASSED, if the data written/read to/from DATA EEPROM memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from DATA EEPROM memory is corrupted */
    UART1_SendString("The Flash_eeprom Erase Succeed ",sizeof("The Flash_eeprom Erase Succeed"));
}

/******************* (C) COPYRIGHT 风驰电子嵌入式开发工作室 *****END OF FILE****/