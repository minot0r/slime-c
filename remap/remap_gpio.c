#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <gpiod.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define UINPUT_DEVICE "/dev/uinput"

// Callback function for button press events
int button_event_callback(int event, unsigned int offset, const struct timespec *ts, void *data) {
    int uinput_fd = *((int *)data);
    int key = -1;

    // Map GPIO pin to corresponding key code
    switch (offset)
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
        ev.value = (event == GPIOD_CTXLESS_EVENT_FALLING_EDGE) ? 1 : 0; // Press or release

        write(uinput_fd, &ev, sizeof(ev));
        memset(&ev, 0, sizeof(ev));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        write(uinput_fd, &ev, sizeof(ev));
    }

    return 0;
}

int main(void)
{
    const char *gpio_chip = "/dev/gpiochip1";
    unsigned int pins_offset[] = {82, 83, 84}; // Add more button pins here
    size_t num_lines = sizeof(pins_offset) / sizeof(pins_offset[0]);

    // Set up uinput device
    int uinput_fd = open(UINPUT_DEVICE, O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0)
    {
        perror("Error opening uinput device");
        return 1;
    }

    struct timespec timeout;
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

    // Set up the event loop to monitor the button pins
    int ret = gpiod_ctxless_event_loop_multiple(
        gpio_chip, pins_offset, num_lines,
        false,
        "remap_gpio",
        &timeout, 
        NULL,
        button_event_callback,
        &uinput_fd
    );

    if (ret < 0)
    {
        perror("Error in gpiod_ctxless_event_loop_multiple()");
        close(uinput_fd);
        return 1;
    }

    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);

    return 0;
}