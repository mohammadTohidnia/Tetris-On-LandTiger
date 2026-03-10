#pragma once

// LTBase

#include <mutex>
#include <filesystem>
#include <string>

#include "CONST.h"

/* Opaque Classes */
class ChipDialog;
class Bundle;

enum class ChipOrientation
{
	TOP,
	LEFT,
	BOTTOM,
	RIGHT
};

enum class BaseType
{
	Rect,
	Rotated_Rect, // 45*
	Ellipse
};

class LTEXPORT LTBase : public CWnd
{
private:
	DECLARE_DYNAMIC(LTBase)
	BOOL enabled;

	std::string bundleID;
	ChipDialog* info;

	LTBase() = delete;
	std::string ID;
protected:
	DECLARE_MESSAGE_MAP();
	static INT OffsetForRotatedRectOfWidth(double width);

	ChipOrientation orientation;
	void OnDraw(CDC& dc, BaseType type = BaseType::Rect, BOOL draw_chip = TRUE, COLORREF back_color = CHIP_COLOR);

	BOOL loadSettings(float &value);
	BOOL saveSettings(float value);
public:
	LTBase(const std::string& ID, ChipOrientation orientation);
	//std::string GetBundleID(void) const;
	void SetBundleID(const std::string& ID);
	std::string GetID(void) const;
	virtual ~LTBase();
	virtual void setEnabled(BOOL enabled);
	virtual BOOL isEnabled() const;
	virtual void setOrientation(ChipOrientation orientation) = 0;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


