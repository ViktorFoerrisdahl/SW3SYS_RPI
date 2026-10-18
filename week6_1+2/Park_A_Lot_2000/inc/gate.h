#ifndef GATE_H
#define GATE_H

#include <mutex>
#include <condition_variable>

enum TYPE_OF_GATE
{
    ENTRY,
    EXIT
};

class Gate
{

private:
    //individuel gate logik
    std::mutex mtx_;
    std::condition_variable pred_;
    int carsWaiting_ = 0;
    bool open_ = false;
    TYPE_OF_GATE type_;

    //fælles logik
    static std::mutex common_mtx_; //Fælles lås, så kun en gate kan opdatere variabler ad gangen
    static int capacity_; //Antallet af ekstra biler, der er plads til på et givent tidspunkt
    static int numberOfCarsCounter_; //Antallet af biler, der findes på parkeringspladssen på et givent tidspunkt
    static std::condition_variable common_pred_; //En fælles contidional variabel til at holde styr på capacity_

public:
    Gate(TYPE_OF_GATE t); //Constructor som udelukkende skal vide, om det er en entry eller exit gate
    void active(); //Dette er gatens hovedlogik, denne kører i evigt while loop
    void requestEntry(); //Bilen kalder denne på den gate, den gerne vil ind/ud af for at in/de-krementere carsWaiting variablen og notify gaten
    void wait_until_open_entry(int ID); //Bilen kalder denne metode for at komme ind
    void wait_until_open_exit(int ID); //Bilen kalder denne metode for at komme ud
    void update_capacity_and_number_of_cars(int i); //Opdaterer fælles variable
};

#endif