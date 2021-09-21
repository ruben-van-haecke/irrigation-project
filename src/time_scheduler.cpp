#include <time_scheduler.h>
#include <TimeLib.h>
#include <Constants.h>
#include <Arduino.h>
#include <EEPROM.h>

Scheduler::Scheduler(){
    EEPROM.get(INDEX_START, index);
    EEPROM.get(SIZE_START, size);
    EEPROM.get(BUSY_START, busy);
    unsigned long t;
    EEPROM.get(TIME_START, t);
    setTime(t);

    if(size != 0){
        times = new unsigned long[size];
        EEPROM.get(TIMES_START, times);
    }else{
        times = nullptr;
    }
}

Scheduler::~Scheduler(){
    delete[] times;
}


void Scheduler::write(){
    EEPROM.put(INDEX_START, index);
    EEPROM.put(SIZE_START, size);
    EEPROM.put(BUSY_START, busy);
    EEPROM.put(TIME_START, now());
    EEPROM.put(TIMES_START, times);
}

void Scheduler::reset(){
    // clear all memory on eeprom
    for(unsigned int i=0; i<EEPROM.length(); i++){
        EEPROM.write(i, 0);
    }
    index = 0;
    size = 0;
    busy = 0;
    delete[] times;
    times = nullptr;
}

void Scheduler::insert(unsigned long time, unsigned long duration){
    size += 2;
    // case 1: empty array
    if(times == nullptr){
        unsigned long *new_times = new unsigned long[2];
        new_times[0] = time;
        new_times[1] = time + duration;
        delete[] times;
        times = new_times;
        write();
        return;
    }
    // case 2: no empty array
    Serial.println(times[0]);
    Serial.println(times[1]);
    unsigned long *new_times = new unsigned long[size];
    int offset = 0;
    for(unsigned int i=0; i < size-2; i += 2){
        if((offset == 0) && (time <= times[i])){
            new_times[i] = time;
            new_times[i+1] = time + duration;
            offset = 2;
        }
        new_times[i+offset] = times[i];
        new_times[i+1+offset] = times[i+1];
    }
    if(offset == 0){    // new timestamps have to be added at the end
        new_times[size-2] = time;
        new_times[size-1] = time + duration;
    }
    delete[] times;
    times = new_times;
    write();
}

bool Scheduler::check_and_execute(){
    time_t t = now();
    Serial.println(index);
    Serial.println(t);
    Serial.println("---------------------");
    Serial.println(times[0]);
    Serial.println(times[1]);
    Serial.println(times[2]);
    Serial.println(times[3]);
    if((not busy) && (t > times[index])){
        Serial.println("on");
        times[index] += 86400;   // day in seconds
        index = (index+1)%size;
        busy = true;
        digitalWrite(PINSOLENOID, HIGH);
        write();
        return true;
    }
    if(busy && (t > times[index])){
        Serial.println("off");
        times[index] += 86400;
        index = (index+1)%size;
        busy = false;
        digitalWrite(PINSOLENOID, LOW);
    }
    write();
    return false;
}

void Scheduler::exit(){
    while(true){;}
}