#include "plc_standardlib.h"

/*
TP
*/
TP::TP() {
    IN = false;
    PT = 0;
    Q = false;
    ET = 0;
}

TP::TP(ulong pt) {
    IN = false;
    PT = pt;
    Q = false;
    ET = 0;
    t0 = 0;
}

boolean TP::process() {
    if (IN) {
        if (t0==0) {
            t0 = millis();
        }
    }
    if (t0 > 0) {
        ET = millis() - t0;
        if (ET >= PT) {
            ET = PT;
        }
    }
    if (!IN && ET == PT) {
        t0 = 0;
        ET = 0;
    }
    Q = ET > 0 && ET < PT;
    return Q;
}

boolean TP::process(boolean in) {
    IN = in;
    return process();
}

/*
TON
*/
TON::TON(ulong pt) {
  IN = false;
  PT = pt;
  ET = 0;
  Q = false;
  t0 = 0;
}

boolean TON::process() {
  if (!IN) {
    ET = 0;
    t0 = 0;
    Q = false;
  }
  else {
    if (t0 == 0) {
      t0 = millis();
    }
    ET = millis() - t0;
    if (ET >= PT) {
      ET = PT;
      Q = true;
    }
  }
  return Q;
}

boolean TON::process(boolean in) {
  IN = in;
  return process();
}

/*
TOF
*/
TOF::TOF(ulong pt) {
  IN = false;
  PT = pt;
  ET = 0;
  Q = false;
  t0 = 0;
}

boolean TOF::process() {
  if (IN) {
    ET = 0;
    t0 = 0;
    Q = true;
  }
  else {
    if (t0 == 0) {
      t0 = millis();
    }
    ET = millis() - t0;
  }
  if (ET >= PT) {
    ET = PT;
    Q = false;
  }
  return Q;
}

boolean TOF::process(boolean in) {
  IN = in;
  return process();
}

/*
R_TRIG
*/
R_TRIG::R_TRIG() {
  CLK = false;
  Q = false;
  m = false;
}

boolean R_TRIG::process() {
  Q = CLK && !m;
  m = CLK;
  return Q;
}

boolean R_TRIG::process(boolean clk) {
  CLK = clk;
  return process();
}

/*
F_TRIG
*/
F_TRIG::F_TRIG() {
  CLK = false;
  Q = false;
  m = false;
}

boolean F_TRIG::process() {
  Q = !CLK && m;
  m = CLK;
  return Q;
}

boolean F_TRIG::process(boolean clk) {
  CLK = clk;
  return process();
}

/*
SR
*/
SR::SR() {
    SET1 = false;
    RESET = false;
    Q1 = false;
}

boolean SR::process() {
    if (SET1) {
        Q1 = true;
    }
    else {
        if (RESET) {
            Q1 = false;
        }
    }
    return Q1;
}

boolean SR::process(boolean set1, boolean reset) {
    SET1 = set1;
    RESET = reset;
    return process();
}

/*
RS
*/
RS::RS() {
    SET = false;
    RESET1 = false;
    Q1 = false;
}

boolean RS::process() {
    if (RESET1) {
        Q1 = false;
    }
    else {
        if (SET) {
            Q1 = true;
        }
    }
    return Q1;
}

boolean RS::process(boolean set, boolean reset1) {
    SET = set;
    RESET1 = reset1;
    return process();
}

/*
SEMA
*/
SEMA::SEMA() {
    CLAIM = false;
    RELEASE = false;
    BUSY = false;
    x = false;
}

boolean SEMA::process() {
    BUSY = x;
    if (CLAIM) {
        x = true;
    }
    else {
        if (RELEASE) {
            BUSY = false;
            x = false;
        }
    }
    return BUSY;
}

boolean SEMA::process(boolean claim, boolean release) {
    CLAIM = claim;
    RELEASE = release;
    return process();
}
