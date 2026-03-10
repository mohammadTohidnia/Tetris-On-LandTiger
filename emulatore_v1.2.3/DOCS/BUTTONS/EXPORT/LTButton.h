#pragma once

#include <cmath>
#include <functional>
#include <mutex>
#include <random>

#include "LTBase.h"
#include "CONST.h"
#include "AGSI.h"

enum class ButtonState
{
	PRESSED,
	RELEASED
};

class LTButton : public LTBase
{
private:
	static std::default_random_engine generator;
	static std::normal_distribution<float> distribution;
	DECLARE_DYNAMIC(LTButton);
	std::function<void(LTButton*, ButtonState)> callback;
	volatile BOOL collapsing;
	INT bounce_state;
	std::mutex mutex;
	std::thread bounce_thread_up;
	std::thread bounce_thread_down;
	CRect btn_rect;
	LTButton() = delete;
	void UpdateState(ButtonState newState);
protected:
	DECLARE_MESSAGE_MAP();
	void OnDraw(CPaintDC& dc);
	ButtonState btn_state;
	void bounce_callback();
	BOOL init;
public:
	static CSize Size(ChipOrientation orientation = ChipOrientation::TOP);
	LTButton(const std::string& ID, ChipOrientation orientation = ChipOrientation::TOP);
	void setCallback(const std::function<void(LTButton*, ButtonState)>& clb);
	void setState(ButtonState state);
	virtual ~LTButton();
	void setOrientation(ChipOrientation orientation) override;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
