#include <iostream>
#include <chrono>
#include <random>
#include <ansi/ansi.hpp>  // https://github.com/josepuga/ansi
#include "consolebuffer.hpp"

using namespace std;
int main() 
{
    ConsoleBuffer cb(80,20);

    default_random_engine gen;
    uniform_int_distribution<uint32_t> rndColor(0x222222, 0xfffff);
    uniform_int_distribution<char> rndAscii(' ', 'Z');

    cout << ansi::clear_all << ansi::hide_cursor;
    auto timeStart = std::chrono::high_resolution_clock::now();
    const int ITERACTIONS = 500;
    for (auto n = 0; n < ITERACTIONS; n++) {
    
        for (auto& cell : cb.GetBuffer()) {
            cell.Fg(rndColor(gen));
            cell.Bg(rndColor(gen));
            cell.Chr(rndAscii(gen));
        }
        cb.Show();
    }
    auto timeStop = std::chrono::high_resolution_clock::now();
    cout << ansi::reset << ansi::show_cursor << endl;
    auto dt = std::chrono::duration<float, 
            std::chrono::seconds::period>(timeStop - timeStart).count();
    cout << "FPS: " << ITERACTIONS/dt << endl;
    return 0;
}
