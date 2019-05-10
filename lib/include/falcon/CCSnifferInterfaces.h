#pragma once

#include <vector>
#include <algorithm>
#include <stdint.h>

#include "definitions.h"

class CCSnifferInterface {

};

class SnifferConfigInterface {

};

class PushData {
public:
  virtual ~PushData() {}
     // Example container class for data to be pushed directly to the subscribers
};

class LineData {
public:
  virtual ~LineData() {}
  // Example container class for data to be pulled by the subscribers
};

template <typename PushDataType>
class Subscriber {
public:
  virtual void update() = 0;
  virtual void push(const PushDataType*) = 0;
  virtual void notifyDetach() = 0;  // Forced detach
};

template <typename PushDataType>
class Provider {

private:
  std::vector<Subscriber<PushDataType>*> subscriber;

public:
  virtual ~Provider() {
    for (typename std::vector<Subscriber<PushDataType>*>::size_type i = 0; i < subscriber.size(); i++)
      subscriber[i]->notifyDetach();
  }

  void subscribe(Subscriber<PushDataType>* s) {
    subscriber.push_back(s);
  }

  void unsubscribe(Subscriber<PushDataType>* s) {
     subscriber.erase(std::remove(subscriber.begin(), subscriber.end(), s), subscriber.end());
  }

  void notifySubscribers() {
    for (typename std::vector<Subscriber<PushDataType>*>::size_type i = 0; i < subscriber.size(); i++)
      subscriber[i]->update();
  }

  void pushToSubscribers(const PushDataType* data) {
    for (typename std::vector<Subscriber<PushDataType>*>::size_type i = 0; i < subscriber.size(); i++)
      subscriber[i]->push(data);
  }

protected:

};

typedef enum {
  SCAN_LINE_UPLINK = 0,
  SCAN_LINE_DOWNLINK = 1,
  SCAN_LINE_SPECTRUM = 2,
  SCAN_LINE_SPECTRUM_DIFF = 3,
  SCAN_LINE_RNTI_HIST = 4
} ScanLineType_t;

class ScanLineLegacy : public LineData, public PushData {
public:
  virtual ~ScanLineLegacy(){}
  ScanLineType_t type;
  float linebuf[SPECTROGRAM_LINE_WIDTH];
  uint32_t rnti_hist[65536];
};

class ScanLine {
public:
  ScanLine(float* line, float nItems) : line(line), nItems(nItems) { }
  ScanLine(const ScanLine& ) = delete;  //prevent copy
  ScanLine& operator=(const ScanLine&) = delete;  //prevent copy
  ~ScanLine() { }
private:
  float* line;
  float nItems;
};

//class SnifferVisualizationInterface : public Provider {
//  // to be filled with methods...
//  virtual LineData* getUplinkLine() { return NULL; };
//  virtual LineData* getDownlinkLine() { return NULL; };
//};