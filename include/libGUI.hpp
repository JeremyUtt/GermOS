#pragma once
#include <fonts.hpp>
#include <string.hpp>
#include <utils.hpp>


const int tabSize = 4;

// For graphics mode
const int screenWidthPx = 320;
const int screenHeightPx = 200;

// For text mode
const int screenWidthChar = 80;
const int screenHeightChar = 25;

enum Color {
    BLACK = 0x00,
    BLUE = 0x01,
    GREEN = 0x02,
    CYAN = 0x03,
    RED = 0x04,
    MAGENTA = 0x05,
    BROWN = 0x06,
    LIGHT_GRAY = 0x07,
    DARK_GRAY = 0x08,
    LIGHT_BLUE = 0x09,
    LIGHT_GREEN = 0x0A,
    LIGHT_CYAN = 0x0B,
    LIGHT_RED = 0x0C,
    LIGHT_MAGENTA = 0x0D,
    YELLOW = 0x0E,
    WHITE = 0x0F
};

// Graphics Mode functions
void putPixel(int pos_x, int pos_y, Color color);
void putRect(int x, int y, int width, int height, Color color);
void putLine(int x, int y, int length, bool vertical, Color color);

// Text Mode functions

// Note: Polymorphism doesn't work yet in this environment
class Renderer {
  protected:
    Color color;
    uint16_t boxStartX;
    uint16_t boxStartY;
    uint16_t boxWidth;
    uint16_t boxHeight;
    uint16_t cursorX;  // The cursor x position in either characters or pixels
    uint16_t cursorY;  // The cursor y position in either characters or pixels

  public:
    Renderer();
    Renderer(int boxStartX, int boxStartY, int boxWidth, int boxHeight);
    ~Renderer();
    void setDrawColor(Color color);
};

class TuiTextRenderer : public Renderer {
  public:
    using Renderer::Renderer;
    void clearBox();
    void putChar(int chr, int x, int y);
    pair<int, int> putString(string str, int x, int y);
    void print(string str);
    void printChar(char chr);

    /**
     * @brief moves the cursor to the position specified by cursorX and cursorY.
     *
     */
    void updateCursor();
};

class GuiTextRenderer : public Renderer {
  private:
    PSF_font* currentFont;

  public:
    using Renderer::Renderer;

    /**
     * @brief Set the Text Font object
     *
     * @param font A pointer to the font to use
     */
    void setTextFont(PSF_font* font);

    /**
     * @brief Clears the box with Black.
     */
    void clearBox();

    /**
     * @brief puts a character on the screen at the specified position.
     * Does NOT update cursor position (See printChar()).
     * Does NOT handle newline characters.
     * @param chr The character to be rendered.
     * @param x The x position in pixels, relative to the box.
     * @param y The y position in pixels, relative to the box.
     * @return pair<int, int> The x and y coordinates after the last character rendered.
     */
    pair<int, int> putChar(int chr, int x, int y);

    /**
     * @brief Renders a string on the GUI at the specified position.
     * Does NOT wrap lines that overflow box.
     * Does handle newline characters.
     * Does NOT update cursor position.
     *
     * @param str The string to be rendered.
     * @param x The x-coordinate (in pixels relative to the box) of the starting position.
     * @param y The y-coordinate (in pixels relative to the box) of the starting position.
     * @return pair<int, int> The x and y coordinates of the last character rendered.
     */
    pair<int, int> putString(string& str, int x, int y);
    pair<int, int> putString(string&& str, int x, int y);

    /**
     * @brief Put a string on the screen at the current cursor position.
     *
     * @param str String to be printed.
     */
    void print(string& str);
    void print(string&& str);

    /**
     * @brief Put a character on the screen at the current cursor position.
     * Does update cursor position.
     * Does NOT handle newline characters.
     *
     * @param chr Character to be printed.
     */
    void printChar(char chr);
};

void newGuiTest();
void newTuiTest();

void putRect(int x, int y, int width, int height, Color color);
void putLine(int x, int y, int length, bool vertical, Color color);
void ClearScreen();

// Polymorphism doesnt work yet in this environment
#ifdef TEXT_MODE
const int screenMemory = 0xb8000;
typedef TuiTextRenderer GenericRenderer;
#else
const int screenMemory = 0xA0000;
typedef GuiTextRenderer GenericRenderer;
#endif