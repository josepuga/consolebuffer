#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <ansi/ansi.hpp>  // https://github.com/josepuga/ansi
#include "consolebuffer.hpp"

using namespace std;
int main() 
{

    ConsoleBuffer cb1(80,5);
    ConsoleBuffer cb2(27,8);
    cb2.SetPos(5,9);
    ConsoleBuffer cb3(45,6);
    cb3.SetPos(37,7);
    std::vector<std::string> utf8Chars {
        "ɮ","ɸ","Ϋ","Ω","Ϣ","ϴ","Ͼ","Ͽ","©","®","õ","Ħ","Ų","Ȝ","ȹ","ȸ","ɷ","ʬ","·"
    };

    default_random_engine gen;
    uniform_int_distribution<uint32_t> rndColor(0x222222, 0xfffff);
    uniform_int_distribution<int> rndUtf8(0, utf8Chars.size()-1);

    cout << ansi::clear_all << ansi::hide_cursor;
    auto timeStart = std::chrono::high_resolution_clock::now();
    const int ITERACTIONS = 500;
    for (auto n = 0; n < ITERACTIONS; n++) {
        for (auto cb : {cb1, cb2, cb3}) {
            for (auto& cell : cb.GetBuffer()) {
                cell.Fg(rndColor(gen));
                cell.Bg(rndColor(gen));
                auto utf8Char = utf8Chars.at(rndUtf8(gen));
                cell.Chr(utf8Char);
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
