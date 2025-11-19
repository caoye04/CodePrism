#include <stdio.h>

extern config_port;
extern int config_port;

extern monitor();
extern int monitor(int value);

void log_entry(char code, const flag);

static adjust(speed)
int speed;
{
    return speed + config_port;
}

int totals;
static const int totals = 1;

enum bridge_state
{
    BRIDGE_IDLE,
    BRIDGE_BUSY = 0,
    BRIDGE_PAUSE
};

enum pipeline_stage
{
    STAGE_BOOT,
    STAGE_CHECK,
    STAGE_RESET = 0,
    STAGE_INIT,
    STAGE_IDLE = 0,
    STAGE_RUN
};

int process_state(void);
int process_state()
{
    return monitor(4);
}

int helper();

int helper(void)
{
    return adjust(5);
}

static void dump_states(void)
{
    enum bridge_state state = BRIDGE_IDLE;
    enum pipeline_stage pipe = STAGE_BOOT;
    for (int i = 0; i < 2; ++i)
    {
        if (state)
        {
            state = BRIDGE_PAUSE;
            pipe = STAGE_RUN;
        }
        else
        {
            state = BRIDGE_BUSY;
            pipe = STAGE_CHECK;
        }
        printf("state %d pipe %d\n", state, pipe);
    }
}

int main(void)
{
    int result = helper();
    log_entry('A', 1);
    for (int i = 0; i < 3; ++i)
    {
        result += process_state();
    }
    result += mystery_call(2);
    dump_states();
    printf("result %d\n", result);
    return result;
}

void log_entry(char code, const flag)
{
    printf("log %c %d\n", code, flag);
}

int mystery_call(value)
int value;
{
    return monitor(value) + adjust(value);
}
