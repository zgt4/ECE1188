//This is a module so the FSM can call it and record to flash the relevant information
//Needed to take the necessary functions out of Lab 10 main so we can use
//everything else where


//Initialize the flash to record information to it
//Erase everything so it is ready to be written to
void Debug_FlashInit(void){
  // write this as part of Lab 10
    Flash_Init(48);
    uint32_t address;
    address = FLASH_BANK1_MIN;

    while(address < FLASH_BANK1_MAX)
    {
        Flash_Erase(address);
        address = address + 64;
    }
}

//Record everything until the flash bank fills up
void Debug_FlashRecord(uint16_t *pt){
  // write this as part of Lab 10
    uint32_t address;
    address = FLASH_BANK1_MIN;
    while(*(uint32_t*)address != 0xFFFFFFFF){ // find first free block
      address = address + 64;
      if(address > FLASH_BANK1_MAX) //Flash is full
      {
          return;
      }
    }
    Flash_FastWrite((uint32_t *)pt, address, 16); //Write 64 bytes
}
