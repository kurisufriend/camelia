#include "menu.h"
#include "globals.h"
#include "drawing.h"
#include "settings.h"


namespace info
{
	static int paddingValue = 5;
	static int baseWidth = 300; 
	static int baseHeight = 200;
	static int currentTab = -1;

	static int currentVisualsTab = 1;
	static int currentAimTab = 1;
}

/* callbacks */
void dummy() {}
/* end callbacks */

bool menu::isMouseInRect(int x1, int y1, int x2, int y2) // ill just hope i never make any non-rectangle controls
{
	return (g::mousePosition.x > x1 && g::mousePosition.y > y1 && g::mousePosition.x < x2 && g::mousePosition.y < y2);
}

void menu::drawButton(std::string label, int x1, int& y1, void(*callback)(), D3DCOLOR color)
{
	y1 += 25 + info::paddingValue;
	int x2 = x1 + drawing::getTextWidth(label.c_str(), g::font);
	int y2 = y1 + 17;
	bool isHovered = isMouseInRect(x1, y1, x2, y2);
	bool isPressed = (isHovered && (GetAsyncKeyState(VK_LBUTTON) & 0x8000));
	if (isHovered)
		color = D3DCOLOR_ARGB(255, 120, 120, 120);
	if (isPressed)
	{
		color = D3DCOLOR_ARGB(255, 210, 210, 0);
		callback();
	}
	drawing::drawFilledRect2(x1, y1, x2, y2, color);
	drawing::drawText(g::font, x1, y1, D3DCOLOR_ARGB(255, 255, 255, 255), label.c_str());
}

void menu::drawMutex(std::string label, int x1, int& y1, int& tracker, std::vector<std::string> options, D3DCOLOR color)
{
	int maxSize = -1;
	std::string maxString;
	for (int i = 0; i < options.size(); i++)
	{
		std::string curOption = options.at(i);
		//int curSize = drawing::getTextWidth(curOption.c_str(), g::font);
		int curSize = curOption.length();
		if (curSize > maxSize)
		{
			maxSize = curSize;
			maxString = curOption;
		}
	}
	D3DCOLOR oldColor = color;
	y1 += 25 + info::paddingValue;
	drawing::drawText(g::font, x1, y1, D3DCOLOR_ARGB(255, 255, 255, 255), label.c_str());
	for (int i = 0; i < options.size(); i++)
	{
		color = oldColor;
		y1 += 17;
		std::string option = options.at(i);
		int x2 = x1 + drawing::getTextWidth(maxString.c_str(), g::font);
		int y2 = y1 + 17;
		bool isHovered = isMouseInRect(x1, y1, x2, y2);
		bool isPressed = (isHovered && (GetAsyncKeyState(VK_LBUTTON) & 0x8000));
		if (isHovered)
			color = D3DCOLOR_ARGB(255, 120, 120, 120);
		D3DCOLOR textColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		if (isPressed || i == tracker)
		{
			color = D3DCOLOR_ARGB(255, 210, 210, 0);
			textColor = D3DCOLOR_ARGB(255, 0, 0, 0);
			tracker = i;
		}
		drawing::drawFilledRect2(x1, y1, x2, y2, color);
		drawing::drawText(g::font, x1, y1, textColor, option.c_str());
	}
}

void menu::drawToggle(std::string label, int x1, int& y1, bool& tracker, D3DCOLOR color)
{
	y1 += 25 + info::paddingValue;
	int x2 = x1 + 17;
	int y2 = y1 + 17;
	bool isHovered = isMouseInRect(x1, y1, x2, y2);
	bool isPressed = (isHovered && (GetAsyncKeyState(VK_LBUTTON) & 0x1));
	if (isHovered)
		color = D3DCOLOR_ARGB(255, 120, 120, 120);
	D3DCOLOR pressColor = color;
	if (isPressed)
	{
		tracker = !tracker;
	}
	pressColor = tracker ? D3DCOLOR_ARGB(255, 210, 210, 0) : color;
	drawing::drawFilledRect2(x1, y1, x2, y2, color);
	drawing::drawFilledRect2(x1 + 1, y1 + 1, x2 - 1, y2 - 1, pressColor);
	drawing::drawText(g::font, x2, y1, D3DCOLOR_ARGB(255, 255, 255, 255), label.c_str());
}

void menu::drawTab(std::string label, int& x1, int y1, int& tracker, int tab)
{
	int x2 = x1 + drawing::getTextWidth(label.c_str(), g::font);
	int y2 = y1 + 17 + 2;
	bool isHovered = isMouseInRect(x1, y1, x2, y2);
	bool isPressed = (isHovered && (GetAsyncKeyState(VK_LBUTTON) & 0x8000));
	D3DCOLOR color = isHovered ? D3DCOLOR_ARGB(255, 210, 210, 0) : D3DCOLOR_ARGB(255, 120, 120, 120);
	if (tracker == tab)
		color = D3DCOLOR_ARGB(255, 210, 210, 0);
	if (isPressed)
	{
		tracker = tab;
	}
	drawing::drawText(g::font, x1, y1, D3DCOLOR_ARGB(255, 255, 255, 255), label.c_str());
	drawing::drawLine(x1, y2, x2, y2, 1, false, color);
	x1 += (info::paddingValue * 2) + drawing::getTextWidth(label.c_str(), g::font);
}

void menu::drawSlider(std::string label, int x1, int& y1, int& tracker, int min, int max, D3DCOLOR color)
{
	y1 += 25 + info::paddingValue;
	int x2 = x1 + 200;
	int y2 = y1 + 17;
	bool isHovered = isMouseInRect(x1, y1, x2, y2);
	bool isPressed = (isHovered && (GetAsyncKeyState(VK_LBUTTON) & 0x8000));
	if (isHovered)
		color = D3DCOLOR_ARGB(255, 120, 120, 120);
	int sliderX = std::lrint((float)x1 + ((float)tracker * (200.f / (float)max))); // fuck doubles
	if (isPressed)
	{
		sliderX = g::mousePosition.x;
		tracker = std::lrint(((g::mousePosition.x - x1) / 200.f) * (float)max);
	}
	D3DCOLOR textColor;
	if (sliderX > (x1 + drawing::getTextWidth(label.c_str(), g::font)))
		textColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	else
		textColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	drawing::drawFilledRect2(x1, y1, x2, y2, color);
	drawing::drawFilledRect2(x1, y1, sliderX, y2, D3DCOLOR_ARGB(255, 210, 210, 0));
	drawing::drawText(g::font, x1, y1, textColor, label.c_str());
	drawing::drawText(g::font, x2 + info::paddingValue, y1, D3DCOLOR_ARGB(255, 255, 255, 255), std::to_string(tracker).c_str());
}

void menu::drawSeparator(int& y) // im lazy
{
	y += 25 + info::paddingValue;
}

void menu::render()
{
	if (GetAsyncKeyState(VK_INSERT) & 0xFFF)
		g::menuOpen = !g::menuOpen;
	if (!g::menuOpen)
		return;
	/* menu base start, very messy */
	float windowLeft = info::baseWidth - 300;
	float windowTop = info::baseHeight - 200;
	float windowRight = info::baseWidth + 300;
	float windowBottom = info::baseHeight + 450;
	drawing::drawFilledRect2(windowLeft, windowTop, windowRight, windowBottom, D3DCOLOR_ARGB(255, 13, 11, 15));
	drawing::drawFilledRect2(windowLeft, windowTop, windowRight, windowTop + 50, D3DCOLOR_ARGB(255, 9, 8, 9));
	drawing::drawFilledRect2(windowLeft, windowBottom - 50, windowRight, windowBottom, D3DCOLOR_ARGB(255, 9, 8, 9));
	drawing::drawText(g::font, windowLeft + 10, windowTop + 10, D3DCOLOR_ARGB(255, 230, 230, 230), "camelia");
	if (isMouseInRect(windowLeft, windowTop, windowRight, windowBottom) && (GetAsyncKeyState(VK_RBUTTON)))
	{
		info::baseWidth = g::mousePosition.x;
		info::baseHeight = g::mousePosition.y;
	}
	/* menu base end */
	/* tabs */
	int xPos = windowLeft + 75;
	drawTab("aim", xPos, windowTop + 10, info::currentTab, 1);
	drawTab("anti-aim", xPos, windowTop + 10, info::currentTab, 2);
	drawTab("visuals", xPos, windowTop + 10, info::currentTab, 3);
	drawTab("movement", xPos, windowTop + 10, info::currentTab, 4);
	drawTab("misc", xPos, windowTop + 10, info::currentTab, 5);
	/* tabs end */
	if (info::currentTab == 1) // aim
	{
		int xPos = windowLeft + 5;
		drawTab("aimbot", xPos, windowBottom - 34, info::currentAimTab, 1);
		drawTab("triggerbot", xPos, windowBottom - 34, info::currentAimTab, 2);
		if (info::currentAimTab == 1)
		{
			int yPos = windowTop + 25; // big brain genius auto padding i should win an award
			drawToggle("aimbot", windowLeft + info::paddingValue, yPos, settings::bAimbot, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSlider("minimum hit chance", windowLeft + info::paddingValue, yPos, settings::iMinimumHitchance, 0, 101, D3DCOLOR_ARGB(255, 25, 25, 25));
			//drawSlider("aimbot fov", windowLeft + info::paddingValue, yPos, settings::iAimbotFOV, 0, 180, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("silent", windowLeft + info::paddingValue, yPos, settings::bSilentAimbot, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("smoothed", windowLeft + info::paddingValue, yPos, settings::bAimbotSmoothing, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSlider("smooth amount", windowLeft + info::paddingValue, yPos, settings::iSmoothingAmount, 0, 100, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("autoshot", windowLeft + info::paddingValue, yPos, settings::bAutoShoot, D3DCOLOR_ARGB(255, 25, 25, 25));
			//drawToggle("autostop", windowLeft + info::paddingValue, yPos, settings::bAutoStop, D3DCOLOR_ARGB(255, 25, 25, 25));
		}
		if (info::currentAimTab == 2)
		{
			int yPos = windowTop + 25; // big brain genius auto padding i should win an award
			drawToggle("triggerbot", windowLeft + info::paddingValue, yPos, settings::bTriggerbot, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("triggerbot on key", windowLeft + info::paddingValue, yPos, settings::bTriggerbotOnKey, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("factor recoil", windowLeft + info::paddingValue, yPos, settings::bTriggerbotRecoil, D3DCOLOR_ARGB(255, 25, 25, 25));
		}
	}
	else if (info::currentTab == 2) // aa
	{
		int yPos = windowTop + 25; // big brain genius auto padding i should win an award
		drawToggle("antiaim", windowLeft + info::paddingValue, yPos, settings::bAntiAim, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawSlider("x", windowLeft + info::paddingValue, yPos, settings::iAAX, 0, 90, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawSlider("y", windowLeft + info::paddingValue, yPos, settings::iAAY, 0, 180, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawToggle("jitter", windowLeft + info::paddingValue, yPos, settings::bJitter, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawSlider("jitter amount", windowLeft + info::paddingValue, yPos, settings::iJitterAmount, 0, 90, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawToggle("fakelag", windowLeft + info::paddingValue, yPos, settings::bFakelag, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawSlider("fakelag amount", windowLeft + info::paddingValue, yPos, settings::iFakelagAmount, 0, 16, D3DCOLOR_ARGB(255, 25, 25, 25));
	}
	else if (info::currentTab == 3) // visuals
	{
		int xPos = windowLeft + 5;
		drawTab("self", xPos, windowBottom - 34, info::currentVisualsTab, 1);
		drawTab("enemies", xPos, windowBottom - 34, info::currentVisualsTab, 2);
		drawTab("world", xPos, windowBottom - 34, info::currentVisualsTab, 3);
		drawTab("swag", xPos, windowBottom - 34, info::currentVisualsTab, 4);
		if (info::currentVisualsTab == 1) // self
		{
			int yPos = windowTop + 25; // big brain genius auto padding i should win an award
			drawToggle("arm chams", windowLeft + info::paddingValue, yPos, settings::bArmChams, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("remove arms", windowLeft + info::paddingValue, yPos, settings::bRemoveArms, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawMutex("chams material", windowLeft + info::paddingValue, yPos, settings::iArmsMaterial, chamsMaterials, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSeparator(yPos);
			drawToggle("force crosshair", windowLeft + info::paddingValue, yPos, settings::bForceCrosshair, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("recoil crosshair", windowLeft + info::paddingValue, yPos, settings::bRecoilCrosshair, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("remove scope overlay", windowLeft + info::paddingValue, yPos, settings::bRemoveScopeOverlay, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSeparator(yPos);
			drawToggle("local anglefix", windowLeft + info::paddingValue, yPos, settings::bAngleFix, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("thirdperson", windowLeft + info::paddingValue, yPos, settings::bThirdPerson, D3DCOLOR_ARGB(255, 25, 25, 25));
		}
		else if (info::currentVisualsTab == 2) // enemies
		{
			int yPos = windowTop + 25; // big brain genius auto padding i should win an award
			drawToggle("esp", windowLeft + info::paddingValue, yPos, settings::bESP, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("only when visible", windowLeft + info::paddingValue, yPos, settings::bESPOnVisible, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSeparator(yPos);
			drawToggle("chams", windowLeft + info::paddingValue, yPos, settings::bPlayerChams, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawMutex("chams material", windowLeft + info::paddingValue, yPos, settings::iPlayerMaterial, chamsMaterials, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("bounding box", windowLeft + info::paddingValue, yPos, settings::bBoxESP, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("name", windowLeft + info::paddingValue, yPos, settings::bNameESP, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("health", windowLeft + info::paddingValue, yPos, settings::bHealthESP, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("glow", windowLeft + info::paddingValue, yPos, settings::bGlowESP, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawToggle("eye lines", windowLeft + info::paddingValue, yPos, settings::bBarrelESP, D3DCOLOR_ARGB(255, 25, 25, 25));
		}
		else if (info::currentVisualsTab == 3) // world
		{
			int yPos = windowTop + 25; // big brain genius auto padding i should win an award
			drawToggle("nightmode", windowLeft + info::paddingValue, yPos, settings::bNightmode, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSlider("nightmode amount", windowLeft + info::paddingValue, yPos, settings::iNightmodeLevel, 0, 10, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawSeparator(yPos);
			drawToggle("grenade prediction", windowLeft + info::paddingValue, yPos, settings::bGrenadePrediction, D3DCOLOR_ARGB(255, 25, 25, 25));
		}
		else if (info::currentVisualsTab == 4) // swag
		{
			int yPos = windowTop + 25; // big brain genius auto padding i should win an award
			drawToggle("knifechanger", windowLeft + info::paddingValue, yPos, settings::bKnifechanger, D3DCOLOR_ARGB(255, 25, 25, 25));
			drawMutex("knife", windowLeft + info::paddingValue, yPos, settings::iKnife, knifechangerOptions, D3DCOLOR_ARGB(255, 25, 25, 25));
		}
	}	
	else if (info::currentTab == 4) // movement
	{
		int yPos = windowTop + 25; // big brain genius auto padding i should win an award
		drawToggle("autohop", windowLeft + info::paddingValue, yPos, settings::bAutohop, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawToggle("autostrafer", windowLeft + info::paddingValue, yPos, settings::bAutostrafer, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawToggle("slow walk", windowLeft + info::paddingValue, yPos, settings::bSlowWalk, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawSlider("slow walk amount", windowLeft + info::paddingValue, yPos, settings::iSlowWalkAmount, 0, 450, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawToggle("unlock stamina", windowLeft + info::paddingValue, yPos, settings::bUnlockStamina, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawToggle("visualise speed", windowLeft + info::paddingValue, yPos, settings::bSpeedGraph, D3DCOLOR_ARGB(255, 25, 25, 25));
	}
	else if (info::currentTab == 5) // misc
	{
		int yPos = windowTop + 25; // big brain genius auto padding i should win an award
		drawToggle("clantag changer", windowLeft + info::paddingValue, yPos, settings::bClantagChanger, D3DCOLOR_ARGB(255, 25, 25, 25));
		drawMutex("clantag", windowLeft + info::paddingValue, yPos, settings::iClantag, clantags, D3DCOLOR_ARGB(255, 25, 25, 25));
	}
	else
	{
		std::string poop = "welcome :)"; // i don't like this add a center text thing later or sometbnrhyi5ehi
		drawing::drawText(g::font, info::baseWidth - (drawing::getTextWidth(poop.c_str(), g::font) / 2), info::baseHeight, D3DCOLOR_ARGB(255, 255, 255, 255), (LPCSTR)poop.c_str());
	}
}