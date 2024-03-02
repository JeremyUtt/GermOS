#include <converts.hpp>
#include <libGUI.hpp>
#include <libKeyboard.hpp>
#include <libTimer.hpp>
#include <PROGRAM_PONG.hpp>

using namespace GuiRenderer;

namespace PONG {

int leftPos, rightPos = screenHeight / 2;
int ballX = screenWidth / 2;
int ballY = screenHeight / 2;
int ballDX = 1;
int ballDY = 1;
int timer = 0;
int leftScore, rightScore = 0;
bool lastScored = false;
#define PADDLE_SPEED 10
#define BALL_SPEED_RATIO 5
#define PADDLE_WIDTH 15
#define PADDLE_X 15
#define BALL_SIZE 4

bool loop();

void pong() {
    while (true) {
        sleep(5);

        if (loop()) {
            ClearScreen();
            println("Game Over");
            println("Press Escape to exit");
            println("Press any other button to play again");
            leftPos = screenHeight / 2;
            rightPos = screenHeight / 2;
            ballX = screenWidth / 2;
            ballY = screenHeight / 2;
            timer = 0;
            leftScore = 0;
            rightScore = 0;

            sleep(500);
            waitForKeyboard();

            while (keyboardBuffer::getKeyBufferIndex() > 0) {
                char c = keyboardBuffer::popKeyBuffer();
                if (c == Escape) {
                    println("Program Exiting");
                    return;
                }
            }
        }
    }
}

bool loop() {
    if (keyboardBuffer::getKeyBufferIndex() > 0) {
        char character = keyboardBuffer::popKeyBuffer();

        switch (character) {
            case 'w':
                leftPos -= PADDLE_SPEED;
                if (leftPos < 0) {
                    leftPos = 0;
                }
                break;
            case 's':
                leftPos += PADDLE_SPEED;
                if (leftPos + PADDLE_WIDTH > screenHeight) {
                    leftPos = screenHeight - PADDLE_WIDTH;
                }
                break;
            case 'i':
                rightPos -= PADDLE_SPEED;
                if (rightPos < 0) {
                    rightPos = 0;
                }
                break;
            case 'k':
                rightPos += PADDLE_SPEED;
                if (rightPos + PADDLE_WIDTH > screenHeight) {
                    rightPos = screenHeight - PADDLE_WIDTH;
                }
                break;
            default:
                break;
        }
    }

    if (ballY <= 0 || ballY + BALL_SIZE >= screenHeight) {
        ballDY *= -1;
    }

#define BALL_L_X_Intersect (ballX <= PADDLE_WIDTH && ballX + BALL_SIZE >= PADDLE_WIDTH)
#define BALL_R_X_INTERSECT \
    (ballX <= screenWidth - PADDLE_WIDTH && ballX + BALL_SIZE >= screenWidth - PADDLE_WIDTH)
#define BALL_L_Y_INTERSECT (ballY <= leftPos + PADDLE_WIDTH && ballY + BALL_SIZE >= leftPos)
#define BALL_R_Y_INTERSECT (ballY <= rightPos + PADDLE_WIDTH && ballY + BALL_SIZE >= rightPos)

    if ((BALL_L_X_Intersect && BALL_L_Y_INTERSECT) || (BALL_R_X_INTERSECT && BALL_R_Y_INTERSECT)) {
        ballDX *= -1;

        if (lastScored) {
            leftScore++;
            lastScored = false;
        } else {
            rightScore++;
            lastScored = true;
        }
    }

    if (ballX <= 0 || ballX >= screenWidth) {
        return true;
    }

    timer++;
    if (timer % BALL_SPEED_RATIO == 0) {
        ballY += ballDY;
    }
    ballX += ballDX;

    ClearScreen();

    putString("Score:", 5, 0);
    putString(intToStr(leftScore, 10), 45, 0);

    putString("Score:", screenWidth - PADDLE_WIDTH - 40, 0);
    putString(intToStr(rightScore, 10), screenWidth - PADDLE_WIDTH, 0);

    putRect(ballX, ballY, 4, 4, 4);
    putLine(PADDLE_WIDTH, leftPos, PADDLE_WIDTH, true, 5);
    putLine(screenWidth - PADDLE_WIDTH, rightPos, PADDLE_WIDTH, true, 5);

    return false;
}

}  // namespace PONG