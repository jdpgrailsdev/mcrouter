/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "Util.h"

#include <folly/Format.h>

#include "mcrouter/lib/mc/msg.h"

namespace facebook { namespace memcache {

std::vector<std::string> describeFlags(uint64_t flags) {
  std::vector<std::string> out;

  uint64_t f = 1;
  while (flags > 0) {
    if (flags & f) {
      out.push_back(mc_flag_to_string(static_cast<enum mc_msg_flags_t>(f)));
      flags &= ~f;
    }
    f <<= 1;
  }

  return out;
}

std::string printTimeAbsolute(const struct timeval& ts) {
  struct tm t;
  localtime_r((const time_t *)&ts.tv_sec, &t);

  return folly::sformat("{:02}/{:02}/{:02} {:02}:{:02}:{:02}.{:06} ",
                       t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour,
                       t.tm_min, t.tm_sec, static_cast<uint32_t>(ts.tv_usec));
}

std::string printTimeDiff(const struct timeval& ts,
                          struct timeval& prev) {
  uint32_t secs, usecs;

  secs = ts.tv_sec - prev.tv_sec;
  if (ts.tv_usec >= prev.tv_usec) {
    usecs = ts.tv_usec - prev.tv_usec;
  } else {
    secs--;
    usecs = 1000000 - (prev.tv_usec - ts.tv_usec);
  }

  prev.tv_sec = ts.tv_sec;
  prev.tv_usec = ts.tv_usec;

  return folly::sformat("+{}.{:06} ", secs, usecs);
}

std::string printTimeOffset(const struct timeval& ts,
                            struct timeval& prev) {
  uint32_t secs, usecs;

  secs = ts.tv_sec - prev.tv_sec;
  if (ts.tv_usec >= prev.tv_usec) {
    usecs = ts.tv_usec - prev.tv_usec;
  } else {
    secs--;
    usecs = 1000000 - (prev.tv_usec - ts.tv_usec);
  }

  if (prev.tv_sec == 0 && prev.tv_usec == 0) {
    prev.tv_sec = ts.tv_sec;
    prev.tv_usec = ts.tv_usec;
    secs = 0;
    usecs = 0;
  }

  return folly::sformat("+{}.{:06} ", secs, usecs);
}

}} // facebook::memcache
