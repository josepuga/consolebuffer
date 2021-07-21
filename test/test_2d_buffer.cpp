#include <iostream>
#include <ansi/ansi.hpp>  // https://github.com/josepuga/ansi
#include "consolebuffer.hpp"

using namespace std;
int main() 
{
    cout << ansi::clear_all << ansi::goto_xy(4,2);
    cout << "Sample using ConsoleBuffer as 2D matrix instead 1D vector." << endl;
    ConsoleBuffer cb;
    cb.SetSize(50, 12);
    cb.SetPos(3, 4);
    cb.FillBg(0xbb8fce);
    cb.FillChr('#');
    for (auto y = 0; y < cb.GetBufferHeight(); y++) {
        for (auto x = 0; x < cb.GetBufferWidth(); x++) {
            if (x % 2) {
                cb.At(x,y).Chr('.');
                cb.At(x,y).Bg(0x2240be);
            }
        }
    }
    cb.Show();
    cout << ansi::reset << endl; // Console default color/attributes state.
    return 0;
}
