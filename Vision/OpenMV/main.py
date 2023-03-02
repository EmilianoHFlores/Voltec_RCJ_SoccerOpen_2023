# Untitled - By: ikerc - vie. ene. 13 2023

import sensor, image, time
from pyb import UART, LED
from struct import *
from math import *

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

EXPOSURE_TIME_SCALE = .4
GAIN_SCALE = 10
SHOW_IMAGE_DECORATIONS = True

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_auto_exposure(False)
sensor.set_saturation(3)
sensor.set_hmirror(True)

clock = time.clock()
uart = UART(3, 115200, timeout_char=1000)                         # init with given baudrate
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1) # init with given parameters
current_exposure_time_in_microseconds = sensor.get_exposure_us()
current_gain_in_decibels = sensor.get_gain_db()
sensor.set_auto_exposure(False, \
    exposure_us = int(current_exposure_time_in_microseconds * EXPOSURE_TIME_SCALE))
sensor.set_auto_gain(False, \
    gain_db = current_gain_in_decibels * GAIN_SCALE)
sensor.set_saturation(3)

ox = 0
oy = 0
oz = 0
bx = 0
by = 0
bz = 0
yx = 0
yy = 0
yz = 0

oBlob = None
yBlob = None
bBlob = None

Pelota = [(37, 100, -20, 86, 37, 127)]
PorteriaAzul = [(15, 60, -6, 29, -68, -12)]
PorteriaAmarilla = [(57, 79, -43, -9, 30, 127)]

while(True):
    clock.tick()
    img = sensor.snapshot().rotation_corr(x_rotation = 180)
    green_led.on();
    oBlobs = img.find_blobs(Pelota, pixels_threshold=150, area_threshold=30, merge=False)
    yBlobs = img.find_blobs(PorteriaAmarilla, pixels_threshold=50, area_threshold=30, merge=False)
    bBlobs = img.find_blobs(PorteriaAzul, pixels_threshold=300, area_threshold=30, merge=False)

    if oBlobs:
        oBlob = max(oBlobs, key=lambda b: b.area())
        if SHOW_IMAGE_DECORATIONS:    img.draw_circle(oBlob.enclosing_circle(), color = (255, 150, 20))
        ox = oBlob.cx();
        oy = oBlob.cy();
        oz = oBlob.area();
        green_led.off()
        red_led.on()
    else:
        oBlob = None
        ox = -1
        oy = -1
        oz = -1
        red_led.off()

    if yBlobs:
        yBlob = max(yBlobs, key=lambda b: b.area())
        if SHOW_IMAGE_DECORATIONS:    img.draw_rectangle(yBlob.rect(), color = (255, 255, 0) )
        yx = yBlob.cx();
        yy = yBlob.cy();
        yz = yBlob.area();
    else:
        yBlob = None
        yx = -1
        yy = -1
        yz = -1

    if bBlobs:
        bBlob = max(bBlobs, key=lambda b: b.area())
        if SHOW_IMAGE_DECORATIONS:    img.draw_rectangle(bBlob.rect(), color = (0, 0, 255))
        bx = bBlob.cx();
        by = bBlob.cy();
        bz = bBlob.area();
        green_led.off()
        blue_led.on()
    else:
        bBlob = None
        bx = -1
        by = -1
        bz = -1
        blue_led.off()

    if uart.any():
        read = uart.read(1)
        print(read)
        if read == bytes("o", "ascii"):
            uart.write(f"x:{ox},y:{oy},z:{oz},id:o")
        if read == bytes("b", "ascii"):
            uart.write(f"x:{bx},y:{by},z:{bz},id:b")
        if read == bytes("y", "ascii"):
            uart.write(f"x:{yx},y:{yy},z:{yz},id:y")

