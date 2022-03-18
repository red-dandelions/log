#include "log.h"
using namespace dailyrecord;
int main() {
  LogInit();
  LOG(INFO) << "test";
  LogClose();
  return 0;
}
