# Untitled - By: ikerc - jue. abr. 13 2023

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(True)

clock = time.clock()


# Disable automatic settings
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

# Set exposure, gain, brightness, contrast, and white balance values
sensor.set_auto_exposure(False, exposure_us=2250) # adjust as needed
sensor.set_gainceiling(128) # adjust as needed
sensor.set_brightness(3) # adjust as needed
sensor.set_contrast(-2) # adjust as needed
sensor.set_saturation(-1) # adjust as needed


i = 0;
threshold = [
[(39, 58, 4, 43, 22, 54)]
]

biggest = True # if Biggest, only the biggest blob will be encircled
circle = False # if Circle, image decorations will be a circle, if not, a rectangle
nanos_start = time.time_ns();
millis = 0
last_millis = 0;

while(True):
    clock.tick()
    img = sensor.snapshot().rotation_corr(x_rotation = 180)
    millis = (time.time_ns() - nanos_start) // 1000000

    oBlobs = img.find_blobs(threshold[i], pixels_threshold=25, area_threshold=10, merge=True)
    if oBlobs:
        if biggest:
            oBlob = max(oBlobs, key=lambda b: b.area())
            if (circle):
                img.draw_circle(oBlob.enclosing_circle(), color=(img.get_pixel(oBlob.cx(), oBlob.cy())), thickness=3)
            else:
                img.draw_rectangle(oBlob.rect(), color = (img.get_pixel(oBlob.cx(), oBlob.cy())), thickness=3)
        else:
            for oBlob in oBlobs:
                img.draw_circle(oBlob.enclosing_circle(), color=(0, 0, 0), thickness=3)

    print(threshold[i])
