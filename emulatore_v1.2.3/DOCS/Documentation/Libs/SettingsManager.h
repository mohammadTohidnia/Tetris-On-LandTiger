#pragma once

#include "pch.h"
#include "CONST.h"

#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <chrono>
#include <mutex>

#define BTN_MIN_BOUNCE_KEY "BTN_MIN_BOUNCE"
#define BTN_MAX_BOUNCE_KEY "BTN_MAX_BOUNCE"
#define BTN_US_RANGE_KEY "BTN_US_RANGE"
#define BTN_BASE_US_KEY "BTN_BASE_US"
#define BTN_SIGMA_KEY "BTN_SIGMA"
#define BTN_UP_BOUNCE_RATE_KEY "BTN_UP_BOUNCE_RATE"
#define POTMETER_SIGMA_KEY "POTMETER_SIGMA"
#define POTMETER_PERC_OF_RAND_KEY "POTMETER_PERC_OF_RAND"
#define SPEAKER_IN_DLL_VOLUME_KEY "SPEAKER_IN_DLL_VOLUME"
#define SINE_TABLE_SIZE_KEY "SIN_TABLE_SIZE"
#define SCALE_RIT_KEY "SCALE_RIT"
#define SCALE_TIM0_KEY "SCALE_TIM0"
#define SCALE_TIM1_KEY "SCALE_TIM1"
#define SCALE_TIM2_KEY "SCALE_TIM2"
#define SCALE_TIM3_KEY "SCALE_TIM3"
#define BTN_BOUNCE_KEY "BTN_BOUNCE"
#define POTMETER_BOUNCE_KEY "POTMETER_BOUNCE"
#define SCALE_CUSTOM_KEY "SCALE_CUSTOM"
#define SCALE_CUSTOM_VALUE_KEY "SCALE_CUSTOM_VALUE"

class FileDictionary;

class LTIMPORT SettingsManager
{
private:
    static std::unique_ptr<SettingsManager> instance;
    static std::once_flag once;
    SettingsManager(void);
    SettingsManager(const SettingsManager& src) = delete;
    SettingsManager& operator=(const SettingsManager& rhs) = delete;
    BOOL changed;
    std::recursive_mutex m;
    std::unique_ptr<FileDictionary> dict;
public:
    static SettingsManager& sharedInstance(void);
    ~SettingsManager(void);
    BOOL getValueWithKey(std::string key, float& value);
    BOOL setValueWithKey(std::string key, float value);
    float getValueWithKey(std::string key); // TODO: Better to return a string here maybe? AND put const & in methods!!!
    BOOL Save(void);
};

#define settingsManager SettingsManager::sharedInstance()