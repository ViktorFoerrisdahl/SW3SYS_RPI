#include "gate.h"
#include <iostream>

int Gate::numberOfCarsCounter_ = 0;
int Gate::capacity_ = 4;
std::mutex Gate::common_mtx_;
std::condition_variable Gate::common_pred_;

Gate::Gate(TYPE_OF_GATE t) : type_(t) {};

void Gate::active()
{
    while (1)
    {

        if (type_ == ENTRY)
        {

            std::unique_lock<std::mutex> lk(mtx_);                                           // Opretter unique lock, som kan deles mellem gates


            std::cout << std::endl << "Gate::active:     Entry gate venter..." << std::endl; // udprinter status
            pred_.wait(lk, [this]() { return carsWaiting_ > 0; });                           // Gaten går i dvale indtil carswaiting er > 0.
            lk.unlock();

            {
            std::unique_lock<std::mutex> commonLock(common_mtx_);

            common_pred_.wait(commonLock, [this]() { return capacity_ > 0; }); // Her ventes der på, at der er plads på parkeringspladsen, den bliver notifiet, når en bil kører ud

            std::cout << std::endl << "Gate::active:     Entry gate done med at vente og nu aabnes gaten" << std::endl;
            }

            lk.lock();
            open_ = true; // åbner gaten

            pred_.notify_one(); // sender signal til den bil, som venter på at gaten er åben, at gaten nu er blevet åbnet.

            pred_.wait(lk, [&]() { return !open_; }); // Her venter vores gate på, at bilen sætter open til false, da vi her ved at bilen er kørt ud
        }

        if (type_ == EXIT) //logikken er nogenlunde ens her i forhold til entry
        {
            std::unique_lock<std::mutex> lk(mtx_);
            std::cout << std::endl << "Gate::active:     Exit gate venter..." << std::endl;
            pred_.wait(lk, [this]() { return carsWaiting_ > 0; }); // holder låsen låst indtil carwaiting er true

            std::cout << "Gate::active:     Exit gate done med at vente og nu aabnes gaten" << std::endl;

            open_ = true; // åbner gaten

            pred_.notify_one(); // sender signal til den bil, som venter på at gaten er åben, at gaten nu er blevet åbnet.


            pred_.wait(lk, [&]() { return !open_; });
        }
    }
};

void Gate::requestEntry()
{

    {
        std::lock_guard<std::mutex> lk(mtx_); // kritisk sektion, så kun 1 bil kan opdatere carswaiting af gangen
        carsWaiting_++;
    }

    pred_.notify_all(); // sender signal til både biler og gates, hvor gates vågner og tjekker carswaiting. Denne er nu lavet om og koden fortsætter i gaten.
}

void Gate::update_capacity_and_number_of_cars(int i) 
{
    std::lock_guard<std::mutex> lock(common_mtx_); //lokal lås for at sørge for, at kun en thread kan ændre på disse varaible af gangen.
    capacity_ -= i;
    numberOfCarsCounter_ += i;
}

void Gate::wait_until_open_entry(int ID)
{
    std::unique_lock<std::mutex> lk(mtx_);
    std::cout << "Gate::wait_until_open_entry:  Bilen med ID: " << ID << " venter paa gaten aabnes..." << std::endl;

    pred_.wait(lk, [this] { return open_; }); // Venter på at gaten er åben

    std::cout << "Gate::wait_until_open_entry:  Gaten er aaben. Bil med ID: " << ID << " koerer ind..." << "\n\n";

    open_ = false; // lukker gaten igen med det samme

    update_capacity_and_number_of_cars(1); // her kaldes en metode til at opdaterer nogle fælles variable. Denne metode har sin egen lås, da både entry og exit gate ellers ville kunne opdatere variablerne samtidigt.

    carsWaiting_--;

    pred_.notify_all(); //sender signal til gaten / bilerne, at numberofcarscounter er opdateret og derved kan den vågne op og gå videre i sin kode.

    std::cout << "Gate::wait_until_open_entry:  Antallet af biler paa parkeringspladsen er: " << numberOfCarsCounter_ << std::endl;
    std::cout << "Gate::wait_until_open_entry:  Capacity paa parkeringspladsen er: " << capacity_ << "\n\n";
}

void Gate::wait_until_open_exit(int ID)
{

    std::unique_lock<std::mutex> lk(mtx_);
    std::cout << "Gate::wait_until_open_exit    Bilen med ID: " << ID << " venter paa gaten aabnes..." << std::endl;
    pred_.wait(lk, [this] { return open_; });
    std::cout << "Gate::wait_until_open_exit    Gaten er aaben. Bil med ID: " << ID << " koerer ud..." << "\n\n";

    open_ = false;

    update_capacity_and_number_of_cars(-1); // her kaldes en metode til at opdaterer nogle fælles variable. Denne metode har sin egen lås, da både entry og exit gate ellers ville kunne opdatere variablerne samtidigt.

    common_pred_.notify_one(); // sender signal om, at der er kørt en bil ud

    carsWaiting_--;

    pred_.notify_all();

    std::cout << "Gate::wait_until_open_exit    Antallet af biler paa parkeringspladsen er: " << numberOfCarsCounter_ << std::endl;
    std::cout << "Gate::wait_until_open_exit    Capacity paa parkeringspladsen er: " << capacity_ << "\n\n";
}