#include <libGUI.hpp>
#include <libKeyboard.hpp>
#include <libTimer.hpp>
#include <printf.hpp>
#include <PROGRAM_PONG.hpp>
#include <utils.hpp>

#define PADDLE_SPEED 10
#define BALL_SPEED_RATIO 5
#define PADDLE_WIDTH 15
#define PADDLE_X 15
#define BALL_SIZE 4

namespace PONG {

int leftPos, rightPos = screenWidthPx / 2;
int ballX = screenWidthPx / 2;
int ballY = screenWidthPx / 2;
int ballDX = 1;
int ballDY = 1;
int timer = 0;
int leftScore, rightScore = 0;
bool lastScored = false;
GuiTextRenderer* renderer = nullptr;
string* score = nullptr;

bool loop();

void main() {
    GuiTextRenderer temp(0, 0, screenWidthPx, screenWidthPx);
    renderer = &temp;
    renderer->setDrawColor(WHITE);
    renderer->setTextFont(&Uni2Terminus12x6psf);

    string tmp = "Score: ";
    score = &tmp;

    while (true) {
        sleep(5);

        if (loop()) {
            ClearScreen();
            printf("Game Over\n");
            printf("Press Escape to exit\n");
            printf("Press any other button to play again\n");
            leftPos = screenWidthPx / 2;
            rightPos = screenWidthPx / 2;
            ballX = screenWidthPx / 2;
            ballY = screenWidthPx / 2;
            timer = 0;
            leftScore = 0;
            rightScore = 0;

            sleep(500);
            KB::waitForKeyboard();

            while (KB::getKeyBufferIndex() > 0) {
                char c = KB::popKeyBuffer();
                if (c == Escape) {
                    printf("Program Exiting\n");
                    return;
                }
            }
        }
    }
}

bool loop() {
    if (KB::getKeyBufferIndex() > 0) {
        char character = KB::popKeyBuffer();

        switch (character) {
            case 'w':
                leftPos -= PADDLE_SPEED;
                if (leftPos < 0) {
                    leftPos = 0;
                }
                break;
            case 's':
                leftPos += PADDLE_SPEED;
                if (leftPos + PADDLE_WIDTH > screenHeightPx) {
                    leftPos = screenHeightPx - PADDLE_WIDTH;
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
                if (rightPos + PADDLE_WIDTH > screenHeightPx) {
                    rightPos = screenHeightPx - PADDLE_WIDTH;
                }
                break;
            default:
                break;
        }
    }

    if (ballY <= 0 || ballY + BALL_SIZE >= screenHeightPx) {
        ballDY *= -1;
    }

#define BALL_L_X_Intersect (ballX <= PADDLE_WIDTH && ballX + BALL_SIZE >= PADDLE_WIDTH)
#define BALL_R_X_INTERSECT \
    (ballX <= screenWidthPx - PADDLE_WIDTH && ballX + BALL_SIZE >= screenWidthPx - PADDLE_WIDTH)
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

    if (ballX <= 0 || ballX >= screenWidthPx) {
        return true;
    }

    timer++;
    if (timer % BALL_SPEED_RATIO == 0) {
        ballY += ballDY;
    }
    ballX += ballDX;

    ClearScreen();

    renderer->putString(*score, 5, 0);
    renderer->putString(intToStr(leftScore, 10), 45, 0);

    renderer->putString(*score, screenWidthPx - PADDLE_WIDTH - 40, 0);
    renderer->putString(intToStr(rightScore, 10), screenWidthPx - PADDLE_WIDTH, 0);

    putRect(ballX, ballY, 4, 4, RED);
    putLine(PADDLE_WIDTH, leftPos, PADDLE_WIDTH, true, MAGENTA);
    putLine(screenWidthPx - PADDLE_WIDTH, rightPos, PADDLE_WIDTH, true, MAGENTA);

    return false;
}

}  // namespace PONG