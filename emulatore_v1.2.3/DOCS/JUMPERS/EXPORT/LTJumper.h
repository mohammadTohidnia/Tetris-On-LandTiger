#pragma once

#include "LTBase.h"
#include <functional>

class LTEXPORT LTJumper : public LTBase
{
private:
	DECLARE_DYNAMIC(LTJumper)
	INT state;
	INT jumps;
	std::function<void(LTJumper*, INT)> callback;
	LTJumper() = delete;
protected:
	DECLARE_MESSAGE_MAP();
	void OnDraw(CPaintDC& dc);
	BOOL init;
public:
	static CSize Size(ChipOrientation orientation = ChipOrientation::TOP, INT jumps = 2);
	LTJumper(const std::string& ID, INT state = 1, INT jumps = 2, ChipOrientation orientation = ChipOrientation::TOP);
	void nextState(); //go to next state (move jumper, one position)
	void setNumberOfJumps(INT jumps); //set the numer of ping that the jumper has
	void setState(INT state); //set the current state (state \IN {0, jumps-1}
	INT getState();
	void Update(); //update jumper (it call the callback)
	void setCallback(const std::function<void(LTJumper*, INT)>& clb); //call a function when the jumper changes state (not called on init)
	virtual ~LTJumper();
	void setOrientation(ChipOrientation orientation) override;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

