
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

threshold_orange = [(50, 79, 28, 86, 53, 78)]
threshold_blue = [(20, 26, -30, -12, -6, 9)]
threshold_yellow = [(47, 84, -50, -18, 48, 86)]
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
    elif y >= 189:    return 0
    elif y >= 175:    return 3
    elif y >= 161:    return 5
    elif y >= 129:    return 10
    elif y >= 108:    return 15
    elif y >= 93:    return 20
    elif y >= 83:    return 25
    elif y >= 77:    return 30
    elif y >= 71:    return 35
    elif y >= 67:    return 40
    elif y >= 64:    return 45
    elif y >= 58:    return 50
    elif y >= 56:    return 55
    elif y >= 55:    return 60
    elif y >= 53:    return 65
    elif y >= 52:    return 70
    elif y >= 51:    return 75
    elif y >= 50:    return 80
    elif y >= 49:    return 85
    elif y >= 48:    return 90
    elif y >= 47:    return 95
    elif y >= 46:    return 100
    elif y >= 45:    return 105
    else:    return -1

start_nanos = time.time_ns();
def millis():
    return (time.time_ns() - start_nanos) // 1000000

uartUpdate = 1;
frameUpdate = 1000 / 30; # fps

previousTime1 = 0;
previousTime2 = 0;


while(True):
    clock.tick()
    currentTime = millis();
    if (currentTime - previousTime1) >= uartUpdate:
        if uart.any():
            read = uart.readline();
            query = [];
            resultString = "";
            for i in range(0, len(read), 2):
                query.append(read[i:i+2])
            for i in range(0, len(query)):
                if query[i] == bytes("ox", "ascii"):
                    resultString += f"#ox:{ox}";
                elif query[i] == bytes("oy", "ascii"):
                    resultString += f"#oy:{distance(oy)}";
                elif query[i] == bytes("bx", "ascii"):
                    resultString += f"#bx:{bx}";
                elif query[i] == bytes("by", "ascii"):
                    resultString += f"#by:{distance(by)}";
                elif query[i] == bytes("yx", "ascii"):
                    resultString += f"#yx:{yx}";
                elif query[i] == bytes("yy", "ascii"):
                    resultString += f"#yy:{distance(yy)}";
                elif query[i] == bytes("yi", "ascii"):
                    resultString += f"#yi:{int(yIntercept)}";
                elif query[i] == bytes("bi", "ascii"):
                    resultString += f"#bi:{int(bIntercept)}";
            uart.write(f"{resultString}\n");
        previousTime1 = currentTime;
    if (currentTime - previousTime2) >= frameUpdate:
        img = sensor.snapshot().rotation_corr(x_rotation = 180)

        oBlobs = img.find_blobs(threshold_orange, pixels_threshold=3, area_threshold=1, merge=False)
        if oBlobs:
            oBlob = max(oBlobs, key=lambda b: b.area())
            if (oBlob.roundness() > .1):
                if (SHOW_IMAGE_DECORATIONS):    img.draw_circle(oBlob.enclosing_circle(), color=(img.get_pixel(oBlob.cx(), oBlob.cy())), thickness=3)
                ox = oBlob.cx()
                oy = oBlob.cy()
                print(f"distance: {distance(oy)} -- raw: {oy}")
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
        previousTime2 = currentTime;



