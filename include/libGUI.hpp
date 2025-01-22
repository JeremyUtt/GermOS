#pragma once
#include <fonts.hpp>
#include <string.hpp>
#include <utils.hpp>

#define putPixelM_new(x, y, color)                                                                \
    *((unsigned char*)GuiTextRenderer::screenMemory + GuiTextRenderer::screenWidth * (y) + (x)) = \
        color;

const int tabSize = 4;

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
void putRect(int x, int y, int width, int height, uint8_t color);
void putLine(int x, int y, int length, bool vertical, uint8_t color);
void putLine(int x, int y, int length, bool vertical, Color color);
void putRect(int x, int y, int width, int height, Color color);

// Text Mode functions

// Note: Polymorphism doesn't work yet in this environment
class Renderer {
  protected:
    Color color;
    Color bgColor;
    uint16_t boxStartX;
    uint16_t boxStartY;
    uint16_t boxWidth;
    uint16_t boxHeight;
    uint16_t cursorX;  // The cursor x position in either characters or pixels
    uint16_t cursorY;  // The cursor y position in either characters or pixels

  public:
    static const int screenMemory;
    static const int screenWidth;
    static const int screenHeight;
    virtual int getScreenMemory() = 0;
    virtual int getScreenWidth() = 0;
    virtual int getScreenHeight() = 0;

    Renderer();
    Renderer(int boxStartX, int boxStartY, int boxWidth, int boxHeight);
    ~Renderer();
    void setDrawColor(Color color);
    void setBackgroundColor(Color color);

    char* saveState();
    void restoreState(char* state);

    virtual void clearBox() = 0;
    virtual void setTextFont(PSF_font* font) = 0;
    virtual pair<int, int> putString(string& str, int x, int y) = 0;
    virtual pair<int, int> putString(string&& str, int x, int y) = 0;
    virtual void print(string& str) = 0;
    virtual void print(string&& str) = 0;
    virtual void printChar(char chr) = 0;
    virtual void backspace() = 0;
};

class TuiTextRenderer : public Renderer {
  private:
    void updateCursor();

  public:
    const static int screenMemory = 0xb8000;
    const static int screenWidth = 80;
    const static int screenHeight = 25;
    int getScreenMemory();
    int getScreenWidth();
    int getScreenHeight();

    using Renderer::Renderer;
    void clearBox();
    pair<int, int> putChar(int chr, int x, int y);
    pair<int, int> putString(string& str, int x, int y);
    pair<int, int> putString(string&& str, int x, int y);

    void print(string& str);
    void print(string&& str);
    void printChar(char chr);
    void backspace();
    void setTextFont(PSF_font* font);
};

class GuiTextRenderer : public Renderer {
  private:
    PSF_font* currentFont;

  public:
    const static int screenMemory = 0xA0000;
    const static int screenWidth = 320;
    const static int screenHeight = 200;
    int getScreenMemory();
    int getScreenWidth();
    int getScreenHeight();
    
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

    void backspace();
};

void newGuiTest();
void newTuiTest();

void putRect(int x, int y, int width, int height, Color color);
void putLine(int x, int y, int length, bool vertical, Color color);
void ClearScreenGUI();
