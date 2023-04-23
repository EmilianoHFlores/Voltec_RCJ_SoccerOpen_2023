# Untitled - By: ikerc - lun. abr. 17 2023

# This piece of code is used when robot's team is YELLOW, Blue goal will be undetectable under this code

import sensor, image, time
from pyb import UART, LED

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led    = LED(4)

def turnOffLeds():
    red_led.off()
    green_led.off()
    blue_led.off()

def turnOnLeds():
    red_led.on();
    green_led.on();
    blue_led.on();


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(True)


# Disable automatic settings
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

# Set exposure, gain, brightness, contrast, and white balance values
sensor.set_auto_exposure(False, exposure_us=1200) # adjust as needed
sensor.set_gainceiling(128) # adjust as needed
sensor.set_brightness(3) # adjust as needed
sensor.set_contrast(-1) # adjust as needed
sensor.set_saturation(-2) # adjust as needed

clock = time.clock()
uart = UART(3, 115200, timeout_char=1000)                         # init with given baudrate
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1) # init with given parameters

threshold_orange = [(34, 52, 28, 58, 5, 37)]
threshold_yellow = [(31, 41, -3, 16, 7, 28)]
SHOW_IMAGE_DECORATIONS = True

ox = 0;
yx = 0;
bx = 0;

oy = 0;

def distance(y):
    if y >= 230:    return 5
    elif y >= 190:    return 10
    elif y >= 140:    return 15
    elif y >= 110:    return 20
    elif y >= 90:    return 25
    elif y >= 75:    return 30
    elif y >= 60:    return 35
    elif y >= 55:    return 40
    elif y >= 45:    return 45
    elif y >= 40:    return 50
    elif y >= 35:    return 55
    elif y >= 32:    return 60
    elif y >= 27:    return 65
    elif y >= 24:    return 70
    elif y >= 20:    return 75
    elif y >= 18:    return 80
    elif y >= 15:    return 85
    elif y >= 13:    return 90
    elif y >= 12:    return 95
    elif y >= 10:    return 100


while(True):
    clock.tick()
    img = sensor.snapshot().rotation_corr(x_rotation = 180)

    oBlobs = img.find_blobs(threshold_orange, pixels_threshold=25, area_threshold=5, merge=False)
    if oBlobs:
        oBlob = max(oBlobs, key=lambda b: b.area())
        print("oBall roundness: {}".format(oBlob.roundness()))
        if (oBlob.roundness() > .3):
            if (SHOW_IMAGE_DECORATIONS):    img.draw_circle(oBlob.enclosing_circle(), color=(img.get_pixel(oBlob.cx(), oBlob.cy())), thickness=3)
            ox = oBlob.cx()
            oy = oBlob.cy()
            green_led.off()
            blue_led.off()
            red_led.on()
    else:
        ox = -1;

    yBlobs = img.find_blobs(threshold_yellow, pixels_threshold=130, area_threshold=100, merge=True)
    if yBlobs:
        yBlob = max(yBlobs, key=lambda b: b.area())
        print("yGoal roundness: {}".format(yBlob.roundness()))
        if (SHOW_IMAGE_DECORATIONS):    img.draw_rectangle(yBlob.rect(), color=(img.get_pixel(yBlob.cx(), yBlob.cy())), thickness=3)
        yx = yBlob.cx()
        green_led.on()
        blue_led.off()
        red_led.on()

        if (ox >= 0):
            turnOnLeds();
    else:
        yx = -1;

    if (ox + yx == -2):
        turnOffLeds();
        green_led.on();

    if uart.any():
        read = uart.read(1)
        # print(read)
        if read == bytes("o", "ascii"):
            uart.write(f"{ox}")
        if read == bytes("y", "ascii"):
            uart.write(f"{yx}")
        if read == bytes("O", "ascii"):
            uart.write(f"{distance(oy)}")


