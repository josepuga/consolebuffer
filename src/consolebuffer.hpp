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
#include <string>
#include <cstring>
#include <ansi/ansi.hpp> // https://github.com/josepuga/ansi

namespace consolebuffer {
    std::string utf8_substr(const std::string& str, unsigned int start, unsigned int leng);

    struct Cell {
        public:
        inline uint32_t Fg() { return fg;}
        inline uint32_t Bg() { return bg;}
        inline const char* Chr() { return chr;}
        inline void Fg(const uint32_t c ) {fg = c;}
        inline void Bg(const uint32_t c ) {bg = c;}
        inline void Chr(char c) {chr[0] = c; chr[1] = '\0';}
        void Chr(std::string s) // UTF8 not as easy
        {
            auto sc = utf8_substr(s, 0, 1);
            strncpy(chr, sc.c_str(), 4);
        }

        private:
        uint32_t fg;
        uint32_t bg;
        char chr[5];
        std::string utf8_substr(const std::string& str, unsigned int start, unsigned int leng)
        {
            if (leng==0) { return ""; }
            unsigned int c, i, ix, q;
            auto min=std::string::npos; 
            auto max=std::string::npos;
            for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
            {
                if (q==start){ min=i; }
                if (q<=start+leng || leng==std::string::npos){ max=i; }

                c = (unsigned char) str[i];
                if      (
                        //c>=0   &&
                        c<=127) i+=0;
                else if ((c & 0xE0) == 0xC0) i+=1;
                else if ((c & 0xF0) == 0xE0) i+=2;
                else if ((c & 0xF8) == 0xF0) i+=3;
                //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
                //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
                else return "";//invalid utf8
            }
            if (q<=start+leng || leng==std::string::npos){ max=i; }
            if (min==std::string::npos || max==std::string::npos) { return ""; }
            return str.substr(min,max);
        }        
    };

    // thanks to http://www.zedwood.com/article/cpp-utf-8-mb_substr-function


} // namespace


class ConsoleBuffer 
{
    public:

    ConsoleBuffer() {};

    ConsoleBuffer(const int width, const int height)
    {
        SetSize(width, height);
    }

    void SetSize(const int width, const int height)
    {
        m_width = width;
        m_height = height;
        m_buffer.clear();
        m_buffer.resize(m_width * m_height);
    }
    
    std::vector<consolebuffer::Cell>& GetBuffer() { return m_buffer;}
    int GetBufferWidth() const {return m_width;}
    int GetBufferHeight() const {return m_height;}
    int GetBufferSize() const {return m_width * m_height;}
    void SetPos(const int x, const int y) {m_pos.x = x; m_pos.y = y;}
    void SetPos(const ansi::pos_t& pos) {m_pos = pos;}
    void SetUseRatio21(const bool r) {m_useRatio = r;}
    void SetUseColors(const bool c) {m_useColors = c;}

    // Lo más facil sería pasar la estructura, pero debido a que tengo que tratar chr como char o
    // como utf8, no hay una forma fácil de hacerlo si no es encapsulando con métodos.
    /*
    inline uint32_t GetFgAt(const int index) {return m_buffer[index].fg;}
    inline uint32_t GetFgAt(const int x, const int y) {return m_buffer[x + m_width * y].fg;}
    inline uint32_t GetBgAt(const int index) {return m_buffer[index].bg;}
    inline uint32_t GetBgAt(const int x, const int y) {return m_buffer[x + m_width * y].bg;}
    inline const char* GetChrAt(const int index) {return m_buffer[index].chr;}
    inline const char* GetChrAt(const int x, const int y) {return m_buffer[x + m_width * y].chr;}

    inline void SetFgAt(const int index, uint32_t fg) {m_buffer[index].fg = fg;}
    inline void SetFgAt(const int index, uint32_t fg) {m_buffer[index].fg = fg;}
    */

    inline consolebuffer::Cell& At(const int index) {return m_buffer[index];}
    inline consolebuffer::Cell& At(const int x, const int y) {return m_buffer[x + m_width * y];}
    inline consolebuffer::Cell& At(const ansi::pos_t pos) {return m_buffer[pos.x + m_width * pos.y];}
    
    void FillFg(const uint32_t fg)
    {
        for (auto& cell : m_buffer) {
            cell.Fg(fg);
        }
    }   

    void FillBg(const uint32_t bg)
    {
        for (auto& cell : m_buffer) {
            cell.Bg(bg);
        }
    }   

    void FillChr(std::string s) 
    {
        for (auto& cell : m_buffer) {
            cell.Chr(s);
        }
    }

    void FillChr(char c)  
    {
        for (auto& cell : m_buffer) {
            cell.Chr(c);
        }
    }

    //The core!
    void Show()
    {    
        std::stringstream ss;
        uint32_t fg = m_buffer[0].Fg();
        uint32_t bg = m_buffer[0].Bg();
        //To make sure first time does not matches.
        fg++;
        bg++;

        auto pos = m_pos;
        for (auto idx =0, y = 0; y < m_height; y++) {
            ss << ansi::goto_xy(pos);
            for (auto x = 0; x < m_width; x++) {
                if (m_useColors) {
                    if (fg != m_buffer[idx].Fg()) {
                        fg = m_buffer[idx].Fg();
                        ss << ansi::fg_rgb(fg);
                    }
                    if (bg != m_buffer[idx].Bg()) {
                        bg = m_buffer[idx].Bg();
                        ss << ansi::bg_rgb(bg);
                    }
                }
                ss << m_buffer[idx].Chr();
                if (m_useRatio) {
                    ss << m_buffer[idx].Chr();
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
    int m_height;
    int m_width;
    ansi::pos_t m_pos;
    bool m_useRatio {false};
    bool m_useColors {true};
    std::vector<consolebuffer::Cell> m_buffer;

};
#endif // CONSOLEBUFFER_HPP
