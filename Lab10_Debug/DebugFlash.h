//Header file for the debug flash file


//Initialize the flash to record information to it
//Erase everything so it is ready to be written to
void Debug_FlashInit(void);

//Record everything until the flash bank fills up
void Debug_FlashRecord(uint16_t *pt);
