#pragma once

#include "LTBase.h"

class LTEXPORT LTLed : public LTBase
{
private:
	DECLARE_DYNAMIC(LTLed);
	CRect diode;
	LTLed() = delete;
protected:
	DECLARE_MESSAGE_MAP();
	BOOL on;
	void OnDraw(CPaintDC& dc);
	BOOL init;
public:
	static CSize Size(ChipOrientation orientation = ChipOrientation::TOP);
	LTLed(const std::string& ID, BOOL on = FALSE, ChipOrientation orientation = ChipOrientation::TOP);
	virtual ~LTLed();
	void setON(BOOL flg);
	void setOrientation(ChipOrientation orientation) override;
	void setEnabled(BOOL enabled) override;
	BOOL isON(void) const;
	void toggle(void);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

