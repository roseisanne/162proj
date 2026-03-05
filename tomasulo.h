#pragma once
#include <string>

struct ReservationStation
{
    bool busy{false};
    std::string op;
    int Vj{0};
    int Vk{0};
    std::string Qj{-1};
    std::string Qk{-1};
    int A{0}; //ld/str addresses
    int dest{-1}; //destination register
    int result{-1}; //result of the operation
};

struct RegisterStatus
{
    bool ready{true}; 
    int tag{-1}; 
};

struct FunctionalUnit
{
    bool busy{false};

    //tag of the reservation station currently using this unit
    int rsTag{-1};
    int cyclesLeft{0}; //cycles left for current operation
};

struct CDB
{
    bool busy{false};
    int tag{-1}; //tag of the reservation station broadcasting result
    float value{0}; //value being broadcast
}