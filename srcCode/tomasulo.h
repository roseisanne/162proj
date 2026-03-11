#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

struct ReservationStation
{
    bool busy{false};
    std::string op{""};
    float Vj{0};
    float Vk{0};
    int Qj{-1};
    int Qk{-1};
    int A{0}; //ld/str addresses

    //destination register
    int dest{-1};
    std::string type{""};
    bool executing{false};

    ReservationStation(std::string t) : type(t) {}

    void printStation()
    {
    std::cout << "busy?: " << busy << "\n";
    std::cout << "RS op: " << op << "\n";
    std::cout << "dest: " << dest << "\n";
    std::cout << "Vj: " << Vj << " Vk: " << Vk << "\n";
    std::cout << "Qj: " << Qj << " Qk: " << Qk << "\n";
    std::cout << "A: " << A << "\n";
    std::cout << "type: " << type << "\n";
    }
};

struct RegisterStatus
{
    bool ready{true}; 

    //of RS station
    int tag{-1};
};

struct FunctionalUnit
{
    bool busy{false};

    std::string type{""};
    std::string op{""};
    
    //tag of the reservation station currently using this unit
    int rsTag{-1};
    //cycles left for current operation
    int cyclesLeft{0};
};

struct CDB
{
    bool busy{false};
    //tag of the reservation station broadcasting result
    int tag{-1};
    //value being broadcast
    float value{0};
};