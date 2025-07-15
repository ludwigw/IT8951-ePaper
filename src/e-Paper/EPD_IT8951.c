/**
 * @file EPD_IT8951.c
 * @brief Implementation of the IT8951 e-Paper controller driver.
 *
 * Provides functions for initializing, refreshing, and controlling e-Paper displays
 * using the IT8951 controller. Supports multiple bit depths, partial updates, and
 * platform-agnostic hardware abstraction.
 *
 * @author Waveshare
 * @date 2019-09-17
 */
#include "EPD_IT8951.h"
#include <time.h>
#include <stdlib.h> // Added for getenv

// External variables for display configuration
extern UBYTE isColor;

// Global variable for 4-byte alignment (default to false)
bool Four_Byte_Align = false;

//basic mode definition
UBYTE INIT_Mode = 0;
UBYTE GC16_Mode = 2;
//A2_Mode's value is not fixed, is decide by firmware's LUT 
UBYTE A2_Mode = 6;

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_IT8951_Reset(void)
{
    EPD_LOG_INFO("Starting hardware reset sequence");
    EPD_LOG_DEBUG("Setting RST_PIN HIGH");
    DEV_Digital_Write(EPD_RST_PIN, HIGH);
    EPD_LOG_DEBUG("Delaying 200ms");
    DEV_Delay_ms(200);
    EPD_LOG_DEBUG("Setting RST_PIN LOW");
    DEV_Digital_Write(EPD_RST_PIN, LOW);
    EPD_LOG_DEBUG("Delaying 10ms");
    DEV_Delay_ms(10);
    EPD_LOG_DEBUG("Setting RST_PIN HIGH");
    DEV_Digital_Write(EPD_RST_PIN, HIGH);
    EPD_LOG_DEBUG("Delaying 200ms");
    DEV_Delay_ms(200);
    EPD_LOG_INFO("Hardware reset sequence completed");
}


/******************************************************************************
function :	Wait until the busy_pin goes HIGH
parameter:
******************************************************************************/
static void EPD_IT8951_ReadBusy(void)
{
	// Debug("Busy ------\r\n");
    UBYTE Busy_State = DEV_Digital_Read(EPD_BUSY_PIN);
    //0: busy, 1: idle
    while(Busy_State == 0) {
        Busy_State = DEV_Digital_Read(EPD_BUSY_PIN);
    }
	// Debug("Busy Release ------\r\n");
}


/******************************************************************************
function :	write command
parameter:  command
******************************************************************************/
static void EPD_IT8951_WriteCommand(UWORD Command)
{
    //Set Preamble for Write Command
    UWORD Write_Preamble = 0x6000;
    
    EPD_IT8951_ReadBusy();

    DEV_Digital_Write(EPD_CS_PIN, LOW);

    EPD_LOG_TRACE("Sending command 0x%04X", Command);
    DEV_SPI_WriteByte(Write_Preamble>>8);
    DEV_SPI_WriteByte(Write_Preamble);
    
    EPD_IT8951_ReadBusy();    
    
    DEV_SPI_WriteByte(Command>>8);
    DEV_SPI_WriteByte(Command);
    
    DEV_Digital_Write(EPD_CS_PIN, HIGH);
}


/******************************************************************************
function :	write data
parameter:  data
******************************************************************************/
static void EPD_IT8951_WriteData(UWORD Data)
{
    //Set Preamble for Write Command
	UWORD Write_Preamble = 0x0000;

    EPD_IT8951_ReadBusy();

    DEV_Digital_Write(EPD_CS_PIN, LOW);

	DEV_SPI_WriteByte(Write_Preamble>>8);
	DEV_SPI_WriteByte(Write_Preamble);

    EPD_IT8951_ReadBusy();

	DEV_SPI_WriteByte(Data>>8);
	DEV_SPI_WriteByte(Data);

    DEV_Digital_Write(EPD_CS_PIN, HIGH);
}


/******************************************************************************
function :	write multi data
parameter:  data
******************************************************************************/
static void EPD_IT8951_WriteMuitiData(UWORD* Data_Buf, UDOUBLE Length)
{
    //Set Preamble for Write Command
	UWORD Write_Preamble = 0x0000;

    EPD_IT8951_ReadBusy();

    DEV_Digital_Write(EPD_CS_PIN, LOW);

	DEV_SPI_WriteByte(Write_Preamble>>8);
	DEV_SPI_WriteByte(Write_Preamble);

    EPD_IT8951_ReadBusy();

    for(UDOUBLE i = 0; i<Length; i++)
    {
	    DEV_SPI_WriteByte(Data_Buf[i]>>8);
	    DEV_SPI_WriteByte(Data_Buf[i]);
    }
    DEV_Digital_Write(EPD_CS_PIN, HIGH);
}



/******************************************************************************
function :	read data
parameter:  data
******************************************************************************/
static UWORD EPD_IT8951_ReadData()
{
    UWORD ReadData;
	UWORD Write_Preamble = 0x1000;
    UWORD Read_Dummy;

    EPD_IT8951_ReadBusy();

    DEV_Digital_Write(EPD_CS_PIN, LOW);

	DEV_SPI_WriteByte(Write_Preamble>>8);
	DEV_SPI_WriteByte(Write_Preamble);

    EPD_IT8951_ReadBusy();

    //dummy
    Read_Dummy = DEV_SPI_ReadByte()<<8;
    Read_Dummy |= DEV_SPI_ReadByte();

    EPD_IT8951_ReadBusy();

    ReadData = DEV_SPI_ReadByte()<<8;
    ReadData |= DEV_SPI_ReadByte();

    DEV_Digital_Write(EPD_CS_PIN, HIGH);

    return ReadData;
}




/******************************************************************************
function :	read multi data
parameter:  data
******************************************************************************/
static void EPD_IT8951_ReadMultiData(UWORD* Data_Buf, UDOUBLE Length)
{
	UWORD Write_Preamble = 0x1000;
    UWORD Read_Dummy;

    EPD_IT8951_ReadBusy();

    DEV_Digital_Write(EPD_CS_PIN, LOW);

	DEV_SPI_WriteByte(Write_Preamble>>8);
	DEV_SPI_WriteByte(Write_Preamble);

    EPD_IT8951_ReadBusy();

    //dummy
    Read_Dummy = DEV_SPI_ReadByte()<<8;
    Read_Dummy |= DEV_SPI_ReadByte();

    EPD_IT8951_ReadBusy();

    for(UDOUBLE i = 0; i<Length; i++)
    {
	    Data_Buf[i] = DEV_SPI_ReadByte()<<8;
	    Data_Buf[i] |= DEV_SPI_ReadByte();
    }

    DEV_Digital_Write(EPD_CS_PIN, HIGH);
}



/******************************************************************************
function:	write multi arg
parameter:	data
description:	some situation like this:
* 1 commander     0    argument
* 1 commander     1    argument
* 1 commander   multi  argument
******************************************************************************/
static void EPD_IT8951_WriteMultiArg(UWORD Arg_Cmd, UWORD* Arg_Buf, UWORD Arg_Num)
{
     //Send Cmd code
     EPD_IT8951_WriteCommand(Arg_Cmd);
     //Send Data
     for(UWORD i=0; i<Arg_Num; i++)
     {
         EPD_IT8951_WriteData(Arg_Buf[i]);
     }
}


/******************************************************************************
function :	Cmd4 ReadReg
parameter:  
******************************************************************************/
static UWORD EPD_IT8951_ReadReg(UWORD Reg_Address)
{
    UWORD Reg_Value;
    EPD_IT8951_WriteCommand(IT8951_TCON_REG_RD);
    EPD_IT8951_WriteData(Reg_Address);
    Reg_Value =  EPD_IT8951_ReadData();
    EPD_LOG_TRACE("ReadReg(0x%04X) = 0x%04X", Reg_Address, Reg_Value);
    return Reg_Value;
}



/******************************************************************************
function :	Cmd5 WriteReg
parameter:  
******************************************************************************/
static void EPD_IT8951_WriteReg(UWORD Reg_Address,UWORD Reg_Value)
{
    EPD_IT8951_WriteCommand(IT8951_TCON_REG_WR);
    EPD_IT8951_WriteData(Reg_Address);
    EPD_IT8951_WriteData(Reg_Value);
}



/******************************************************************************
function :	get VCOM
parameter:  
******************************************************************************/
static UWORD EPD_IT8951_GetVCOM(void)
{
    UWORD VCOM;
    EPD_IT8951_WriteCommand(USDEF_I80_CMD_VCOM);
    EPD_IT8951_WriteData(0x0000);
    VCOM =  EPD_IT8951_ReadData();
    return VCOM;
}



/******************************************************************************
function :	set VCOM
parameter:  
******************************************************************************/
static void EPD_IT8951_SetVCOM(UWORD VCOM)
{
    EPD_IT8951_WriteCommand(USDEF_I80_CMD_VCOM);
    EPD_IT8951_WriteData(0x0001);
    EPD_IT8951_WriteData(VCOM);
}



/******************************************************************************
function :	Cmd10 LD_IMG
parameter:  
******************************************************************************/
static void EPD_IT8951_LoadImgStart( IT8951_Load_Img_Info* Load_Img_Info )
{
    UWORD Args;
    Args = (\
        Load_Img_Info->Endian_Type<<8 | \
        Load_Img_Info->Pixel_Format<<4 | \
        Load_Img_Info->Rotate\
    );
    EPD_IT8951_WriteCommand(IT8951_TCON_LD_IMG);
    EPD_IT8951_WriteData(Args);
}


/******************************************************************************
function :	Cmd11 LD_IMG_Area
parameter:  
******************************************************************************/
static void EPD_IT8951_LoadImgAreaStart( IT8951_Load_Img_Info* Load_Img_Info, IT8951_Area_Img_Info* Area_Img_Info )
{
    UWORD Args[5];
    Args[0] = (\
        Load_Img_Info->Endian_Type<<8 | \
        Load_Img_Info->Pixel_Format<<4 | \
        Load_Img_Info->Rotate\
    );
    Args[1] = Area_Img_Info->Area_X;
    Args[2] = Area_Img_Info->Area_Y;
    Args[3] = Area_Img_Info->Area_W;
    Args[4] = Area_Img_Info->Area_H;
    EPD_IT8951_WriteMultiArg(IT8951_TCON_LD_IMG_AREA, Args,5);
}

/******************************************************************************
function :	Cmd12 LD_IMG_End
parameter:  
******************************************************************************/
static void EPD_IT8951_LoadImgEnd(void)
{
    EPD_IT8951_WriteCommand(IT8951_TCON_LD_IMG_END);
}


/******************************************************************************
function :	EPD_IT8951_Get_System_Info
parameter:  
******************************************************************************/
static void EPD_IT8951_GetSystemInfo(void* Buf)
{
    EPD_LOG_DEBUG("Starting system info retrieval");
    IT8951_Dev_Info* Dev_Info; 

    EPD_LOG_DEBUG("Sending GET_DEV_INFO command");
    EPD_IT8951_WriteCommand(USDEF_I80_CMD_GET_DEV_INFO);

    EPD_LOG_DEBUG("Reading device info data");
    EPD_IT8951_ReadMultiData((UWORD*)Buf, sizeof(IT8951_Dev_Info)/2);

    Dev_Info = (IT8951_Dev_Info*)Buf;
    EPD_LOG_INFO("Retrieved panel size: %dx%d", Dev_Info->Panel_W, Dev_Info->Panel_H);
    EPD_LOG_DEBUG("Memory address: 0x%X", Dev_Info->Memory_Addr_L | (Dev_Info->Memory_Addr_H << 16));
    EPD_LOG_DEBUG("FW Version: %s", (UBYTE*)Dev_Info->FW_Version);
    EPD_LOG_DEBUG("LUT Version: %s", (UBYTE*)Dev_Info->LUT_Version);
    EPD_LOG_DEBUG("System info retrieval completed");
}


/******************************************************************************
function :	EPD_IT8951_Set_Target_Memory_Addr
parameter:  
******************************************************************************/
static void EPD_IT8951_SetTargetMemoryAddr(UDOUBLE Target_Memory_Addr)
{
	UWORD WordH = (UWORD)((Target_Memory_Addr >> 16) & 0x0000FFFF);
	UWORD WordL = (UWORD)( Target_Memory_Addr & 0x0000FFFF);

    EPD_IT8951_WriteReg(LISAR+2, WordH);
    EPD_IT8951_WriteReg(LISAR  , WordL);
}


/******************************************************************************
function :	EPD_IT8951_WaitForDisplayReady
parameter:  
******************************************************************************/
static void EPD_IT8951_WaitForDisplayReady(void)
{
    //Check IT8951 Register LUTAFSR => NonZero Busy, Zero - Free
    EPD_LOG_DEBUG("Waiting for display to become ready...");
    int timeout = 0;
    const int max_timeout = 10000; // 10 second timeout
    
    while( EPD_IT8951_ReadReg(LUTAFSR) )
    {
        //wait in idle state
        timeout++;
        if (timeout > max_timeout) {
            EPD_LOG_ERROR("Display ready timeout - LUTAFSR register stuck at non-zero value");
            break;
        }
        if (timeout % 1000 == 0) {
            EPD_LOG_WARN("Display still busy after %d ms", timeout);
        }
        usleep(1000); // Sleep 1ms to avoid busy-waiting
    }
    
    if (timeout <= max_timeout) {
        EPD_LOG_DEBUG("Display became ready after %d ms", timeout);
    }
}





/******************************************************************************
function :	EPD_IT8951_HostAreaPackedPixelWrite_1bp
parameter:  
******************************************************************************/
static void EPD_IT8951_HostAreaPackedPixelWrite_1bp(IT8951_Load_Img_Info*Load_Img_Info,IT8951_Area_Img_Info*Area_Img_Info, bool Packed_Write)
{
    UWORD Source_Buffer_Width, Source_Buffer_Height;
    UWORD Source_Buffer_Length;

    UWORD* Source_Buffer = (UWORD*)Load_Img_Info->Source_Buffer_Addr;
    EPD_IT8951_SetTargetMemoryAddr(Load_Img_Info->Target_Memory_Addr);
    EPD_IT8951_LoadImgAreaStart(Load_Img_Info,Area_Img_Info);

    //from byte to word
    //use 8bp to display 1bp, so here, divide by 2, because every byte has full bit.
    Source_Buffer_Width = Area_Img_Info->Area_W/2;
    Source_Buffer_Height = Area_Img_Info->Area_H;
    Source_Buffer_Length = Source_Buffer_Width * Source_Buffer_Height;
    
    if(Packed_Write == true)
    {
        EPD_IT8951_WriteMuitiData(Source_Buffer, Source_Buffer_Length);
    }
    else
    {
        for(UDOUBLE i=0; i<Source_Buffer_Height; i++)
        {
            for(UDOUBLE j=0; j<Source_Buffer_Width; j++)
            {
                EPD_IT8951_WriteData(*Source_Buffer);
                Source_Buffer++;
            }
        }
    }

    EPD_IT8951_LoadImgEnd();
}





/******************************************************************************
function :	EPD_IT8951_HostAreaPackedPixelWrite_2bp
parameter:  
******************************************************************************/
static void EPD_IT8951_HostAreaPackedPixelWrite_2bp(IT8951_Load_Img_Info*Load_Img_Info, IT8951_Area_Img_Info*Area_Img_Info, bool Packed_Write)
{
    UWORD Source_Buffer_Width, Source_Buffer_Height;
    UWORD Source_Buffer_Length;

    UWORD* Source_Buffer = (UWORD*)Load_Img_Info->Source_Buffer_Addr;
    EPD_IT8951_SetTargetMemoryAddr(Load_Img_Info->Target_Memory_Addr);
    EPD_IT8951_LoadImgAreaStart(Load_Img_Info,Area_Img_Info);

    //from byte to word
    Source_Buffer_Width = (Area_Img_Info->Area_W*2/8)/2;
    Source_Buffer_Height = Area_Img_Info->Area_H;
    Source_Buffer_Length = Source_Buffer_Width * Source_Buffer_Height;

    if(Packed_Write == true)
    {
        EPD_IT8951_WriteMuitiData(Source_Buffer, Source_Buffer_Length);
    }
    else
    {
        for(UDOUBLE i=0; i<Source_Buffer_Height; i++)
        {
            for(UDOUBLE j=0; j<Source_Buffer_Width; j++)
            {
                EPD_IT8951_WriteData(*Source_Buffer);
                Source_Buffer++;
            }
        }
    }

    EPD_IT8951_LoadImgEnd();
}





/******************************************************************************
function :	EPD_IT8951_HostAreaPackedPixelWrite_4bp
parameter:  
******************************************************************************/
static void EPD_IT8951_HostAreaPackedPixelWrite_4bp(IT8951_Load_Img_Info*Load_Img_Info, IT8951_Area_Img_Info*Area_Img_Info, bool Packed_Write)
{
    EPD_LOG_DEBUG("HostAreaPackedPixelWrite_4bp: Area %dx%d at (%d,%d), Packed=%d", 
                  Area_Img_Info->Area_W, Area_Img_Info->Area_H, 
                  Area_Img_Info->Area_X, Area_Img_Info->Area_Y, Packed_Write);
    
    UWORD Source_Buffer_Width, Source_Buffer_Height;
    UWORD Source_Buffer_Length;
	
    UWORD* Source_Buffer = (UWORD*)Load_Img_Info->Source_Buffer_Addr;
    EPD_LOG_DEBUG("Setting target memory address");
    EPD_IT8951_SetTargetMemoryAddr(Load_Img_Info->Target_Memory_Addr);
    EPD_LOG_DEBUG("Starting load image area");
    EPD_IT8951_LoadImgAreaStart(Load_Img_Info,Area_Img_Info);

    //from byte to word
    Source_Buffer_Width = (Area_Img_Info->Area_W*4/8)/2;
    Source_Buffer_Height = Area_Img_Info->Area_H;
    Source_Buffer_Length = Source_Buffer_Width * Source_Buffer_Height;
    
    EPD_LOG_DEBUG("Buffer dimensions: %dx%d, length=%d", Source_Buffer_Width, Source_Buffer_Height, Source_Buffer_Length);

    if(Packed_Write == true)
    {
        EPD_LOG_DEBUG("Using packed write for %d words", Source_Buffer_Length);
        EPD_IT8951_WriteMuitiData(Source_Buffer, Source_Buffer_Length);
        EPD_LOG_DEBUG("Packed write completed");
    }
    else
    {
        EPD_LOG_DEBUG("Using individual writes for %dx%d pixels", Source_Buffer_Width, Source_Buffer_Height);
        for(UDOUBLE i=0; i<Source_Buffer_Height; i++)
        {
            for(UDOUBLE j=0; j<Source_Buffer_Width; j++)
            {
                EPD_IT8951_WriteData(*Source_Buffer);
                Source_Buffer++;
            }
        }
        EPD_LOG_DEBUG("Individual writes completed");
    }

    EPD_LOG_DEBUG("Ending load image");
    EPD_IT8951_LoadImgEnd();
    EPD_LOG_DEBUG("HostAreaPackedPixelWrite_4bp completed");
}







/******************************************************************************
function :	EPD_IT8951_HostAreaPackedPixelWrite_8bp
parameter:  
Precautions: Can't Packed Write
******************************************************************************/
static void EPD_IT8951_HostAreaPackedPixelWrite_8bp(IT8951_Load_Img_Info*Load_Img_Info,IT8951_Area_Img_Info*Area_Img_Info)
{
    UWORD Source_Buffer_Width, Source_Buffer_Height;

    UWORD* Source_Buffer = (UWORD*)Load_Img_Info->Source_Buffer_Addr;
    EPD_IT8951_SetTargetMemoryAddr(Load_Img_Info->Target_Memory_Addr);
    EPD_IT8951_LoadImgAreaStart(Load_Img_Info,Area_Img_Info);

    //from byte to word
    Source_Buffer_Width = (Area_Img_Info->Area_W*8/8)/2;
    Source_Buffer_Height = Area_Img_Info->Area_H;

    for(UDOUBLE i=0; i<Source_Buffer_Height; i++)
    {
        for(UDOUBLE j=0; j<Source_Buffer_Width; j++)
        {
            EPD_IT8951_WriteData(*Source_Buffer);
            Source_Buffer++;
        }
    }
    EPD_IT8951_LoadImgEnd();
}






/******************************************************************************
function :	EPD_IT8951_Display_Area
parameter:  
******************************************************************************/
static void EPD_IT8951_Display_Area(UWORD X,UWORD Y,UWORD W,UWORD H,UWORD Mode)
{
    UWORD Args[5];
    Args[0] = X;
    Args[1] = Y;
    Args[2] = W;
    Args[3] = H;
    Args[4] = Mode;
    //0x0034
    EPD_IT8951_WriteMultiArg(USDEF_I80_CMD_DPY_AREA, Args,5);
}



/******************************************************************************
function :	EPD_IT8951_Display_AreaBuf
parameter:  
******************************************************************************/
static void EPD_IT8951_Display_AreaBuf(UWORD X,UWORD Y,UWORD W,UWORD H,UWORD Mode, UDOUBLE Target_Memory_Addr)
{
    UWORD Args[7];
    Args[0] = X;
    Args[1] = Y;
    Args[2] = W;
    Args[3] = H;
    Args[4] = Mode;
    Args[5] = (UWORD)Target_Memory_Addr;
    Args[6] = (UWORD)(Target_Memory_Addr>>16);
    //0x0037
    EPD_IT8951_WriteMultiArg(USDEF_I80_CMD_DPY_BUF_AREA, Args,7); 
}



/******************************************************************************
function :	EPD_IT8951_Display_1bp
parameter:  
******************************************************************************/
static void EPD_IT8951_Display_1bp(UWORD X, UWORD Y, UWORD W, UWORD H, UWORD Mode,UDOUBLE Target_Memory_Addr, UBYTE Back_Gray_Val,UBYTE Front_Gray_Val)
{
    //Set Display mode to 1 bpp mode - Set 0x18001138 Bit[18](0x1800113A Bit[2])to 1
    EPD_IT8951_WriteReg(UP1SR+2, EPD_IT8951_ReadReg(UP1SR+2) | (1<<2) );

    EPD_IT8951_WriteReg(BGVR, (Front_Gray_Val<<8) | Back_Gray_Val);

    if(Target_Memory_Addr == 0)
    {
        EPD_IT8951_Display_Area(X,Y,W,H,Mode);
    }
    else
    {
        EPD_IT8951_Display_AreaBuf(X,Y,W,H,Mode,Target_Memory_Addr);
    }
    
    EPD_IT8951_WaitForDisplayReady();

    EPD_IT8951_WriteReg(UP1SR+2, EPD_IT8951_ReadReg(UP1SR+2) & ~(1<<2) );
}


/******************************************************************************
function :	Enhanced driving capability
parameter:  Enhanced driving capability for IT8951, in case the blurred display effect
******************************************************************************/
void Enhance_Driving_Capability(void)
{
    UWORD RegValue = EPD_IT8951_ReadReg(0x0038);
    Debug("The reg value before writing is %x\r\n", RegValue);

    EPD_IT8951_WriteReg(0x0038, 0x0602);

    RegValue = EPD_IT8951_ReadReg(0x0038);
    Debug("The reg value after writing is %x\r\n", RegValue);
}




/******************************************************************************
function :	Cmd1 SYS_RUN
parameter:  Run the system
******************************************************************************/
void EPD_IT8951_SystemRun(void)
{
    EPD_LOG_DEBUG("Sending SYS_RUN command");
    EPD_IT8951_WriteCommand(IT8951_TCON_SYS_RUN);
    EPD_LOG_DEBUG("SYS_RUN command sent");
}


/******************************************************************************
function :	Cmd2 STANDBY
parameter:  Standby
******************************************************************************/
void EPD_IT8951_Standby(void)
{
    EPD_IT8951_WriteCommand(IT8951_TCON_STANDBY);
}


/******************************************************************************
function :	Cmd3 SLEEP
parameter:  Sleep
******************************************************************************/
void EPD_IT8951_Sleep(void)
{
    EPD_IT8951_WriteCommand(IT8951_TCON_SLEEP);
}


/******************************************************************************
function :	EPD_IT8951_Init
parameter:  
******************************************************************************/
IT8951_Dev_Info EPD_IT8951_Init(UWORD VCOM)
{
    EPD_LOG_INFO("Starting initialization with VCOM=%d", VCOM);
    IT8951_Dev_Info Dev_Info;

    EPD_LOG_DEBUG("Calling EPD_IT8951_Reset()");
    EPD_IT8951_Reset();
    EPD_LOG_DEBUG("Reset completed");

    EPD_LOG_DEBUG("Calling EPD_IT8951_SystemRun()");
    EPD_IT8951_SystemRun();
    EPD_LOG_DEBUG("System run completed");

    EPD_LOG_DEBUG("Calling EPD_IT8951_GetSystemInfo()");
    EPD_IT8951_GetSystemInfo(&Dev_Info);
    EPD_LOG_INFO("Got system info - Panel: %dx%d", Dev_Info.Panel_W, Dev_Info.Panel_H);
    
    EPD_LOG_DEBUG("Enabling pack write");
    EPD_IT8951_WriteReg(I80CPCR,0x0001);
    EPD_LOG_DEBUG("Pack write enabled");

    EPD_LOG_DEBUG("Getting current VCOM");
    UWORD current_vcom = EPD_IT8951_GetVCOM();
    EPD_LOG_DEBUG("Current VCOM=%d, requested VCOM=%d", current_vcom, VCOM);
    
    if(VCOM != current_vcom)
    {
        EPD_LOG_INFO("Setting VCOM to %d", VCOM);
        EPD_IT8951_SetVCOM(VCOM);
        UWORD new_vcom = EPD_IT8951_GetVCOM();
        EPD_LOG_INFO("VCOM set to %d", new_vcom);
    }
    else
    {
        EPD_LOG_DEBUG("VCOM already set correctly");
    }
    
    EPD_LOG_INFO("Initialization completed successfully");
    return Dev_Info;
}


/******************************************************************************
function :	EPD_IT8951_Clear_Refresh
parameter:  
******************************************************************************/
void EPD_IT8951_Clear_Refresh(IT8951_Dev_Info Dev_Info,UDOUBLE Target_Memory_Addr, UWORD Mode)
{

    UDOUBLE ImageSize = ((Dev_Info.Panel_W * 4 % 8 == 0)? (Dev_Info.Panel_W * 4 / 8 ): (Dev_Info.Panel_W * 4 / 8 + 1)) * Dev_Info.Panel_H;
    UBYTE* Frame_Buf = malloc (ImageSize);
    memset(Frame_Buf, 0xFF, ImageSize);


    IT8951_Load_Img_Info Load_Img_Info;
    IT8951_Area_Img_Info Area_Img_Info;

    EPD_IT8951_WaitForDisplayReady();

    Load_Img_Info.Source_Buffer_Addr = Frame_Buf;
    Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
    Load_Img_Info.Pixel_Format = IT8951_4BPP;
    Load_Img_Info.Rotate =  IT8951_ROTATE_0;
    Load_Img_Info.Target_Memory_Addr = Target_Memory_Addr;

    Area_Img_Info.Area_X = 0;
    Area_Img_Info.Area_Y = 0;
    Area_Img_Info.Area_W = Dev_Info.Panel_W;
    Area_Img_Info.Area_H = Dev_Info.Panel_H;

    EPD_IT8951_HostAreaPackedPixelWrite_4bp(&Load_Img_Info, &Area_Img_Info, false);

    EPD_IT8951_Display_Area(0, 0, Dev_Info.Panel_W, Dev_Info.Panel_H, Mode);

    free(Frame_Buf);
    Frame_Buf = NULL;
}


/******************************************************************************
function :	EPD_IT8951_1bp_Refresh
parameter:
******************************************************************************/
void EPD_IT8951_1bp_Refresh(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, UBYTE Mode, UDOUBLE Target_Memory_Addr, bool Packed_Write)
{
    IT8951_Load_Img_Info Load_Img_Info;
    IT8951_Area_Img_Info Area_Img_Info;

    EPD_IT8951_WaitForDisplayReady();

    Load_Img_Info.Source_Buffer_Addr = Frame_Buf;
    Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
    //Use 8bpp to set 1bpp
    Load_Img_Info.Pixel_Format = IT8951_8BPP;
    Load_Img_Info.Rotate =  IT8951_ROTATE_0;
    Load_Img_Info.Target_Memory_Addr = Target_Memory_Addr;

    Area_Img_Info.Area_X = X/8;
    Area_Img_Info.Area_Y = Y;
    Area_Img_Info.Area_W = W/8;
    Area_Img_Info.Area_H = H;


    //clock_t start, finish;
    //double duration;

    //start = clock();

    EPD_IT8951_HostAreaPackedPixelWrite_1bp(&Load_Img_Info, &Area_Img_Info, Packed_Write);

    //finish = clock();
    //duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//Debug( "Write occupy %f second\n", duration );

    //start = clock();

    EPD_IT8951_Display_1bp(X,Y,W,H,Mode,Target_Memory_Addr,0xF0,0x00);

    //finish = clock();
    //duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//Debug( "Show occupy %f second\n", duration );
}



/******************************************************************************
function :	EPD_IT8951_1bp_Multi_Frame_Write
parameter:  
******************************************************************************/
void EPD_IT8951_1bp_Multi_Frame_Write(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H,UDOUBLE Target_Memory_Addr, bool Packed_Write)
{
    IT8951_Load_Img_Info Load_Img_Info;
    IT8951_Area_Img_Info Area_Img_Info;

    EPD_IT8951_WaitForDisplayReady();

    Load_Img_Info.Source_Buffer_Addr = Frame_Buf;
    Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
    //Use 8bpp to set 1bpp
    Load_Img_Info.Pixel_Format = IT8951_8BPP;
    Load_Img_Info.Rotate =  IT8951_ROTATE_0;
    Load_Img_Info.Target_Memory_Addr = Target_Memory_Addr;

    Area_Img_Info.Area_X = X/8;
    Area_Img_Info.Area_Y = Y;
    Area_Img_Info.Area_W = W/8;
    Area_Img_Info.Area_H = H;
    
    EPD_IT8951_HostAreaPackedPixelWrite_1bp(&Load_Img_Info, &Area_Img_Info,Packed_Write);
}




/******************************************************************************
function :	EPD_IT8951_1bp_Multi_Frame_Refresh
parameter:  
******************************************************************************/
void EPD_IT8951_1bp_Multi_Frame_Refresh(UWORD X, UWORD Y, UWORD W, UWORD H,UDOUBLE Target_Memory_Addr)
{
    EPD_IT8951_WaitForDisplayReady();

    EPD_IT8951_Display_1bp(X,Y,W,H, A2_Mode,Target_Memory_Addr,0xF0,0x00);
}




/******************************************************************************
function :	EPD_IT8951_2bp_Refresh
parameter:  
******************************************************************************/
void EPD_IT8951_2bp_Refresh(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, bool Hold, UDOUBLE Target_Memory_Addr, bool Packed_Write)
{
    IT8951_Load_Img_Info Load_Img_Info;
    IT8951_Area_Img_Info Area_Img_Info;

    EPD_IT8951_WaitForDisplayReady();

    Load_Img_Info.Source_Buffer_Addr = Frame_Buf;
    Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
    Load_Img_Info.Pixel_Format = IT8951_2BPP;
    Load_Img_Info.Rotate =  IT8951_ROTATE_0;
    Load_Img_Info.Target_Memory_Addr = Target_Memory_Addr;

    Area_Img_Info.Area_X = X;
    Area_Img_Info.Area_Y = Y;
    Area_Img_Info.Area_W = W;
    Area_Img_Info.Area_H = H;

    EPD_IT8951_HostAreaPackedPixelWrite_2bp(&Load_Img_Info, &Area_Img_Info,Packed_Write);

    if(Hold == true)
    {
        EPD_IT8951_Display_Area(X,Y,W,H, GC16_Mode);
    }
    else
    {
        EPD_IT8951_Display_AreaBuf(X,Y,W,H, GC16_Mode,Target_Memory_Addr);
    }
}




/******************************************************************************
function :	EPD_IT8951_4bp_Refresh
parameter:  
******************************************************************************/
void EPD_IT8951_4bp_Refresh(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, bool Hold, UDOUBLE Target_Memory_Addr, bool Packed_Write)
{
    EPD_LOG_DEBUG("Starting 4bp refresh: %dx%d at (%d,%d), Hold=%d", W, H, X, Y, Hold);
    IT8951_Load_Img_Info Load_Img_Info;
    IT8951_Area_Img_Info Area_Img_Info;

    EPD_IT8951_WaitForDisplayReady();

    Load_Img_Info.Source_Buffer_Addr = Frame_Buf;
    Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
    Load_Img_Info.Pixel_Format = IT8951_4BPP;
    Load_Img_Info.Rotate =  IT8951_ROTATE_0;
    Load_Img_Info.Target_Memory_Addr = Target_Memory_Addr;

    Area_Img_Info.Area_X = X;
    Area_Img_Info.Area_Y = Y;
    Area_Img_Info.Area_W = W;
    Area_Img_Info.Area_H = H;

    EPD_LOG_DEBUG("Calling HostAreaPackedPixelWrite_4bp");
    EPD_IT8951_HostAreaPackedPixelWrite_4bp(&Load_Img_Info, &Area_Img_Info, Packed_Write);
    EPD_LOG_DEBUG("HostAreaPackedPixelWrite_4bp completed");

    if(Hold == true)
    {
        EPD_LOG_DEBUG("Calling Display_Area");
        EPD_IT8951_Display_Area(X,Y,W,H, GC16_Mode);
        EPD_LOG_DEBUG("Display_Area completed");
    }
    else
    {
        EPD_LOG_DEBUG("Calling Display_AreaBuf");
        EPD_IT8951_Display_AreaBuf(X,Y,W,H, GC16_Mode,Target_Memory_Addr);
        EPD_LOG_DEBUG("Display_AreaBuf completed");
    }
    EPD_LOG_DEBUG("4bp refresh completed");
}


/******************************************************************************
function :	EPD_IT8951_8bp_Refresh
parameter:  
******************************************************************************/
void EPD_IT8951_8bp_Refresh(UBYTE *Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, bool Hold, UDOUBLE Target_Memory_Addr)
{
    IT8951_Load_Img_Info Load_Img_Info;
    IT8951_Area_Img_Info Area_Img_Info;

    EPD_IT8951_WaitForDisplayReady();

    Load_Img_Info.Source_Buffer_Addr = Frame_Buf;
    Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
    Load_Img_Info.Pixel_Format = IT8951_8BPP;
    Load_Img_Info.Rotate =  IT8951_ROTATE_0;
    Load_Img_Info.Target_Memory_Addr = Target_Memory_Addr;

    Area_Img_Info.Area_X = X;
    Area_Img_Info.Area_Y = Y;
    Area_Img_Info.Area_W = W;
    Area_Img_Info.Area_H = H;

    EPD_IT8951_HostAreaPackedPixelWrite_8bp(&Load_Img_Info, &Area_Img_Info);

    if(Hold == true)
    {
        EPD_IT8951_Display_Area(X, Y, W, H, GC16_Mode);
    }
    else
    {
        EPD_IT8951_Display_AreaBuf(X, Y, W, H, GC16_Mode, Target_Memory_Addr);
    }
}

/**
 * @brief High-level API: Display a BMP image file on the e-Paper display.
 *
 * Loads a BMP file, draws it to the display buffer, and refreshes the e-Paper display.
 * Automatically configures rotation, mirroring, and bit depth based on the display mode.
 * Handles all buffer management and display logic internally.
 *
 * @param path Path to the BMP file.
 * @param VCOM VCOM voltage setting (pass 0 to use default).
 * @param Mode Display mode (0-3) that determines rotation, mirroring, and color settings.
 * @return 0 on success, negative value on error.
 */
#include "GUI_BMPfile.h"
#include "GUI_Paint.h"

/**
 * @brief Compute the display configuration (bit depth, rotation, mirroring, color) for a given mode.
 *
 * Mode mapping (see docs/quickstart.md and examples):
 *   0: No rotate, no mirror, grayscale
 *   1: No rotate, horizontal mirror, grayscale (10.3")
 *   2: No rotate, horizontal mirror, grayscale (5.2")
 *   3: No rotate, no mirror, color (6" color)
 *
 * @param mode Display mode (0, 1, 2, 3)
 * @return EPD_Config struct with fields set appropriately
 */
EPD_Config EPD_IT8951_ComputeConfig(UWORD mode) {
    EPD_Config cfg;
    // Default: 4bpp, no rotation, no mirror, grayscale
    cfg.bits_per_pixel = 4;
    cfg.rotate = 0;
    cfg.mirror = 0;
    cfg.is_color = 0;

    switch (mode) {
        case 3:
            cfg.bits_per_pixel = 4; // 4bpp for color, but could be 8bpp if needed
            cfg.rotate = 0;
            cfg.mirror = 0;
            cfg.is_color = 1;
            break;
        case 2:
            cfg.bits_per_pixel = 4;
            cfg.rotate = 0;
            cfg.mirror = 1; // Horizontal mirror
            cfg.is_color = 0;
            break;
        case 1:
            cfg.bits_per_pixel = 4;
            cfg.rotate = 0;
            cfg.mirror = 1; // Horizontal mirror
            cfg.is_color = 0;
            break;
        case 0:
        default:
            cfg.bits_per_pixel = 4;
            cfg.rotate = 0;
            cfg.mirror = 0;
            cfg.is_color = 0;
            break;
    }
    return cfg;
}

int EPD_IT8951_DisplayBMP(const char *path, UWORD VCOM, UWORD Mode) {
    EPD_LOG_INFO("path=%s, VCOM=%d, Mode=%d", path, VCOM, Mode);
    // 1. Initialize the display and get device info
    IT8951_Dev_Info dev_info = EPD_IT8951_Init(VCOM);
    if (dev_info.Panel_W == 0 || dev_info.Panel_H == 0) {
        EPD_LOG_ERROR("Failed to initialize display or get panel info");
        return -10; // Failed to init or get panel info
    }
    EPD_LOG_INFO("Initialized display, panel size: %dx%d", dev_info.Panel_W, dev_info.Panel_H);
    EPD_Config cfg = EPD_IT8951_ComputeConfig(Mode);
    Paint_SetRotate(cfg.rotate);
    Paint_SetMirroring(cfg.mirror);
    isColor = cfg.is_color;
    UBYTE bits_per_pixel = cfg.bits_per_pixel;
    UWORD width = dev_info.Panel_W;
    UWORD height = dev_info.Panel_H;
    if (Four_Byte_Align) {
        width = dev_info.Panel_W - (dev_info.Panel_W % 32);
    }
    UDOUBLE image_size;
    if (bits_per_pixel == 1) {
        image_size = ((width + 7) / 8) * height;
    } else if (bits_per_pixel == 2) {
        image_size = ((width + 3) / 4) * height;
    } else if (bits_per_pixel == 4) {
        image_size = ((width + 1) / 2) * height;
    } else {
        image_size = width * height;
    }
    EPD_LOG_DEBUG("Allocating frame buffer of size %u", image_size);
    UBYTE *frame_buf = (UBYTE*)malloc(image_size);
    if (!frame_buf) {
        EPD_LOG_ERROR("Out of memory allocating display buffer");
        return -11; // Out of memory
    }
    Paint_NewImage(frame_buf, width, height, ROTATE_0, WHITE);
    Paint_SelectImage(frame_buf);
    Paint_SetBitsPerPixel(bits_per_pixel);
    Paint_Clear(WHITE);
    int bmp_result = GUI_ReadBmp(path, 0, 0);
    EPD_LOG_DEBUG("Loaded BMP file, result=%d", bmp_result);
    if (bmp_result < 0) {
        EPD_LOG_ERROR("Failed to load BMP file (error %d)", bmp_result);
        free(frame_buf);
        return bmp_result; // Propagate error from BMP loader
    }
    switch (bits_per_pixel) {
        case 1:
            EPD_IT8951_1bp_Refresh(frame_buf, 0, 0, width, height, Mode, 0, false);
            break;
        case 2:
            EPD_IT8951_2bp_Refresh(frame_buf, 0, 0, width, height, false, 0, false);
            break;
        case 4:
            EPD_IT8951_4bp_Refresh(frame_buf, 0, 0, width, height, true, 0, false);
            break;
        case 8:
            EPD_IT8951_8bp_Refresh(frame_buf, 0, 0, width, height, true, 0);
            break;
        default:
            EPD_LOG_ERROR("Invalid bit depth %d", bits_per_pixel);
            free(frame_buf);
            return -12; // Invalid bit depth
    }
    free(frame_buf);
    return 0;
}
