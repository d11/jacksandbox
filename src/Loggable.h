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
