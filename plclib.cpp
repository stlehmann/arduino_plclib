#include "plclib.h"

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

bool TP::process() {
    if (IN) {
        if (t0==0) {
            t0 = plclib_millis();
        }
    }
    if (t0 > 0) {
        ET = plclib_millis() - t0;
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

bool TP::process(bool in) {
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

bool TON::process() {
  if (!IN) {
    ET = 0;
    t0 = 0;
    Q = false;
  }
  else {
    if (t0 == 0) {
      t0 = plclib_millis();
    }
    ET = plclib_millis() - t0;
    if (ET >= PT) {
      ET = PT;
      Q = true;
    }
  }
  return Q;
}

bool TON::process(bool in) {
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

bool TOF::process() {
  if (IN) {
    ET = 0;
    t0 = 0;
    Q = true;
  }
  else {
    if (t0 == 0) {
      t0 = plclib_millis();
    }
    ET = plclib_millis() - t0;
  }
  if (ET >= PT) {
    ET = PT;
    Q = false;
  }
  return Q;
}

bool TOF::process(bool in) {
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

bool R_TRIG::process() {
  Q = CLK && !m;
  m = CLK;
  return Q;
}

bool R_TRIG::process(bool clk) {
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

bool F_TRIG::process() {
  Q = !CLK && m;
  m = CLK;
  return Q;
}

bool F_TRIG::process(bool clk) {
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

bool SR::process() {
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

bool SR::process(bool set1, bool reset) {
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

bool RS::process() {
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

bool RS::process(bool set, bool reset1) {
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

bool SEMA::process() {
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

bool SEMA::process(bool claim, bool release) {
    CLAIM = claim;
    RELEASE = release;
    return process();
}

/*
CTU
*/
CTU::CTU(uint pv) {
    CU = false;
    RESET = false;
    Q = false;
    PV = pv;
    CV = 0;
    cu_old = false;
}

bool CTU::process() {
    if (RESET) {
        CV = 0;
    }
    if (CU &! cu_old) {
        CV++;
    }
    Q = (CV >= PV);
    cu_old = CU;
    return Q;
}

bool CTU::process(bool cu, bool reset) {
    CU = cu;
    RESET = reset;
    return process();
}

/*
CTD
*/
CTD::CTD(uint pv) {
    CD = false;
    LOAD = false;
    Q = false;
    PV = pv;
    CV = 0;
    cd_old = false;
}

bool CTD::process() {
    if (LOAD) {
        CV = PV;
    }
    if (CD &! cd_old) {
        if (CV > 0) {
            CV--;
        }
    }
    Q = (CV == 0);
    cd_old = CD;
    return Q;
}

bool CTD::process(bool cd, bool load) {
    CD = cd;
    LOAD = load;
    return process();
}

/*
CTUD
*/
CTUD::CTUD(uint pv) {
    CU = false;
    CD = false;
    RESET = false;
    LOAD = false;
    QU = false;
    QD = false;
    PV = pv;
    CV = 0;
    cu_old = false;
    cd_old = false;
}

bool CTUD::process() {
    if (RESET) {
        CV = 0;
    }
    else if (LOAD) {
        CV = PV;
    }
    if (CU &! cu_old) {
        CV++;
    }
    if (CD &! cd_old) {
        if (CV > 0) {
            CV--;
        }
    }
    QU = (CV >= PV);
    QD = (CV == 0);
    cu_old = CU;
    cd_old = CD;
    return QU || QD;
}

bool CTUD::process(bool cu, bool cd, bool reset, bool load) {
    CU = cu;
    CD = cd;
    RESET = reset;
    LOAD = load;
    return process();
}
