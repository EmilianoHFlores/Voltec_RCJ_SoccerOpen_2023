# Untitled - By: ikerc - lun. abr. 17 2023

# This piece of code is used when robot's team is YELLOW, Blue goal will be undetectable under this code
# MADE FOR YELLOW GOAL

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

i = 0;
while (i <= 10):
    i += 1;
    turnOnLeds();
    time.sleep(.1);
    turnOffLeds();
    time.sleep(.1);

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(True)
sensor.skip_frames(time = 2000)

# Disable automatic settings
# Set rgb values of white balance (pump blue up)
sensor.set_auto_whitebal(False, (0,2,128))
sensor.set_auto_gain(False, gain_db = 0)

# Set exposure, gain, brightness, contrast, and white balance values
sensor.set_auto_exposure(False, exposure_us=4000) # adjust as needed
sensor.set_brightness(0) # adjust as needed
sensor.set_contrast(0) # adjust as needed
sensor.set_saturation(3) # adjust as needed

clock = time.clock()
uart = UART(3, 115200, timeout_char=1000)                         # init with given baudrate
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1) # init with given parameters

threshold_orange = [(44, 82, 3, 78, 34, 88)]
threshold_blue = [(17, 33, -28, 0, -32, -3)]
threshold_yellow = [(54, 74, -46, -7, 54, 77)]
SHOW_IMAGE_DECORATIONS = True

ox = 0;
yx = 0;
bx = 0;

oy = 0;

def distance(y):
    if y == -1:    return -1;
    elif y >= 181:    return 0
    elif y >= 152:    return 5
    elif y >= 124:    return 10
    elif y >= 105:    return 15
    elif y >= 91:    return 20
    elif y >= 82:    return 25
    elif y >= 75:    return 30
    elif y >= 70:    return 35
    elif y >= 66:    return 40
    elif y >= 62:    return 45
    elif y >= 60:    return 50
    elif y >= 57:    return 55
    elif y >= 55:    return 60
    elif y >= 54:    return 65
    elif y >= 52:    return 70
    elif y >= 51:    return 75
    elif y >= 50:    return 80
    elif y >= 49:    return 85
    elif y >= 48:    return 90
    elif y >= 47:    return 95
    elif y >= 46:    return 100
    else:    return -1


while(True):
    clock.tick()
    img = sensor.snapshot().rotation_corr(x_rotation = 180)

    oBlobs = img.find_blobs(threshold_orange, pixels_threshold=5, area_threshold=2, merge=False)
    if oBlobs:
        oBlob = max(oBlobs, key=lambda b: b.area())
        if (oBlob.roundness() > .1):
            if (SHOW_IMAGE_DECORATIONS):    img.draw_circle(oBlob.enclosing_circle(), color=(img.get_pixel(oBlob.cx(), oBlob.cy())), thickness=3)
            ox = oBlob.cx()
            oy = oBlob.cy()
            print(oBlob.roundness());
    else:
        ox = -1;
        oy = -1;

    yBlobs = img.find_blobs(threshold_yellow, pixels_threshold=130, area_threshold=100, merge=True)
    if yBlobs:
        yBlob = max(yBlobs, key=lambda b: b.area())
        if (SHOW_IMAGE_DECORATIONS):    img.draw_rectangle(yBlob.rect(), color=(img.get_pixel(yBlob.cx(), yBlob.cy())), thickness=3)
        yx = yBlob.cx()
        yy = yBlob.cy();
    else:
        yx = -1;
        yy = -1;



    bBlobs = img.find_blobs(threshold_blue, pixels_threshold=130, area_threshold=100, merge=True)
    if bBlobs:
        bBlob = max(bBlobs, key=lambda b: b.area())
        if (SHOW_IMAGE_DECORATIONS):    img.draw_rectangle(bBlob.rect(), color=(img.get_pixel(bBlob.cx(), bBlob.cy())), thickness=3)
        bx = bBlob.cx()
        by = bBlob.cy();
    else:
        bx = -1;
        by = -1;


    turnOffLeds();
    if (ox >= 0):    red_led.on(); green_led.off(); blue_led.off();
    if (yx >= 0):    red_led.on(); green_led.on(); blue_led.off();
    if (ox >= 0 and yx >= 0):    blue_led.on(); green_led.on(); red_led.on();
    if (bx >= 0):    blue_led.on(); green_led.off(); red_led.off();
    if (ox >= 0 and bx >= 0):    blue_led.on(); red_led.on(); green_led.off();
    if (ox + yx + bx == -3):    green_led.on(); red_led.off(); blue_led.off();
    if uart.any():
        read = uart.read(1)
        if read == bytes("o", "ascii"):
            uart.write(f"{ox}")
        if read == bytes("b", "ascii"):
            uart.write(f"{bx}")
        if read == bytes("y", "ascii"):
            uart.write(f"{yx}")
        if read == bytes("O", "ascii"):
            uart.write(f"{distance(oy)}")
