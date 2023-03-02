import subprocess
import uinput

def read_gpio_pin(pin_num):
    return subprocess.Popen("lgpio get 7j1" + pin_num, shell=True, stdout=subprocess.PIPE).stdout.read()

def is_down(pin_num):
    return read_gpio_pin(str(pin_num)) == "1"


player1_left_pin = 8
player1_right_pin = 7
player1_up_pin = 10
player2_left_pin = 38
player2_right_pin = 40
player2_up_pin = 37
pause_pin = 15

with uinput.Device([uinput.KEY_Z, uinput.KEY_Q,
                    uinput.KEY_D, uinput.KEY_SPACE, uinput.KEY_UP, uinput.KEY_LEFT, uinput.KEY_RIGHT]) as device:

    while True:
        if is_down(player1_left_pin):
            device.emit_click(uinput.KEY_Q)
        if is_down(player1_right_pin):
            device.emit_click(uinput.KEY_D)
        if is_down(player1_up_pin):
            device.emit_click(uinput.KEY_Z)
        if is_down(player2_left_pin):
            device.emit_click(uinput.KEY_LEFT)
        if is_down(player2_right_pin):
            device.emit_click(uinput.KEY_RIGHT)
        if is_down(player2_up_pin):
            device.emit_click(uinput.KEY_UP)
        if is_down(pause_pin):
            device.emit_click(uinput.KEY_SPACE)