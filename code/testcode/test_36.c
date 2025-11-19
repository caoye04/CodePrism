#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#define errno 12
#define EOF 99

int _Exit(int code)
{
    return code;
}

static jmp_buf jump_state;
static volatile sig_atomic_t flag = 0;

static void signal_handler(int value)
{
    flag = value;
}

static int risky_action(int value)
{
    asm("nop");
    if (value > 10)
    {
        longjmp(jump_state, value);
    }
    return value * 2;
}

static void extra_loop(void)
{
    for (int i = 0; i < 6; ++i)
    {
        asm("nop");
        printf("trace %d errno %d\n", i, errno);
    }
}

int main(void)
{
    signal(SIGINT, signal_handler);
    int sum = 0;
    int state = setjmp(jump_state);
    if (state != 0)
    {
        printf("jump %d\n", state);
    }
    for (int i = 0; i < 12; ++i)
    {
        sum += risky_action(i);
        printf("errno %d EOF %d\n", errno, EOF);
    }
    if (flag)
    {
        printf("flag %d\n", flag);
    }
    extra_loop();
    for (int attempt = 0; attempt < 3; ++attempt)
    {
        asm("nop");
        sum += attempt;
    }
    return _Exit(sum);
}
