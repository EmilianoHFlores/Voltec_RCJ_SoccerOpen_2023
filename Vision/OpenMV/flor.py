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
sensor.skip_frames(time = 3000)

# Disable automatic settings
# Set rgb values of white balance (pump blue up)
sensor.set_auto_whitebal(False, (0,2,128))
sensor.set_auto_gain(False, gain_db = 0)

# Set exposure, gain, brightness, contrast, and white balance values
sensor.set_auto_exposure(False, exposure_us=1450) # adjust as needed
sensor.set_brightness(1) # adjust as needed
sensor.set_contrast(3) # adjust as needed
sensor.set_saturation(3) # adjust as needed

clock = time.clock()
uart = UART(3, 115200, timeout_char=1000)                         # init with given baudrate
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1) # init with given parameters

threshold_orange = [(44, 65, 18, 127, -128, 127)]
threshold_blue = [(20, 26, -30, -12, -6, 9)]
threshold_yellow = [(43, 70, -48, -19, 43, 91)]
SHOW_IMAGE_DECORATIONS = True
TURN_ON_LEDS = True

ox = 0;
yx = 0;
bx = 0;

oy = 0;

yIntercept = False
bIntercept = False

def distance(y):
    if y == -1:    return -1;
    elif y >= 197:    return 0
    elif y >= 172:    return 5
    elif y >= 143:    return 10
    elif y >= 121:    return 15
    elif y >= 110:    return 20
    elif y >= 100:    return 25
    elif y >= 94:    return 30
    elif y >= 89:    return 35
    elif y >= 84:    return 40
    elif y >= 81:    return 45
    elif y >= 78:    return 50
    elif y >= 75:    return 55
    elif y >= 73:    return 60
    elif y >= 71:    return 65
    elif y >= 69:    return 70
    elif y >= 68:    return 75
    elif y >= 67:    return 80
    elif y >= 66:    return 85
    elif y >= 65:    return 90
    elif y >= 64:    return 95
    elif y >= 63:    return 100
    elif y >= 62:    return 105
    else:    return -1


while(True):
    clock.tick()
    img = sensor.snapshot().rotation_corr(x_rotation = 180)

    oBlobs = img.find_blobs(threshold_orange, pixels_threshold=3, area_threshold=1, merge=False)
    if oBlobs:
        oBlob = max(oBlobs, key=lambda b: b.area())
        if (oBlob.roundness() > .1):
            if (SHOW_IMAGE_DECORATIONS):    img.draw_circle(oBlob.enclosing_circle(), color=(img.get_pixel(oBlob.cx(), oBlob.cy())), thickness=3)
            ox = oBlob.cx()
            oy = oBlob.cy()
        print(oBlob.roundness())
    else:
        ox = -1;
        oy = -1;

    yBlobs = img.find_blobs(threshold_yellow, pixels_threshold=130, area_threshold=100, merge=True)
    if yBlobs:
        yBlob = max(yBlobs, key=lambda b: b.area())
        if (SHOW_IMAGE_DECORATIONS):    img.draw_rectangle(yBlob.rect(), color=(img.get_pixel(yBlob.cx(), yBlob.cy())), thickness=3)
        if (SHOW_IMAGE_DECORATIONS):    img.draw_circle(yBlob.cx(), yBlob.cy(), 3, color=(0, 0, 0), thickness=3, fill=True)
        yx = yBlob.cx();
        yy = yBlob.cy();
        if yBlob[0] < ox and ox < (yBlob[0] + yBlob[2]):    yIntercept = True;
        else:    yIntercept = False
    else:
        yx = -1;
        yy = -1;



    bBlobs = img.find_blobs(threshold_blue, pixels_threshold=130, area_threshold=100, merge=True)
    if bBlobs:
        bBlob = max(bBlobs, key=lambda b: b.area())
        if (SHOW_IMAGE_DECORATIONS):    img.draw_rectangle(bBlob.rect(), color=(img.get_pixel(bBlob.cx(), bBlob.cy())), thickness=3)
        if (SHOW_IMAGE_DECORATIONS):    img.draw_circle(bBlob.cx(), bBlob.cy(), 3, color=(0, 0, 0), thickness=3, fill=True)
        bx = bBlob.cx();
        by = bBlob.cy();
        if bBlob[0] < ox and ox < (bBlob[0] + bBlob[2]):    bIntercept = True;
        else:    bIntercept = False;
    else:
        bx = -1;
        by = -1;


    turnOffLeds();
    if (TURN_ON_LEDS):
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
        if read == bytes("i", "ascii"): # Yellow intercept
            uart.write(f"{int(yIntercept)}")
        if read == bytes("I", "ascii"): # Blue intercept
            uart.write(f"{int(bIntercept)}");
        if read == bytes("O", "ascii"):
            uart.write(f"{distance(oy)}")
        time.sleep_ms(1)


"""
        read = uart.readline();
        query = [];
        resultString = "";
        for i in range(0, len(read), 2):
            query.append(read[i:i+2])
        for i in range(0, len(query)):
            if query[i] == bytes("ox", "ascii"):
                resultString += f"#ox:{ox}";
            elif query[i] == bytes("oy", "ascii"):
                resultString += f"#oy:{oy}";
            elif query[i] == bytes("bx", "ascii"):
                resultString += f"#bx:{bx}";
            elif query[i] == bytes("by", "ascii"):
                resultString += f"#by:{by}";
            elif query[i] == bytes("yx", "ascii"):
                resultString += f"#yx:{yx}";
            elif query[i] == bytes("yy", "ascii"):
                resultString += f"#yy:{yy}";
        uart.write(f"{resultString}\n");
"""
