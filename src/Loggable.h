#ifndef __LOGGABLE__
#define __LOGGABLE__

#include <iostream>
#include <string>

using namespace std;

class Loggable {
private:
   const char *mName;
public:
   Loggable(const char *name) : mName(name) { }
   void Log(const char *msg) {
      cout << "[" << mName << "] " << msg << endl;
   }
   void Log(const string &msg) {
      Log(msg.c_str());
   }
};

#endif /* End of include guard: __LOGGABLE__ */
