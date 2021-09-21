#ifndef TIME_SCHEDULER_H
#define TIME_SCHEDULER_H
class Scheduler{
public:
    unsigned int index = 0;     // sizeof(unsigned int) = 2
    unsigned int size = 0;
    bool busy = false;          // sizeof(bool) = 1
    unsigned long *times;       // sizeof(unsigned long) = 4 (pointer if ofcourse 2 bytes)

    /*|index|size|busy|time|times       |
      |0-2  |2-4 |4-5 |5-9 |9-(9+size*4)|*/
    Scheduler();
    ~Scheduler();
    // writes everything to eeprom memory (doesn't change after power off)
    void write();
    // resests everything, inclusive eeprom
    void reset();
    // inserts 2 times (since 1970 in seconds) in times
    void insert(unsigned long time, unsigned long duration);
    // sets SOLENOID_PIN to HIGH if necesarry and only then return true
    bool check_and_execute();
    // infinit loop such that when the power is shut off the memory is not corrupted
    void exit();
};
#endif