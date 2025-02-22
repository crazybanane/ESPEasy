#include "../Helpers/StringProvider.h"

#if FEATURE_ETHERNET
# include <ETH.h>
#endif // if FEATURE_ETHERNET

#include "../../ESPEasy-Globals.h"

#include "../CustomBuild/CompiletimeDefines.h"

#include "../ESPEasyCore/ESPEasyNetwork.h"
#include "../ESPEasyCore/ESPEasyWifi.h"
#if FEATURE_ETHERNET
#include "../ESPEasyCore/ESPEasyEth.h"
#endif

#include "../Globals/Device.h"
#include "../Globals/ESPEasy_Console.h"
#include "../Globals/ESPEasy_Scheduler.h"
#include "../Globals/ESPEasy_time.h"
#include "../Globals/ESPEasyWiFiEvent.h"

#if FEATURE_ETHERNET
#include "../Globals/ESPEasyEthEvent.h"
#endif

#include "../Globals/NetworkState.h"
#include "../Globals/SecuritySettings.h"
#include "../Globals/Settings.h"
#include "../Globals/WiFi_AP_Candidates.h"

#include "../Helpers/Convert.h"
#include "../Helpers/ESPEasy_Storage.h"
#include "../Helpers/Hardware_device_info.h"
#include "../Helpers/Hardware_temperature_sensor.h"
#include "../Helpers/Memory.h"
#include "../Helpers/Misc.h"
#include "../Helpers/Networking.h"
#include "../Helpers/Scheduler.h"
#include "../Helpers/StringConverter.h"
#include "../Helpers/StringGenerator_System.h"
#include "../Helpers/StringGenerator_WiFi.h"

#include "../WebServer/JSON.h"
#include "../WebServer/AccessControl.h"

#ifdef ESP32
#include <soc/rtc.h>
#endif

String getInternalLabel(LabelType::Enum label, char replaceSpace) {
  return to_internal_string(getLabel(label), replaceSpace);
}

const __FlashStringHelper * getLabel(LabelType::Enum label) {
  switch (label)
  {
    case LabelType::UNIT_NR:                return F("Unit Number");
    #if FEATURE_ZEROFILLED_UNITNUMBER
    case LabelType::UNIT_NR_0:              return F("Unit Number 0-filled");
    #endif // FEATURE_ZEROFILLED_UNITNUMBER
    case LabelType::UNIT_NAME:              return F("Unit Name");
    case LabelType::HOST_NAME:              return F("Hostname");

    case LabelType::LOCAL_TIME:             return F("Local Time");
    case LabelType::TIME_SOURCE:            return F("Time Source");
    case LabelType::TIME_WANDER:            return F("Time Wander");
    #if FEATURE_EXT_RTC
    case LabelType::EXT_RTC_UTC_TIME:       return F("UTC time stored in RTC chip");
    #endif
    case LabelType::UPTIME:                 return F("Uptime");
    case LabelType::LOAD_PCT:               return F("Load");
    case LabelType::LOOP_COUNT:             return F("Load LC");
    case LabelType::CPU_ECO_MODE:           return F("CPU Eco Mode");
#if FEATURE_SET_WIFI_TX_PWR
    case LabelType::WIFI_TX_MAX_PWR:        return F("Max WiFi TX Power");
    case LabelType::WIFI_CUR_TX_PWR:        return F("Current WiFi TX Power");
    case LabelType::WIFI_SENS_MARGIN:       return F("WiFi Sensitivity Margin");
    case LabelType::WIFI_SEND_AT_MAX_TX_PWR:return F("Send With Max TX Power");
#endif
    case LabelType::WIFI_NR_EXTRA_SCANS:    return F("Extra WiFi scan loops");
    case LabelType::WIFI_USE_LAST_CONN_FROM_RTC: return F("Use Last Connected AP from RTC");

    case LabelType::FREE_MEM:               return F("Free RAM");
    case LabelType::FREE_STACK:             return F("Free Stack");
#ifdef USE_SECOND_HEAP
    case LabelType::FREE_HEAP_IRAM:         return F("Free 2nd Heap");
#endif

#if defined(CORE_POST_2_5_0) || defined(ESP32)
  #ifndef LIMIT_BUILD_SIZE
    case LabelType::HEAP_MAX_FREE_BLOCK:    return F("Heap Max Free Block");
  #endif
#endif // if defined(CORE_POST_2_5_0) || defined(ESP32)
#if defined(CORE_POST_2_5_0)
  #ifndef LIMIT_BUILD_SIZE
    case LabelType::HEAP_FRAGMENTATION:     return F("Heap Fragmentation");
  #endif
#endif // if defined(CORE_POST_2_5_0)

#ifdef ESP32
    case LabelType::HEAP_SIZE:              return F("Heap Size");
    case LabelType::HEAP_MIN_FREE:          return F("Heap Min Free");
    #ifdef BOARD_HAS_PSRAM
    case LabelType::PSRAM_SIZE:             return F("PSRAM Size");
    case LabelType::PSRAM_FREE:             return F("PSRAM Free");
    case LabelType::PSRAM_MIN_FREE:         return F("PSRAM Min Free");
    case LabelType::PSRAM_MAX_FREE_BLOCK:   return F("PSRAM Max Free Block");
    #endif // BOARD_HAS_PSRAM
#endif // ifdef ESP32

    case LabelType::JSON_BOOL_QUOTES:           return F("JSON bool output without quotes");
#if FEATURE_TIMING_STATS
    case LabelType::ENABLE_TIMING_STATISTICS:   return F("Collect Timing Statistics");
#endif
    case LabelType::ENABLE_RULES_CACHING:       return F("Enable Rules Cache");
    case LabelType::ENABLE_SERIAL_PORT_CONSOLE: return F("Enable Serial Port Console");
    case LabelType::CONSOLE_SERIAL_PORT:        return F("Console Serial Port");
#if USES_ESPEASY_CONSOLE_FALLBACK_PORT
    case LabelType::CONSOLE_FALLBACK_TO_SERIAL0: return F("Fallback to Serial 0");
    case LabelType::CONSOLE_FALLBACK_PORT:       return F("Console Fallback Port");
#endif

//    case LabelType::ENABLE_RULES_EVENT_REORDER: return F("Optimize Rules Cache Event Order"); // TD-er: Disabled for now
    case LabelType::TASKVALUESET_ALL_PLUGINS:   return F("Allow TaskValueSet on all plugins");
    case LabelType::ALLOW_OTA_UNLIMITED:        return F("Allow OTA without size-check");
#if FEATURE_CLEAR_I2C_STUCK
    case LabelType::ENABLE_CLEAR_HUNG_I2C_BUS:  return F("Try clear I2C bus when stuck");
#endif
    #if FEATURE_I2C_DEVICE_CHECK
    case LabelType::ENABLE_I2C_DEVICE_CHECK:    return F("Check I2C devices when enabled");
    #endif // if FEATURE_I2C_DEVICE_CHECK
#ifndef BUILD_NO_RAM_TRACKER
    case LabelType::ENABLE_RAM_TRACKING:    return F("Enable RAM Tracker");
#endif
#if FEATURE_AUTO_DARK_MODE
    case LabelType::ENABLE_AUTO_DARK_MODE:  return F("Web light/dark mode");
#endif // FEATURE_AUTO_DARK_MODE
#if FEATURE_RULES_EASY_COLOR_CODE
    case LabelType::DISABLE_RULES_AUTOCOMPLETE:  return F("Disable Rules auto-completion");
#endif // if FEATURE_RULES_EASY_COLOR_CODE

    case LabelType::BOOT_TYPE:              return F("Last Boot Cause");
    case LabelType::BOOT_COUNT:             return F("Boot Count");
    case LabelType::DEEP_SLEEP_ALTERNATIVE_CALL: return F("Deep Sleep Alternative");
    case LabelType::RESET_REASON:           return F("Reset Reason");
    case LabelType::LAST_TASK_BEFORE_REBOOT: return F("Last Action before Reboot");
    case LabelType::SW_WD_COUNT:            return F("SW WD count");


    case LabelType::WIFI_CONNECTION:        return F("WiFi Connection");
    case LabelType::WIFI_RSSI:              return F("RSSI");
    case LabelType::IP_CONFIG:              return F("IP Config");
#if FEATURE_USE_IPV6
    case LabelType::IP6_LOCAL:              return F("IPv6 link local");
    case LabelType::IP6_GLOBAL:             return F("IPv6 global");
//    case LabelType::IP6_ALL_ADDRESSES:      return F("IPv6 all addresses");
#endif
    case LabelType::IP_CONFIG_STATIC:       return F("Static");
    case LabelType::IP_CONFIG_DYNAMIC:      return F("DHCP");
    case LabelType::IP_ADDRESS:             return F("IP Address");
    case LabelType::IP_SUBNET:              return F("IP Subnet");
    case LabelType::IP_ADDRESS_SUBNET:      return F("IP / Subnet");
    case LabelType::GATEWAY:                return F("Gateway");
    case LabelType::CLIENT_IP:              return F("Client IP");
    #if FEATURE_MDNS
    case LabelType::M_DNS:                  return F("mDNS");
    #endif // if FEATURE_MDNS
    case LabelType::DNS:                    return F("DNS");
    case LabelType::DNS_1:                  return F("DNS 1");
    case LabelType::DNS_2:                  return F("DNS 2");
    case LabelType::ALLOWED_IP_RANGE:       return F("Allowed IP Range");
    case LabelType::STA_MAC:                return F("STA MAC");
    case LabelType::AP_MAC:                 return F("AP MAC");
    case LabelType::SSID:                   return F("SSID");
    case LabelType::BSSID:                  return F("BSSID");
    case LabelType::CHANNEL:                return F("Channel");
    case LabelType::ENCRYPTION_TYPE_STA:    return F("Encryption Type");
    case LabelType::CONNECTED:              return F("Connected");
    case LabelType::CONNECTED_MSEC:         return F("Connected msec");
    case LabelType::LAST_DISCONNECT_REASON: return F("Last Disconnect Reason");
    case LabelType::LAST_DISC_REASON_STR:   return F("Last Disconnect Reason str");
    case LabelType::NUMBER_RECONNECTS:      return F("Number Reconnects");
    case LabelType::WIFI_STORED_SSID1:      return F("Configured SSID1");
    case LabelType::WIFI_STORED_SSID2:      return F("Configured SSID2");


    case LabelType::FORCE_WIFI_BG:          return F("Force WiFi B/G");
    case LabelType::RESTART_WIFI_LOST_CONN: return F("Restart WiFi Lost Conn");
    case LabelType::FORCE_WIFI_NOSLEEP:     return F("Force WiFi No Sleep");
    case LabelType::PERIODICAL_GRAT_ARP:    return F("Periodical send Gratuitous ARP");
    case LabelType::CONNECTION_FAIL_THRESH: return F("Connection Failure Threshold");
    case LabelType::WAIT_WIFI_CONNECT:      return F("Extra Wait WiFi Connect");
    case LabelType::CONNECT_HIDDEN_SSID:    return F("Include Hidden SSID");
    case LabelType::HIDDEN_SSID_SLOW_CONNECT: return F("Hidden SSID Slow Connect");
    case LabelType::SDK_WIFI_AUTORECONNECT: return F("Enable SDK WiFi Auto Reconnect");

    case LabelType::BUILD_DESC:             return F("Build");
    case LabelType::GIT_BUILD:              return F("Git Build");
    case LabelType::SYSTEM_LIBRARIES:       return F("System Libraries");
    case LabelType::PLUGIN_COUNT:           return F("Plugin Count");
    case LabelType::PLUGIN_DESCRIPTION:     return F("Plugin Description");
    case LabelType::BUILD_TIME:             return F("Build Time");
    case LabelType::BINARY_FILENAME:        return F("Binary Filename");
    case LabelType::BUILD_PLATFORM:         return F("Build Platform");
    case LabelType::GIT_HEAD:               return F("Git HEAD");
    #ifdef CONFIGURATION_CODE
    case LabelType::CONFIGURATION_CODE_LBL: return F("Configuration code");
    #endif // ifdef CONFIGURATION_CODE

    case LabelType::I2C_BUS_STATE:          return F("I2C Bus State");
    case LabelType::I2C_BUS_CLEARED_COUNT:  return F("I2C bus cleared count");

    case LabelType::SYSLOG_LOG_LEVEL:       return F("Syslog Log Level");
    case LabelType::SERIAL_LOG_LEVEL:       return F("Serial Log Level");
    case LabelType::WEB_LOG_LEVEL:          return F("Web Log Level");
  #if FEATURE_SD
    case LabelType::SD_LOG_LEVEL:           return F("SD Log Level");
  #endif // if FEATURE_SD

    case LabelType::ESP_CHIP_ID:            return F("ESP Chip ID");
    case LabelType::ESP_CHIP_FREQ:          return F("ESP Chip Frequency");
#ifdef ESP32
    case LabelType::ESP_CHIP_XTAL_FREQ:     return F("ESP Crystal Frequency");
    case LabelType::ESP_CHIP_APB_FREQ:      return F("ESP APB Frequency");
#endif
    case LabelType::ESP_CHIP_MODEL:         return F("ESP Chip Model");
    case LabelType::ESP_CHIP_REVISION:      return F("ESP Chip Revision");
    case LabelType::ESP_CHIP_CORES:         return F("ESP Chip Cores");

    case LabelType::BOARD_NAME:         return F("ESP Board Name");

    case LabelType::FLASH_CHIP_ID:          return F("Flash Chip ID");
    case LabelType::FLASH_CHIP_VENDOR:      return F("Flash Chip Vendor");
    case LabelType::FLASH_CHIP_MODEL:       return F("Flash Chip Model");
    case LabelType::FLASH_CHIP_REAL_SIZE:   return F("Flash Chip Real Size");
    case LabelType::FLASH_CHIP_SPEED:       return F("Flash Chip Speed");
    case LabelType::FLASH_IDE_SIZE:         return F("Flash IDE Size");
    case LabelType::FLASH_IDE_SPEED:        return F("Flash IDE Speed");
    case LabelType::FLASH_IDE_MODE:         return F("Flash IDE Mode");
    case LabelType::FLASH_WRITE_COUNT:      return F("Flash Writes");
    case LabelType::SKETCH_SIZE:            return F("Sketch Size");
    case LabelType::SKETCH_FREE:            return F("Sketch Free");
    #ifdef USE_LITTLEFS
    case LabelType::FS_SIZE:                return F("Little FS Size");
    case LabelType::FS_FREE:                return F("Little FS Free");
    #else // ifdef USE_LITTLEFS
    case LabelType::FS_SIZE:                return F("SPIFFS Size");
    case LabelType::FS_FREE:                return F("SPIFFS Free");
    #endif // ifdef USE_LITTLEFS
    case LabelType::MAX_OTA_SKETCH_SIZE:    return F("Max. OTA Sketch Size");
    case LabelType::OTA_2STEP:              return F("OTA 2-step Needed");
    case LabelType::OTA_POSSIBLE:           return F("OTA possible");
    #if FEATURE_INTERNAL_TEMPERATURE
    case LabelType::INTERNAL_TEMPERATURE:   return F("Internal temperature (ESP32)");
    #endif // if FEATURE_INTERNAL_TEMPERATURE
#if FEATURE_ETHERNET
    case LabelType::ETH_IP_ADDRESS:         return F("Eth IP Address");
    case LabelType::ETH_IP_SUBNET:          return F("Eth IP Subnet");
    case LabelType::ETH_IP_ADDRESS_SUBNET:  return F("Eth IP / Subnet");
    case LabelType::ETH_IP_GATEWAY:         return F("Eth Gateway");
    case LabelType::ETH_IP_DNS:             return F("Eth DNS");
    case LabelType::ETH_MAC:                return F("Eth MAC");
    case LabelType::ETH_DUPLEX:             return F("Eth Mode");
    case LabelType::ETH_SPEED:              return F("Eth Speed");
    case LabelType::ETH_STATE:              return F("Eth State");
    case LabelType::ETH_SPEED_STATE:        return F("Eth Speed State");
    case LabelType::ETH_CONNECTED:          return F("Eth connected");
#endif // if FEATURE_ETHERNET
# if FEATURE_ETHERNET || defined(USES_ESPEASY_NOW)
    case LabelType::ETH_WIFI_MODE:          return F("Network Type");
#endif
    case LabelType::SUNRISE:                return F("Sunrise");
    case LabelType::SUNSET:                 return F("Sunset");
    case LabelType::SUNRISE_S:              return F("Sunrise sec.");
    case LabelType::SUNSET_S:               return F("Sunset sec.");
    case LabelType::SUNRISE_M:              return F("Sunrise min.");
    case LabelType::SUNSET_M:               return F("Sunset min.");
    case LabelType::ISNTP:                  return F("Use NTP");
    case LabelType::UPTIME_MS:              return F("Uptime (ms)");
    case LabelType::TIMEZONE_OFFSET:        return F("Timezone Offset");
    case LabelType::LATITUDE:               return F("Latitude");
    case LabelType::LONGITUDE:              return F("Longitude");

    case LabelType::MAX_LABEL:
      break;

  }
  return F("MissingString");
}

String getValue(LabelType::Enum label) {
  int retval = INT_MAX;
  switch (label)
  {
    case LabelType::UNIT_NR:                retval = Settings.Unit; break;
    #if FEATURE_ZEROFILLED_UNITNUMBER
    case LabelType::UNIT_NR_0: 
    {
      // Fixed 3-digit unitnumber
      return formatIntLeadingZeroes(Settings.Unit, 3);
    }
    #endif // FEATURE_ZEROFILLED_UNITNUMBER
    case LabelType::UNIT_NAME:              return Settings.getName(); // Only return the set name, no appended unit.
    case LabelType::HOST_NAME:              return NetworkGetHostname();


    case LabelType::LOCAL_TIME:             return node_time.getDateTimeString('-', ':', ' ');
    case LabelType::TIME_SOURCE:
    {
      String timeSource_str = toString(node_time.timeSource);
      if (((node_time.timeSource == timeSource_t::ESPEASY_p2p_UDP) ||
           (node_time.timeSource == timeSource_t::ESP_now_peer)) &&
          (node_time.timeSource_p2p_unit != 0))
      {
        return strformat(F("%s (%u)"), timeSource_str.c_str(), node_time.timeSource_p2p_unit);
      }
      return timeSource_str;
    }
    case LabelType::TIME_WANDER:            return String(node_time.timeWander, 1);
    #if FEATURE_EXT_RTC
    case LabelType::EXT_RTC_UTC_TIME:
    {
      if (Settings.ExtTimeSource() != ExtTimeSource_e::None) {
        // Try to read the stored time in the ext. time source to allow to check if it is working properly.
        uint32_t unixtime;
        if (node_time.ExtRTC_get(unixtime)) {
          struct tm RTC_time;
          breakTime(unixtime, RTC_time);
          return formatDateTimeString(RTC_time);
        } else {
          return F("Not Set");
        }
      }
      return String('-');
    }
    #endif
    case LabelType::UPTIME:                 retval = getUptimeMinutes(); break;
    case LabelType::LOAD_PCT:               return toString(getCPUload(), 2);
    case LabelType::LOOP_COUNT:             retval = getLoopCountPerSec(); break;
    case LabelType::CPU_ECO_MODE:           return jsonBool(Settings.EcoPowerMode());
#if FEATURE_SET_WIFI_TX_PWR
    case LabelType::WIFI_TX_MAX_PWR:        return toString(Settings.getWiFi_TX_power(), 2);
    case LabelType::WIFI_CUR_TX_PWR:        return toString(WiFiEventData.wifi_TX_pwr, 2);
    case LabelType::WIFI_SENS_MARGIN:       retval = Settings.WiFi_sensitivity_margin; break;
    case LabelType::WIFI_SEND_AT_MAX_TX_PWR:return jsonBool(Settings.UseMaxTXpowerForSending());
#endif
    case LabelType::WIFI_NR_EXTRA_SCANS:    retval = Settings.NumberExtraWiFiScans; break;
    case LabelType::WIFI_USE_LAST_CONN_FROM_RTC: return jsonBool(Settings.UseLastWiFiFromRTC());

    case LabelType::FREE_MEM:               retval = FreeMem(); break;
    case LabelType::FREE_STACK:             retval = getCurrentFreeStack(); break;

#ifdef USE_SECOND_HEAP
    case LabelType::FREE_HEAP_IRAM:         retval = FreeMem2ndHeap(); break;
#endif

#if defined(CORE_POST_2_5_0)
  #ifndef LIMIT_BUILD_SIZE
    case LabelType::HEAP_MAX_FREE_BLOCK:    retval = ESP.getMaxFreeBlockSize(); break;
  #endif
#endif // if defined(CORE_POST_2_5_0)
#if  defined(ESP32)
  #ifndef LIMIT_BUILD_SIZE
    case LabelType::HEAP_MAX_FREE_BLOCK:    retval = ESP.getMaxAllocHeap(); break;
  #endif
#endif // if  defined(ESP32)
#if defined(CORE_POST_2_5_0)
  #ifndef LIMIT_BUILD_SIZE
    case LabelType::HEAP_FRAGMENTATION:     retval = ESP.getHeapFragmentation(); break;
  #endif
#endif // if defined(CORE_POST_2_5_0)
#ifdef ESP32
    case LabelType::HEAP_SIZE:              retval = ESP.getHeapSize(); break;
    case LabelType::HEAP_MIN_FREE:          retval = ESP.getMinFreeHeap(); break;
    #ifdef BOARD_HAS_PSRAM
    case LabelType::PSRAM_SIZE:             retval = UsePSRAM() ? ESP.getPsramSize() : 0; break;
    case LabelType::PSRAM_FREE:             retval = UsePSRAM() ? ESP.getFreePsram() : 0; break;
    case LabelType::PSRAM_MIN_FREE:         retval = UsePSRAM() ? ESP.getMinFreePsram() : 0; break;
    case LabelType::PSRAM_MAX_FREE_BLOCK:   retval = UsePSRAM() ? ESP.getMaxAllocPsram() : 0; break;
    #endif // BOARD_HAS_PSRAM
#endif // ifdef ESP32


    case LabelType::JSON_BOOL_QUOTES:           return jsonBool(Settings.JSONBoolWithoutQuotes());
#if FEATURE_TIMING_STATS
    case LabelType::ENABLE_TIMING_STATISTICS:   return jsonBool(Settings.EnableTimingStats());
#endif
    case LabelType::ENABLE_RULES_CACHING:       return jsonBool(Settings.EnableRulesCaching());
    case LabelType::ENABLE_SERIAL_PORT_CONSOLE: return jsonBool(Settings.UseSerial);
    case LabelType::CONSOLE_SERIAL_PORT:        return ESPEasy_Console.getPortDescription();

#if USES_ESPEASY_CONSOLE_FALLBACK_PORT
    case LabelType::CONSOLE_FALLBACK_TO_SERIAL0: return jsonBool(Settings.console_serial0_fallback);
    case LabelType::CONSOLE_FALLBACK_PORT:       return ESPEasy_Console.getFallbackPortDescription();
#endif

//    case LabelType::ENABLE_RULES_EVENT_REORDER: return jsonBool(Settings.EnableRulesEventReorder()); // TD-er: Disabled for now
    case LabelType::TASKVALUESET_ALL_PLUGINS:   return jsonBool(Settings.AllowTaskValueSetAllPlugins());
    case LabelType::ALLOW_OTA_UNLIMITED:        return jsonBool(Settings.AllowOTAUnlimited());
#if FEATURE_CLEAR_I2C_STUCK
    case LabelType::ENABLE_CLEAR_HUNG_I2C_BUS:  return jsonBool(Settings.EnableClearHangingI2Cbus());
#endif
#if FEATURE_I2C_DEVICE_CHECK
    case LabelType::ENABLE_I2C_DEVICE_CHECK:    return jsonBool(Settings.CheckI2Cdevice());
#endif // if FEATURE_I2C_DEVICE_CHECK
#ifndef BUILD_NO_RAM_TRACKER
    case LabelType::ENABLE_RAM_TRACKING:        return jsonBool(Settings.EnableRAMTracking());
#endif
#if FEATURE_AUTO_DARK_MODE
    case LabelType::ENABLE_AUTO_DARK_MODE:      return toString(Settings.getCssMode());
#endif // FEATURE_AUTO_DARK_MODE
#if FEATURE_RULES_EASY_COLOR_CODE
    case LabelType::DISABLE_RULES_AUTOCOMPLETE: return jsonBool(Settings.DisableRulesCodeCompletion());
#endif // if FEATURE_RULES_EASY_COLOR_CODE

    case LabelType::BOOT_TYPE:              return getLastBootCauseString();
    case LabelType::BOOT_COUNT:             break;
    case LabelType::DEEP_SLEEP_ALTERNATIVE_CALL: return jsonBool(Settings.UseAlternativeDeepSleep());
    case LabelType::RESET_REASON:           return getResetReasonString();
    case LabelType::LAST_TASK_BEFORE_REBOOT: return ESPEasy_Scheduler::decodeSchedulerId(lastMixedSchedulerId_beforereboot);
    case LabelType::SW_WD_COUNT:            retval = sw_watchdog_callback_count; break;

    case LabelType::WIFI_CONNECTION:        break;
    case LabelType::WIFI_RSSI:              retval = WiFi.RSSI(); break;
    case LabelType::IP_CONFIG:              return useStaticIP() 
                                                     ? getLabel(LabelType::IP_CONFIG_STATIC) 
                                                     : getLabel(LabelType::IP_CONFIG_DYNAMIC);
    case LabelType::IP_CONFIG_STATIC:       break;
    case LabelType::IP_CONFIG_DYNAMIC:      break;
    case LabelType::IP_ADDRESS:             return formatIP(NetworkLocalIP());
    case LabelType::IP_SUBNET:              return formatIP(NetworkSubnetMask());
    case LabelType::IP_ADDRESS_SUBNET:      return strformat(F("%s / %s"), getValue(LabelType::IP_ADDRESS).c_str(), getValue(LabelType::IP_SUBNET).c_str());
    case LabelType::GATEWAY:                return formatIP(NetworkGatewayIP());
#if FEATURE_USE_IPV6
    case LabelType::IP6_LOCAL:              return formatIP(NetworkLocalIP6());
    case LabelType::IP6_GLOBAL:             return formatIP(NetworkGlobalIP6());
//    case LabelType::IP6_ALL_ADDRESSES:
    {
      IP6Addresses_t addresses = NetworkAllIPv6();
      String res;
      for (auto it = addresses.begin(); it != addresses.end(); ++it)
      {
        if (!res.isEmpty()) {
          res += F("<br>");
        }
        res += it->toString();
      }
      return res;
    }
#endif
    case LabelType::CLIENT_IP:              return formatIP(web_server.client().remoteIP());
    #if FEATURE_INTERNAL_TEMPERATURE
    case LabelType::INTERNAL_TEMPERATURE:   return toString(getInternalTemperature());
    #endif // if FEATURE_INTERNAL_TEMPERATURE

    #if FEATURE_MDNS
    case LabelType::M_DNS:                  return NetworkGetHostname() + F(".local");
    #endif // if FEATURE_MDNS
    case LabelType::DNS:                    return strformat(F("%s / %s"), getValue(LabelType::DNS_1).c_str(), getValue(LabelType::DNS_2).c_str());
    case LabelType::DNS_1:                  return formatIP(NetworkDnsIP(0));
    case LabelType::DNS_2:                  return formatIP(NetworkDnsIP(1));
    case LabelType::ALLOWED_IP_RANGE:       return describeAllowedIPrange();
    case LabelType::STA_MAC:                return WifiSTAmacAddress().toString();
    case LabelType::AP_MAC:                 return WifiSoftAPmacAddress().toString();
    case LabelType::SSID:                   return WiFi.SSID();
    case LabelType::BSSID:                  return WiFi.BSSIDstr();
    case LabelType::CHANNEL:                retval = WiFi.channel(); break;
    case LabelType::ENCRYPTION_TYPE_STA:    return // WiFi_AP_Candidates.getCurrent().encryption_type();
                                                   WiFi_encryptionType(WiFiEventData.auth_mode);
    case LabelType::CONNECTED:
      #if FEATURE_ETHERNET
      if(active_network_medium == NetworkMedium_t::Ethernet) {
        return format_msec_duration(EthEventData.lastConnectMoment.millisPassedSince());
      }
      #endif // if FEATURE_ETHERNET
      return format_msec_duration(WiFiEventData.lastConnectMoment.millisPassedSince());

    // Use only the nr of seconds to fit it in an int32, plus append '000' to have msec format again.
    case LabelType::CONNECTED_MSEC:         
      #if FEATURE_ETHERNET
      if(active_network_medium == NetworkMedium_t::Ethernet) {
        return String(static_cast<int32_t>(EthEventData.lastConnectMoment.millisPassedSince() / 1000ll)) + F("000"); 
      }
      #endif // if FEATURE_ETHERNET
      return String(static_cast<int32_t>(WiFiEventData.lastConnectMoment.millisPassedSince() / 1000ll)) + F("000");
    case LabelType::LAST_DISCONNECT_REASON: return String(WiFiEventData.lastDisconnectReason);
    case LabelType::LAST_DISC_REASON_STR:   return getLastDisconnectReason();
    case LabelType::NUMBER_RECONNECTS:      retval = WiFiEventData.wifi_reconnects; break;
    case LabelType::WIFI_STORED_SSID1:      return String(SecuritySettings.WifiSSID);
    case LabelType::WIFI_STORED_SSID2:      return String(SecuritySettings.WifiSSID2);


    case LabelType::FORCE_WIFI_BG:          return jsonBool(Settings.ForceWiFi_bg_mode());
    case LabelType::RESTART_WIFI_LOST_CONN: return jsonBool(Settings.WiFiRestart_connection_lost());
    case LabelType::FORCE_WIFI_NOSLEEP:     return jsonBool(Settings.WifiNoneSleep());
    case LabelType::PERIODICAL_GRAT_ARP:    return jsonBool(Settings.gratuitousARP());
    case LabelType::CONNECTION_FAIL_THRESH: retval = Settings.ConnectionFailuresThreshold; break;
    case LabelType::WAIT_WIFI_CONNECT:      return jsonBool(Settings.WaitWiFiConnect());
    case LabelType::CONNECT_HIDDEN_SSID:    return jsonBool(Settings.IncludeHiddenSSID());
    case LabelType::HIDDEN_SSID_SLOW_CONNECT: return jsonBool(Settings.HiddenSSID_SlowConnectPerBSSID());
    case LabelType::SDK_WIFI_AUTORECONNECT: return jsonBool(Settings.WifiNoneSleep());

    case LabelType::BUILD_DESC:             return getSystemBuildString();
    case LabelType::GIT_BUILD:
    {
      const String res(F(BUILD_GIT));

      if (!res.isEmpty()) { return res; }
      return get_git_head();
    }
    case LabelType::SYSTEM_LIBRARIES:       return getSystemLibraryString();
    case LabelType::PLUGIN_COUNT:           retval = getDeviceCount() + 1; break;
    case LabelType::PLUGIN_DESCRIPTION:     return getPluginDescriptionString();
    case LabelType::BUILD_TIME:             return String(get_build_date()) + ' ' + get_build_time();
    case LabelType::BINARY_FILENAME:        return get_binary_filename();
    case LabelType::BUILD_PLATFORM:         return get_build_platform();
    case LabelType::GIT_HEAD:               return get_git_head();
    #ifdef CONFIGURATION_CODE
    case LabelType::CONFIGURATION_CODE_LBL: return getConfigurationCode();
    #endif // ifdef CONFIGURATION_CODE
    case LabelType::I2C_BUS_STATE:          return toString(I2C_state);
    case LabelType::I2C_BUS_CLEARED_COUNT:  retval = I2C_bus_cleared_count; break;
    case LabelType::SYSLOG_LOG_LEVEL:       return getLogLevelDisplayString(Settings.SyslogLevel);
    case LabelType::SERIAL_LOG_LEVEL:       return getLogLevelDisplayString(getSerialLogLevel());
    case LabelType::WEB_LOG_LEVEL:          return getLogLevelDisplayString(getWebLogLevel());
  #if FEATURE_SD
    case LabelType::SD_LOG_LEVEL:           return getLogLevelDisplayString(Settings.SDLogLevel);
  #endif // if FEATURE_SD

    case LabelType::ESP_CHIP_ID:            return formatToHex(getChipId(), 6);
    case LabelType::ESP_CHIP_FREQ:          retval = ESP.getCpuFreqMHz(); break;
#ifdef ESP32
    case LabelType::ESP_CHIP_XTAL_FREQ:     retval = getXtalFrequencyMHz(); break;
    case LabelType::ESP_CHIP_APB_FREQ:      retval = rtc_clk_apb_freq_get() / 1000000; break;
    //getApbFrequency() / 1000000; break;
#endif
    case LabelType::ESP_CHIP_MODEL:         return getChipModel();
    case LabelType::ESP_CHIP_REVISION:      return getChipRevision();
    case LabelType::ESP_CHIP_CORES:         retval = getChipCores(); break;
    case LabelType::BOARD_NAME:             return get_board_name();
    case LabelType::FLASH_CHIP_ID:          return formatToHex(getFlashChipId(), 6);
    case LabelType::FLASH_CHIP_VENDOR:      return formatToHex(getFlashChipId() & 0xFF, 2);
    case LabelType::FLASH_CHIP_MODEL:
    {
      const uint32_t flashChipId = getFlashChipId();
      const uint32_t flashDevice = (flashChipId & 0xFF00) | ((flashChipId >> 16) & 0xFF);
      return formatToHex(flashDevice, 4);
    }
    case LabelType::FLASH_CHIP_REAL_SIZE:   retval = getFlashRealSizeInBytes(); break;
    case LabelType::FLASH_CHIP_SPEED:       retval = getFlashChipSpeed() / 1000000; break;
    case LabelType::FLASH_IDE_SIZE:         break;
    case LabelType::FLASH_IDE_SPEED:        break;
    case LabelType::FLASH_IDE_MODE:         return getFlashChipMode();
    case LabelType::FLASH_WRITE_COUNT:      break;
    case LabelType::SKETCH_SIZE:            break;
    case LabelType::SKETCH_FREE:            break;
    case LabelType::FS_SIZE:                retval = SpiffsTotalBytes(); break;
    case LabelType::FS_FREE:                retval = SpiffsFreeSpace(); break;
    case LabelType::MAX_OTA_SKETCH_SIZE:    break;
    case LabelType::OTA_2STEP:              break;
    case LabelType::OTA_POSSIBLE:           break;
#if FEATURE_ETHERNET
    case LabelType::ETH_IP_ADDRESS:         return formatIP(NetworkLocalIP());
    case LabelType::ETH_IP_SUBNET:          return formatIP(NetworkSubnetMask());
    case LabelType::ETH_IP_ADDRESS_SUBNET:  return strformat(
                                                          F("%s / %s"),
                                                          getValue(LabelType::ETH_IP_ADDRESS).c_str(),
                                                          getValue(LabelType::ETH_IP_SUBNET).c_str());
    case LabelType::ETH_IP_GATEWAY:         return formatIP(NetworkGatewayIP());
    case LabelType::ETH_IP_DNS:             return formatIP(NetworkDnsIP(0));
    case LabelType::ETH_MAC:                return NetworkMacAddress().toString();
    case LabelType::ETH_DUPLEX:             return EthLinkUp() ? (EthFullDuplex() ? F("Full Duplex") : F("Half Duplex")) : F("Link Down");
    case LabelType::ETH_SPEED:              return EthLinkUp() ? getEthSpeed() : F("Link Down");
    case LabelType::ETH_STATE:              return EthLinkUp() ? F("Link Up") : F("Link Down");
    case LabelType::ETH_SPEED_STATE:        return EthLinkUp() ? getEthLinkSpeedState() : F("Link Down");
    case LabelType::ETH_CONNECTED:          return ETHConnected() ? F("CONNECTED") : F("DISCONNECTED"); // 0=disconnected, 1=connected
#endif // if FEATURE_ETHERNET
# if FEATURE_ETHERNET || defined(USES_ESPEASY_NOW)
    case LabelType::ETH_WIFI_MODE:          return toString(active_network_medium);
#endif
    case LabelType::SUNRISE:                return node_time.getSunriseTimeString(':');
    case LabelType::SUNSET:                 return node_time.getSunsetTimeString(':');
    case LabelType::SUNRISE_S:              retval = node_time.sunRise.tm_hour * 3600 + node_time.sunRise.tm_min * 60 + node_time.sunRise.tm_sec; break;
    case LabelType::SUNSET_S:               retval = node_time.sunSet.tm_hour * 3600 + node_time.sunSet.tm_min * 60 + node_time.sunSet.tm_sec; break;
    case LabelType::SUNRISE_M:              retval = node_time.sunRise.tm_hour * 60 + node_time.sunRise.tm_min; break;
    case LabelType::SUNSET_M:               retval = node_time.sunSet.tm_hour * 60 + node_time.sunSet.tm_min; break;
    case LabelType::ISNTP:                  return jsonBool(Settings.UseNTP());
    case LabelType::UPTIME_MS:              return ull2String(getMicros64() / 1000);
    case LabelType::TIMEZONE_OFFSET:        retval = Settings.TimeZone; break;
    case LabelType::LATITUDE:               return toString(Settings.Latitude, 6);
    case LabelType::LONGITUDE:              return toString(Settings.Longitude, 6);

    case LabelType::MAX_LABEL:
      break;
  }
  if (retval != INT_MAX) return String(retval);
  return F("MissingString");
}

#if FEATURE_ETHERNET
String getEthSpeed() {
  return strformat(F("%dMbps"), EthLinkSpeed());
}

String getEthLinkSpeedState() {
  if (EthLinkUp()) {
    return strformat(F("%s %s %s"), 
    getValue(LabelType::ETH_STATE).c_str(), 
    getValue(LabelType::ETH_DUPLEX).c_str(), 
    getEthSpeed().c_str());
  }
  return getValue(LabelType::ETH_STATE);
}

#endif // if FEATURE_ETHERNET

String getExtendedValue(LabelType::Enum label) {
  switch (label)
  {
    case LabelType::UPTIME:
    {
      return minutesToDayHourMinute(getUptimeMinutes());
    }

    default:
      break;
  }
  return EMPTY_STRING;
}
