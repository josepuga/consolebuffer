#include <iostream>
#include <chrono>
#include <random>
#include <ansi/ansi.hpp>  // https://github.com/josepuga/ansi
#include "consolebufferu8.hpp"

using namespace std;
int main() 
{

    ConsoleBufferU8 cb1(80,5);
    ConsoleBufferU8 cb2(27,8);
    cb2.SetPos(5,9);
    ConsoleBufferU8 cb3(45,6);
    cb3.SetPos(37,7);

    default_random_engine gen;
    uniform_int_distribution<uint32_t> rndColor(0x222222, 0xfffff);
    uniform_int_distribution<char> rndAscii(' ', 'Z');

    cout << ansi::clear_all << ansi::hide_cursor;
    auto timeStart = std::chrono::high_resolution_clock::now();
    const int ITERACTIONS = 500;
    for (auto n = 0; n < ITERACTIONS; n++) {
        for (auto cb : {cb1, cb2, cb3}) {
            for (auto& cell : cb.GetBuffer()) {
                cell.bg = rndColor(gen);
                cell.fg = rndColor(gen);
                cell.chr = "Þ";        
            }
            cb.Show();
        }
    }
    auto timeStop = std::chrono::high_resolution_clock::now();
    cout << ansi::reset << ansi::show_cursor << endl;
    auto dt = std::chrono::duration<float, 
            std::chrono::seconds::period>(timeStop - timeStart).count();
    cout << "FPS: " << ITERACTIONS/dt << endl;
    return 0;
}
