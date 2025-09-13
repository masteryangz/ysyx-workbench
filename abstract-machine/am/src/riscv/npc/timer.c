#include <am.h>
#include "../riscv.h"
#include "npc.h"
#include <sys/time.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t lo = inl(RTC_ADDR);       // 低 32 位
  uint32_t hi = inl(RTC_ADDR + 4);   // 高 32 位
  uint64_t ms = ((uint64_t)hi << 32) | lo;  // 毫秒
  uptime->us = ms;
  //uptime->us = 0;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
