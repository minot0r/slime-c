#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <gpiod.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

#define UINPUT_DEVICE "/dev/uinput"
#define GPIO_CHIP "/dev/gpiochip1"

struct timespec timeout;
unsigned int pins_offset[] = {82, 83, 84}; // Add more button pins here
size_t num_lines = sizeof(pins_offset) / sizeof(pins_offset[0]);

// Callback function for button press events
static int button_event_callback(int event_type, unsigned int line_offset,
			  const struct timespec *timestamp, void *data) {
    int uinput_fd = *((int *)data);
    int key = -1;

    // Map GPIO pin to corresponding key code
    switch (line_offset)
    {
    case 82:
        key = KEY_Q;
        break;
    case 83:
        key = KEY_D;
        break;
    case 84:
        key = KEY_Z;
        break;
    case 37:
        key = KEY_LEFT;
        break;
    case 38:
        key = KEY_UP;
        break;
    case 40:
        key = KEY_RIGHT;
        break;
        // Add more cases for other pins and keys
    }

    if (key != -1)
    {
        struct input_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.type = EV_KEY;
        ev.code = key;
        ev.value = (event_type == GPIOD_CTXLESS_EVENT_CB_FALLING_EDGE) ? 1 : 0; // Press or release

        write(uinput_fd, &ev, sizeof(ev));
        memset(&ev, 0, sizeof(ev));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        write(uinput_fd, &ev, sizeof(ev));
    }

    return GPIOD_CTXLESS_EVENT_CB_RET_OK;
}

void *rising_edge_loop(void *data)
{
    int *uinput_fd = (int *)data;
    return (void *)(intptr_t)gpiod_ctxless_event_monitor_multiple_ext(
        GPIO_CHIP, GPIOD_CTXLESS_EVENT_RISING_EDGE,
        pins_offset, num_lines,
        false,
        "rising_edge_loop",
        &timeout,
        NULL,
        button_event_callback,
        uinput_fd,
        0);
}

void *falling_edge_loop(void *data)
{
    int *uinput_fd = (int *)data;
    return (void *)(intptr_t)gpiod_ctxless_event_monitor_multiple_ext(
        GPIO_CHIP, GPIOD_CTXLESS_EVENT_FALLING_EDGE,
        pins_offset, num_lines,
        false,
        "falling_edge_loop",
        &timeout,
        NULL,
        button_event_callback,
        uinput_fd,
        0);
}

int main(void)
{
    // Set up uinput device
    int uinput_fd = open(UINPUT_DEVICE, O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0)
    {
        perror("Error opening uinput device");
        return 1;
    }

    timeout.tv_sec = 1; // 1 second timeout
    timeout.tv_nsec = 0;

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    strcpy(usetup.name, "gpio-keyboard");
    usetup.id.bustype = BUS_USB;

    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_KEYBIT, KEY_Q);
    ioctl(uinput_fd, UI_SET_KEYBIT, KEY_D);
    ioctl(uinput_fd, UI_SET_KEYBIT, KEY_Z);
    ioctl(uinput_fd, UI_SET_KEYBIT, KEY_LEFT);
    ioctl(uinput_fd, UI_SET_KEYBIT, KEY_UP);
    ioctl(uinput_fd, UI_SET_KEYBIT, KEY_RIGHT);

    ioctl(uinput_fd, UI_DEV_SETUP, &usetup);
    ioctl(uinput_fd, UI_DEV_CREATE);

    pthread_t rising_edge_thread;
    pthread_t falling_edge_thread;

    pthread_create(&rising_edge_thread, NULL, rising_edge_loop, &uinput_fd);
    pthread_create(&falling_edge_thread, NULL, falling_edge_loop, &uinput_fd);

    // Wait for the threads to finish
    void *rising_edge_ret;
    void *falling_edge_ret;
    pthread_join(rising_edge_thread, &rising_edge_ret);
    pthread_join(falling_edge_thread, &falling_edge_ret);

    int ret1 = (int)(intptr_t)rising_edge_ret;
    int ret2 = (int)(intptr_t)falling_edge_ret;

    if (ret1 < 0 || ret2 < 0)
    {
        perror("Error in gpiod_ctxless_event_monitor_multiple_ext()");
        close(uinput_fd);
        return 1;
    }

    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);

    return 0;
}