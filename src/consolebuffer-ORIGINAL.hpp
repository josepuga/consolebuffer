/*
MIT License

Copyright (c) 2021 José Puga

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CONSOLEBUFFER_HPP
#define CONSOLEBUFFER_HPP
#define CONSOLEBUFFER_HPP_VERSION "1.0.0"
#include <iostream>
#include <sstream>
#include <vector>
#include <ansi/ansi.hpp> // https://github.com/josepuga/ansi

namespace consolebuffer {
    typedef struct size_t {
        int w {0};
        int h {0};
    } size_t;

    typedef struct cell_t {
        uint32_t fg;
        uint32_t bg;
        char chr;
    } cell_t;
}

class ConsoleBuffer 
{
    public:
    ConsoleBuffer() {};

    ConsoleBuffer(const int width, const int heigth)
    {
        SetSize(width, heigth);
    }

    void SetSize(const int width, const int heigth)
    {
        m_size.w = width;
        m_size.h = heigth;
        m_buffer.clear();
        m_buffer.resize(m_size.w * m_size.h);
    }
    
    std::vector<consolebuffer::cell_t>& GetBuffer() { return m_buffer;}
    consolebuffer::size_t GetSize() const {return m_size;}
    int GetBufferSize() const {return m_size.w * m_size.h;}
    void SetPos(const int x, const int y) {m_pos.x = x; m_pos.y = y;}
    void SetPos(const ansi::pos_t& pos) {m_pos = pos;}
    void SetUseRatio21(const bool r) {m_useRatio = r;}
    void SetUseColors(const bool c) {m_useColors = c;}

    inline consolebuffer::cell_t& At(const int index) {return m_buffer[index];}
    inline consolebuffer::cell_t& At(const int x, const int y) {return m_buffer[x + m_size.w * y];}
    inline consolebuffer::cell_t& At(const ansi::pos_t pos) {return m_buffer[pos.x + m_size.w * pos.y];}
    
    void FillFg(const uint32_t fg)
    {
        for (auto& cell : m_buffer) {
            cell.fg = fg;
        }
    }   

    void FillBg(const uint32_t bg)
    {
        for (auto& cell : m_buffer) {
            cell.bg = bg;
        }
    }   

    void FillChr(char c)
        {
            for (auto& cell : m_buffer) {
                cell.chr = c;
            }
        }

    //The core!
    void Show()
    {    
        std::stringstream ss;
        uint32_t fg = m_buffer[0].fg;
        uint32_t bg = m_buffer[0].bg;
        //To make sure first time does not matches.
        fg++;
        bg++;

        auto pos = m_pos;
        for (auto idx =0, y = 0; y < m_size.h; y++) {
            ss << ansi::goto_xy(pos);
            for (auto x = 0; x < m_size.w; x++) {
                if (m_useColors) {
                    if (fg != m_buffer[idx].fg) {
                        fg = m_buffer[idx].fg;
                        ss << ansi::fg_rgb(fg);
                    }
                    if (bg != m_buffer[idx].bg) {
                        bg = m_buffer[idx].bg;
                        ss << ansi::bg_rgb(bg);
                    }
                }
                ss << m_buffer[idx].chr;
                if (m_useRatio) {
                    ss << m_buffer[idx].chr;
                }
                idx++;
            }
            pos.y++;
        }
        //Disables automatic flush
        //TODO: Save and restore old state.
        std::cout.unsetf(std::ios_base::unitbuf);
        std::cout << ss.str() << std::flush;
        //Enables automatic flush
        std::cout.setf(std::ios::unitbuf);
    }

    private:
    consolebuffer::size_t m_size;
    ansi::pos_t m_pos;
    bool m_useRatio {false};
    bool m_useColors {true};
    std::vector<consolebuffer::cell_t> m_buffer;


};
#endif // CONSOLEBUFFER_HPP