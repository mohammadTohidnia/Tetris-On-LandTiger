#pragma once

#include "SettingsManager.h"
#include "LTBase.h"
#include "AGSI.h"
#include "KeyListener.h"

enum class Peripheral {
	PCTIM0 = 1 << 1,
	PCTIM1 = 1 << 2,
	PCUART0 = 1 << 3,
	PCUART1 = 1 << 4,
	//bit 5 is reserved
	PCPWM1 = 1 << 6,
	PCI2C0 = 1 << 7,
	PCSPI = 1 << 8,
	PCRTC = 1 << 9,
	PCSSP1 = 1 << 10,
	//bit 11 is reserved
	PCADC = 1 << 12,
	PCCAN1 = 1 << 13,
	PCCAN2 = 1 << 14,
	PCGPIO = 1 << 15,
	PCRIT = 1 << 16,
	PCMCPWM = 1 << 17,
	PCQEI = 1 << 18,
	PCI2C1 = 1 << 19,
	//bit 20 is reserved
	PCSSP0 = 1 << 21,
	PCTIM2 = 1 << 22,
	PCTIM3 = 1 << 23,
	PCUART2 = 1 << 24,
	PCUART3 = 1 << 25,
	PCI2C2 = 1 << 26,
	PCI2S = 1 << 27,
	//bit 28 is reserved
	PCGPDMA = 1 << 29,
	PCENECT = 1 << 30,
	PCUSB = 1L << 31
};

/* API IMPORTS */
extern "C" LTIMPORT const struct AGSIFUNCS* getAgsi(void);
extern "C" LTIMPORT BOOL DefineKey(KeyRAI* key);
extern "C" LTIMPORT BOOL DefineNewComponent(LTBase *component, CWnd* parent, CRect rect, Bundle* bundle);
extern "C" LTIMPORT BOOL DefinePageDialog(CDialog* page, CWnd* parent);
extern "C" LTIMPORT LTBase * getBoardChipWithID(const std::string & ID);
extern "C" LTIMPORT BOOL isDialogForeMost(void);
extern "C" LTIMPORT BOOL isBoardPowered(void);
extern "C" LTIMPORT void setBoardPowered(BOOL power);
extern "C" LTEXPORT DOUBLE getScaleFlag(void);
extern "C" LTEXPORT CDialog * getDialogAtIndexForComponent(LTBase * component, UINT index);
extern "C" LTEXPORT const LTBase *getDialogComponentCaller(CDialog *parent);

/* API EXPORTS */
/* Required */
extern "C" LTEXPORT BOOL DefineAllSFR(void);
extern "C" LTEXPORT BOOL DefineAllVTREG(void);
extern "C" LTEXPORT BOOL DefineAllInterrupts(void);
extern "C" LTEXPORT BOOL DefineAllWatches(void);
extern "C" LTEXPORT BOOL ResetPeripheral(void);
extern "C" LTEXPORT BOOL DefineAllKeys(void);
extern "C" LTEXPORT BOOL DefineAllComponents(CWnd * dialog, CRect board_rect, Bundle* bundle);
extern "C" LTEXPORT BOOL DefineAllDialogPages(CWnd * parent);
extern "C" LTEXPORT void UpdateUI(void);
/* Optional */
extern "C" LTEXPORT BOOL DefineAllMenuEntries(void);
extern "C" LTEXPORT void ClosingUI(void);
extern "C" LTEXPORT void ComponentsLoaded(void);
extern "C" LTEXPORT BOOL Terminate(void);