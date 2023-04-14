# Untitled - By: ikerc - jue. abr. 13 2023

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

clock = time.clock()

orange = [(76, 90, 2, 23, 33, 61)]

biggest = False # if Biggest, only the biggest blob will be encircled

while(True):
    clock.tick()
    img = sensor.snapshot()

    oBlobs = img.find_blobs(orange, pixels_threshold=150, area_threshold=30, merge=False)
    if oBlobs:
        if biggest:
            oBlob = max(oBlobs, key=lambda b: b.area())
            img.draw_circle(oBlob.enclosing_circle(), color=(0, 0, 0))
        else:
            for oBlob in oBlobs:
                img.draw_circle(oBlob.enclosing_circle(), color=(0, 0, 0))

    print(clock.fps())
