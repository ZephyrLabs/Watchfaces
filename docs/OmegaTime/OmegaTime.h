#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "ScreenList.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
  }
  namespace Applications {
    namespace Screens {

      class WatchFaceAnalog : public Screen {
      public:
        WatchFaceAnalog(DisplayApp* app,
                        Controllers::DateTime& dateTimeController,
                        Controllers::Battery& batteryController,
                        Controllers::Ble& bleController,
                        Controllers::NotificationManager& notificatioManager,
                        Controllers::Settings& settingsController);

        ~WatchFaceAnalog() override;

        bool Refresh() override;

      private:
        uint8_t sHour, sMinute, sSecond;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;

        Pinetime::Controllers::DateTime::Months month;
        uint8_t day;
        Pinetime::Controllers::DateTime::Days dayOfWeek;

        Pinetime::Controllers::DateTime::Months currentMonth = Pinetime::Controllers::DateTime::Months::Unknown;
        Pinetime::Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
        uint8_t currentDay = 0;

        DirtyValue<float> batteryPercentRemaining {0};
        DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime;
        DirtyValue<bool> notificationState {false};

        lv_obj_t* hour_bar_1;
        lv_obj_t* min_bar_1;
        lv_obj_t* hour_bar_2;
        lv_obj_t* min_bar_2;
        lv_obj_t* second_body;

        lv_obj_t* three_bar;
        lv_obj_t* six_bar;
        lv_obj_t* nine_bar;
        lv_obj_t* twelve_bar_1;
        lv_obj_t* twelve_bar_2;
        lv_obj_t* min_tri_1;
        lv_obj_t* min_tri_2;
        lv_obj_t* min_tri_3;
        lv_obj_t* hour_tri_1;
        lv_obj_t* hour_tri_2;
        lv_obj_t* hour_tri_3;
        lv_obj_t* one_dot;
        lv_obj_t* two_dot;
        lv_obj_t* four_dot;
        lv_obj_t* five_dot;
        lv_obj_t* seven_dot;
        lv_obj_t* eight_dot;
        lv_obj_t* ten_dot;
        lv_obj_t* eleven_dot;
        lv_obj_t* seconds_dot;

        // ##
        lv_point_t hour_point_1[2];
        lv_point_t hour_point_2[2];
        lv_point_t minute_point_1[2];
        lv_point_t minute_point_2[2];
        lv_point_t second_point[2];
        lv_point_t bar_point_horitz[2];
        lv_point_t bar_point_vert[2];
        lv_point_t minute_tri_set_1[2];
        lv_point_t minute_tri_set_2[2];
        lv_point_t minute_tri_set_3[2];
        lv_point_t hour_tri_set_1[2];
        lv_point_t hour_tri_set_2[2];
        lv_point_t hour_tri_set_3[2];

        // ##
        lv_style_t min_hour_style;
        lv_style_t second_line_style;
        lv_style_t dots_style;
        lv_style_t bars_style;
        lv_style_t tri_style;

        lv_obj_t* label_date;
        lv_obj_t* label_day;
        lv_obj_t* batteryIcon;
        lv_obj_t* notificationIcon;

        Controllers::DateTime& dateTimeController;
        Controllers::Battery& batteryController;
        Controllers::Ble& bleController;
        Controllers::NotificationManager& notificatioManager;
        Controllers::Settings& settingsController;

        void UpdateClock();
      };
    }
  }
}