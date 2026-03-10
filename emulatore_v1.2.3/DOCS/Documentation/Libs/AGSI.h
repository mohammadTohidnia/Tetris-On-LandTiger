#ifndef __AGSI_H__ 
#define __AGSI_H__ 


#include <windows.h> 


#define AGSIEXPORT __declspec( dllexport ) 
#define AGSIAPI    WINAPIV 


#ifdef __cplusplus 
extern "C" {
#endif 

	/////////////////////////////////////////////////////////////////////////////// 
	// 
	// AGSI: Advanced Generic Simulator Interface (Simulation Interface Part) 
	// 

	typedef DWORD AGSIVTR;
	typedef DWORD AGSIADDR;
	typedef DWORD AGSITIMER;
	typedef DWORD AGSICB_REASON;
	typedef void (*AGSICALLBACK)(DWORD addr, AGSICB_REASON r);
	typedef void (*AGSICALLBACK_VOID)(void);

	typedef struct {
		HINSTANCE     m_hInstance;
		const char* m_pszProjectPath;
		const char* m_pszDevice;
		const char* m_pszConfiguration;
		const char* m_pszAppFile;
	} AGSICONFIG;

	typedef enum {
		AGSIBYTE = 1,
		AGSIWORD = 2,
		AGSILONG = 4,
		AGSIBIT = 5,
		AGSIVTRCHAR = 10,
		AGSIVTRWORD = 11,
		AGSIVTRLONG = 12,
		AGSIVTRFLOAT = 13
	} AGSITYPE;

	typedef enum {
		AGSIREAD,
		AGSIWRITE,
		AGSIREADWRITE,
	} AGSIACCESS;

	// function code numbers for AgsiEntry function 
	typedef enum {
		AGSI_CHECK = 0,
		AGSI_INIT = 1,
		AGSI_TERMINATE = 2,
		AGSI_RESET = 3,
		AGSI_PREPLL = 4,
		AGSI_POSTPLL = 5,
		AGSI_PRERESET = 6,
		AGSI_CMDOUT = 7,
		AGSI_ONINTERRUPT = 8,
		AGSI_ONRETI = 9,
		AGSI_ENTERSLEEP = 10,
		AGSI_EXITSLEEP = 11
	} AGSIFUNC;


	/*
	 * 80x51 memory spaces
	 */

#define   amXDATA  0x0001           // XDATA 
#define   amBANK0  0x0080           // BANK0 
#define   amBANK31 0x009F           // BANK31 
#define   amDATA   0x00F0           // DATA 
#define   amBIT    0x00F1           // BIT 
#define   amEDATA  0x00F2           // EDATA (i251) 
#define   amIDATA  0x00F3           // IDATA 
#define   amECODE  0x00F4           // 251 ecode 
#define   amHDATA  0x00F5           // 251 hdata 
#define   amHCONS  0x00F6           // 251 hconst 
#define   amCONST  0x00F7           // 251 const 
#define   amPDATA  0x00FE           // PDATA (c51 macht das bei generic) 
#define   amCODE   0x00FF           // CODE  


	typedef struct {
#ifdef __C166__ 
		AGSIADDR        vec;
		char* mess;     // interrupt name 
		AGSIADDR        sfr;     // interrupt control sfr  
#else 
		AGSIADDR        vec;
		char* mess;     // interrupt name 
		AGSIADDR       msfr;     // interrupt mode sfr  
		WORD          mmask;     // interrupt mode bit mask 
		const char* mname;     // name of interrupt mode bit 
		AGSIADDR       rsfr;     // interrupt request sfr  
		WORD          rmask;     // interrupt request bit mask 
		const char* rname;     // name of interrupt request bit 
		AGSIADDR       esfr;     // interrupt enable sfr 
		WORD          emask;     // interrupt enable bit mask 
		const char* ename;     // name of interrupt enable bit 
		AGSIADDR      p0sfr;     // interrupt priority 0 sfr 
		WORD         p0mask;     // interrupt priority 0 bit mask 
		const char* pname;     // name of interrupt priority bit 
		AGSIADDR      p1sfr;     // interrupt priority 1 sfr 
		WORD         p1mask;     // interrupt priority 1 bit mask 
		WORD            pwl;     // priority within level 
		WORD     auto_reset;     // reset interrupt request flag on interrupt entry 
#endif 
	} AGSIINTERRUPT;


	/*
	 * Symbol search masks (may be combined using |) :
	 */

	typedef enum {
		AGSI_SYM_VAR = 0x0001,         // search for non-bit Variables 
		AGSI_SYM_CON = 0x0002,         // search for named Constants 
		AGSI_SYM_BIT = 0x0004,         // search for Bit in Memory 
		AGSI_SYM_LOC = 0x0008,         // search for Function/Label 
		AGSI_SYM_SFR = 0x0200          // search for SFR name 
	} AGSISYMMASK;

	/*
	 * Type of found symbol:
	 */

	typedef enum {
		AGSI_TP_VOID = 0,
		AGSI_TP_BIT = 1,
		AGSI_TP_CHAR = 2,
		AGSI_TP_UCHAR = 3,
		AGSI_TP_INT = 4,
		AGSI_TP_UINT = 5,
		AGSI_TP_SHORT = 6,
		AGSI_TP_USHORT = 7,
		AGSI_TP_LONG = 8,
		AGSI_TP_ULONG = 9,
		AGSI_TP_FLOAT = 10,
		AGSI_TP_DOUBLE = 11,
		AGSI_TP_PTR = 12,
		AGSI_TP_UNION = 13,
		AGSI_TP_STRUCT = 14,
		AGSI_TP_FUNC = 15,
		AGSI_TP_STRING = 16,
		AGSI_TP_ENUM = 17,
		AGSI_TP_FIELD = 18
	} AGSISYMTYPE;


	typedef struct {                  // Search for Sym by Name or Value. 
		AGSISYMMASK nMask;               // search mask (AG_SYM_LOC | ...) 
		char szName[256];               // search/found name (zero-terminated 
		UINT64        val;               // search/found Adr/Value 
		AGSISYMTYPE  type;               // type of found symbol (AGSI_TP_???) 
		DWORD          Ok;               // 1:=Ok, else find failed. 
	} AGSISYMDSC;



	/*
	 * Dynamic Menu construction and maintanance
	 */

	 //#pragma pack(1) 

#define AGSIDLGD struct AgsiDlgDat 
	struct AgsiDlgDat {                      // every dialog has it's own structure 
		DWORD             iOpen;               // auto reopen dialog (pos := 'rc') 
		HWND                 hw;               // Hwnd of Dialog 
		BOOL(CALLBACK* wp) (HWND hw, UINT msg, WPARAM wp, LPARAM lp);
		RECT                 rc;               // Position rectangle 
		void   (*Update) (void);               // Update dialog content 
		void (*Kill) (AGSIDLGD* pM);           // Kill dialog 
		void* vp;               // reserved for C++ Dialogs (Dlg *this) 
	};

#define AGSIMENU struct AgsiDynaM 
	struct AgsiDynaM {                      // Menu item data structure 
		int              nDelim;               // Menu template delimiter 
		char* szText;               // Menu item text 
		void    (*fp) (AGSIMENU* pM);          // create/bringDlgtoTop function 
		DWORD               nID;               // uv3 assigned ID_xxxx 
		DWORD            nDlgId;               // Dialog ID 
		AGSIDLGD* pDlg;               // link to dialog attributes 
	};
	//#pragma pack() 

	/*
	 * nDelim:  1 := normal Menu entry
	 *          2 := Popup-Entry (nested submenu)
	 *         -2 := end of Popup-Group-List
	 *         -1 := total end of Menu-List
	 *  text:   the name for the menu/popup-menu entry
	 *    fp:   Function to be activated on menu-selection
	 */


	 // Function that must be exported by the loaded DLL 
	AGSIEXPORT DWORD        AGSIAPI AgsiEntry(DWORD nCode, void* vp);

	// Callback functions that are ONLY allowed to call during the initialisation process 
	AGSIEXPORT BOOL         AGSIAPI AgsiDefineSFR(const char* pszSfrName, AGSIADDR dwAddress, AGSITYPE eType, BYTE bBitPos);
	AGSIEXPORT AGSIVTR      AGSIAPI AgsiDefineVTR(const char* pszVtrName, AGSITYPE eType, DWORD dwValue);
	AGSIEXPORT BOOL         AGSIAPI AgsiDeclareInterrupt(AGSIINTERRUPT* pInterrupt);
	AGSIEXPORT BOOL         AGSIAPI AgsiSetWatchOnSFR(AGSIADDR SFRAddress, AGSICALLBACK pfnReadWrite, AGSIACCESS eAccess);
	AGSIEXPORT BOOL         AGSIAPI AgsiSetWatchOnVTR(AGSIVTR hVTR, AGSICALLBACK pfnReadWrite, AGSIACCESS eAccess);
	AGSIEXPORT BOOL         AGSIAPI AgsiSetWatchOnMemory(AGSIADDR StartAddress, AGSIADDR EndAddress, AGSICALLBACK pfnReadWrite, AGSIACCESS eAccess);
	AGSIEXPORT AGSITIMER    AGSIAPI AgsiCreateTimer(AGSICALLBACK_VOID pfnTimer);
	AGSIEXPORT BOOL         AGSIAPI AgsiDefineMenuItem(AGSIMENU* pDym);
	AGSIEXPORT BOOL         AGSIAPI AgsiRegisterExecCallBack(void (*fp) (DWORD pc, DWORD Ypc));

	// Callback functions that are NOT allowed to call during the initialisation process 
	AGSIEXPORT BOOL         AGSIAPI AgsiWriteSFR(AGSIADDR SFRAddress, DWORD dwValue, DWORD dwMask);
	AGSIEXPORT BOOL         AGSIAPI AgsiReadSFR(AGSIADDR SFRAddress, DWORD* pdwCurrentValue, DWORD* pdwPreviousValue, DWORD dwMask);
	AGSIEXPORT BOOL         AGSIAPI AgsiWriteVTR(AGSIVTR hVTR, DWORD dwValue);
	AGSIEXPORT BOOL         AGSIAPI AgsiReadVTR(AGSIVTR hVTR, DWORD* pdwCurrentValue);
	AGSIEXPORT BOOL         AGSIAPI AgsiSetSFRReadValue(DWORD dwValue);
	AGSIEXPORT BOOL         AGSIAPI AgsiReadMemory(AGSIADDR Address, DWORD dwCount, BYTE* pbValue);
	AGSIEXPORT BOOL         AGSIAPI AgsiWriteMemory(AGSIADDR Address, DWORD dwCount, BYTE* pbValue);
	AGSIEXPORT AGSIADDR     AGSIAPI AgsiGetLastMemoryAddress(void);
	AGSIEXPORT BOOL         AGSIAPI AgsiIsSimulatorAccess(void);
	AGSIEXPORT BOOL         AGSIAPI AgsiSetTimer(AGSITIMER hTimer, DWORD dwClock);
	AGSIEXPORT UINT64       AGSIAPI AgsiGetStates(void);
	AGSIEXPORT AGSIADDR     AGSIAPI AgsiGetProgramCounter(void);
	AGSIEXPORT DWORD        AGSIAPI AgsiIsInInterrupt(DWORD);
	AGSIEXPORT BOOL			AGSIAPI AgsiRequestInterrupt(DWORD);
	AGSIEXPORT BOOL         AGSIAPI AgsiIsSleeping(void);
	AGSIEXPORT void         AGSIAPI AgsiStopSimulator(void);
	AGSIEXPORT void         AGSIAPI AgsiTriggerReset(void);
	AGSIEXPORT void         AGSIAPI AgsiUpdateWindows(void);
	AGSIEXPORT void         AGSIAPI AgsiHandleFocus(HWND hwndDialog);
	AGSIEXPORT DWORD        AGSIAPI AgsiGetExternalClockRate(void);
	AGSIEXPORT DWORD        AGSIAPI AgsiGetInternalClockRate(void);
	AGSIEXPORT double       AGSIAPI AgsiGetClockFactor(void);
	AGSIEXPORT void         AGSIAPI AgsiMessage(const char* pszFormat, ...);
	AGSIEXPORT const char* AGSIAPI AgsiGetTargetKey(const char* pszKey);
	AGSIEXPORT BOOL         AGSIAPI AgsiSetTargetKey(const char* pszKey, const char* pszString);
	AGSIEXPORT DWORD        AGSIAPI AgsiGetSymbolByName(AGSISYMDSC* vp);
	AGSIEXPORT DWORD        AGSIAPI AgsiGetSymbolByValue(AGSISYMDSC* vp);
	AGSIEXPORT void         AGSIAPI AgsiExecuteCommand(const char* pszCommand);

	AGSIEXPORT DOUBLE       AGSIAPI AgsiGetTime(void);
	AGSIEXPORT DWORD        AGSIAPI AgsiGetExtInfo(void);

	AGSIEXPORT void			AGSIAPI AgsiExecuteCommand(const char* pszCommand);
#ifdef __cplusplus 
}
#endif 

// Callback functions that are ONLY allowed to call during the initialisation process
typedef AGSIEXPORT BOOL(*tAgsiDefineSFR) (const char* pszSfrName, AGSIADDR dwAddress, AGSITYPE eType, BYTE bBitPos);
typedef AGSIEXPORT AGSIVTR(*tAgsiDefineVTR) (const char* pszVtrName, AGSITYPE eType, DWORD dwValue);
typedef AGSIEXPORT BOOL(*tAgsiDeclareInterrupt)(AGSIINTERRUPT* pInterrupt);
typedef AGSIEXPORT BOOL(*tAgsiSetWatchOnSFR) (AGSIADDR SFRAddress, AGSICALLBACK pfnReadWrite, AGSIACCESS eAccess);
typedef AGSIEXPORT BOOL(*tAgsiSetWatchOnVTR) (AGSIVTR hVTR, AGSICALLBACK pfnReadWrite, AGSIACCESS eAccess);
typedef AGSIEXPORT BOOL(*tAgsiSetWatchOnMemory) (AGSIADDR StartAddress, AGSIADDR EndAddress, AGSICALLBACK pfnReadWrite, AGSIACCESS eAccess);
typedef AGSIEXPORT AGSITIMER(*tAgsiCreateTimer) (AGSICALLBACK_VOID pfnTimer);
typedef AGSIEXPORT BOOL(*tAgsiDefineMenuItem)(AGSIMENU* pDym);
typedef AGSIEXPORT BOOL(*tAgsiRegisterExecCallBack)(void (*fp) (DWORD pc, DWORD Ypc));

// Callback functions that are NOT allowed to call during the initialisation process
typedef AGSIEXPORT BOOL(*tAgsiWriteSFR) (AGSIADDR SFRAddress, DWORD dwValue, DWORD dwMask);
typedef AGSIEXPORT BOOL(*tAgsiReadSFR)  (AGSIADDR SFRAddress, DWORD* pdwCurrentValue, DWORD* pdwPreviousValue, DWORD dwMask);
typedef AGSIEXPORT BOOL(*tAgsiWriteVTR) (AGSIVTR hVTR, DWORD dwValue);
typedef AGSIEXPORT BOOL(*tAgsiReadVTR) (AGSIVTR hVTR, DWORD* pdwCurrentValue);
typedef AGSIEXPORT BOOL(*tAgsiSetSFRReadValue)(DWORD dwValue);
typedef AGSIEXPORT BOOL(*tAgsiReadMemory) (AGSIADDR Address, DWORD dwCount, BYTE* pbValue);
typedef AGSIEXPORT BOOL(*tAgsiWriteMemory)(AGSIADDR Address, DWORD dwCount, BYTE* pbValue);
typedef AGSIEXPORT AGSIADDR(*tAgsiGetLastMemoryAddress)(void);
typedef AGSIEXPORT BOOL(*tAgsiIsSimulatorAccess)(void);
typedef AGSIEXPORT BOOL(*tAgsiSetTimer)(AGSITIMER hTimer, DWORD dwClock);
typedef AGSIEXPORT UINT64(*tAgsiGetStates)(void);
typedef AGSIEXPORT AGSIADDR(*tAgsiGetProgramCounter)(void);
typedef AGSIEXPORT DWORD(*tAgsiIsInInterrupt)(DWORD);
typedef AGSIEXPORT BOOL(*tAgsiRequestInterrupt)(DWORD);
typedef AGSIEXPORT BOOL(*tAgsiIsSleeping)(void);
typedef AGSIEXPORT void      (*tAgsiStopSimulator)(void);
typedef AGSIEXPORT void      (*tAgsiTriggerReset)(void);
typedef AGSIEXPORT void      (*tAgsiUpdateWindows)(void);
typedef AGSIEXPORT void      (*tAgsiHandleFocus) (HWND hwndDialog);
typedef AGSIEXPORT DWORD(*tAgsiGetExternalClockRate)(void);
typedef AGSIEXPORT DWORD(*tAgsiGetInternalClockRate)(void);
typedef AGSIEXPORT double    (*tAgsiGetClockFactor)(void);
typedef AGSIEXPORT void      (*tAgsiMessage)(const char* pszFormat, ...);
typedef AGSIEXPORT const char* (*tAgsiGetTargetKey)(const char* pszKey);
typedef AGSIEXPORT BOOL(*tAgsiSetTargetKey)(const char* pszKey, const char* pszString);
typedef AGSIEXPORT DWORD(*tAgsiGetSymbolByName) (AGSISYMDSC* vp);
typedef AGSIEXPORT DWORD(*tAgsiGetSymbolByValue)(AGSISYMDSC* vp);

typedef AGSIEXPORT DOUBLE(*tAgsiGetTime)(void);
typedef AGSIEXPORT DWORD(*tAgsiGetExtInfo)(void);
typedef AGSIEXPORT void(*tAgsiExecuteCommand)(const char* pszCommand);

struct AGSIFUNCS {
	tAgsiDefineSFR             DefineSFR;
	tAgsiDefineVTR             DefineVTR;
	tAgsiSetWatchOnSFR         SetWatchOnSFR;
	tAgsiSetWatchOnVTR         SetWatchOnVTR;
	tAgsiSetWatchOnMemory      SetWatchOnMemory;
	tAgsiCreateTimer           CreateTimer;
	tAgsiDefineMenuItem        DefineMenuItem;
	tAgsiRegisterExecCallBack  RegisterExecCallBack;
	tAgsiWriteSFR              WriteSFR;
	tAgsiReadSFR               ReadSFR;
	tAgsiWriteVTR              WriteVTR;
	tAgsiReadVTR               ReadVTR;
	tAgsiSetSFRReadValue       SetSFRReadValue;
	tAgsiReadMemory            ReadMemory;
	tAgsiWriteMemory           WriteMemory;
	tAgsiGetLastMemoryAddress  GetLastMemoryAddress;
	tAgsiIsSimulatorAccess     IsSimulatorAccess;
	tAgsiSetTimer              SetTimer;
	tAgsiGetStates             GetStates;
	tAgsiGetProgramCounter     GetProgramCounter;
	tAgsiDeclareInterrupt      DeclareInterrupt;
	tAgsiIsInInterrupt         IsInInterrupt;
	tAgsiRequestInterrupt	   RequestInterrupt;
	tAgsiIsSleeping            IsSleeping;
	tAgsiStopSimulator         StopSimulator;
	tAgsiTriggerReset          TriggerReset;
	tAgsiUpdateWindows         UpdateWindows;
	tAgsiHandleFocus           HandleFocus;
	tAgsiGetExternalClockRate  GetExternalClockRate;
	tAgsiGetInternalClockRate  GetInternalClockRate;
	tAgsiGetClockFactor        GetClockFactor;
	tAgsiMessage               Message;
	tAgsiGetTargetKey          GetTargetKey;
	tAgsiSetTargetKey          SetTargetKey;
	tAgsiGetSymbolByName       GetSymbolByName;
	tAgsiGetSymbolByValue      GetSymbolByValue;

	tAgsiGetTime               GetTime;
	tAgsiGetExtInfo            GetExtInfo;
	tAgsiExecuteCommand		   ExecuteCommand;
};

union fv {      // float value union
	float f;
	DWORD DW;
};

struct vtrlist {
	char* pName;			// name of SFR
	AGSITYPE Type;			// type of VTREG
	DWORD    Value;			// value of VTREG
	AGSIVTR  hVTR;			// handle of VTREG
};

typedef enum IRQn {
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  Reset_IRQn                    = -15,      /*!< 1 Reset Vector, invoked on PowerUp and warm reset*/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  HardFault_IRQn                = -13,      /*!< 3  Hard Fault, all classes of Fault              */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
  WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
  SPI_IRQn                      = 13,       /*!< SPI Interrupt                                    */
  SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
  RIT_IRQn                      = 29,       /*!< Repetitive Interrupt Timer Interrupt             */
  MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
  USBActivity_IRQn              = 33,       /*!< USB Activity Interrupt 						  */
  CANActivity_IRQn              = 34,       /*!< CAN Activity Interrupt 						  */
} IRQn_Type;

#endif // __AGSI_H__ 