# Untitled - By: ikerc - jue. abr. 13 2023

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # Width = 320, Height = 240

clock = time.clock()

x, y = 160, 120

def lab_to_rgb(lab_L, lab_a, lab_b):
    # Convert LAB to XYZ
    lab_var_Y = (lab_L + 16) / 116
    lab_var_X = lab_a / 500 + lab_var_Y
    lab_var_Z = lab_var_Y - lab_b / 200

    if lab_var_Y ** 3 > 0.008856:
        lab_var_Y_cubed = lab_var_Y ** 3
    else:
        lab_var_Y_cubed = (lab_var_Y - 16 / 116) / 7.787

    if lab_var_X ** 3 > 0.008856:
        lab_var_X_cubed = lab_var_X ** 3
    else:
        lab_var_X_cubed = (lab_var_X - 16 / 116) / 7.787

    if lab_var_Z ** 3 > 0.008856:
        lab_var_Z_cubed = lab_var_Z ** 3
    else:
        lab_var_Z_cubed = (lab_var_Z - 16 / 116) / 7.787

    lab_X = 0.95047 * lab_var_X_cubed
    lab_Y = 1.00000 * lab_var_Y_cubed
    lab_Z = 1.08883 * lab_var_Z_cubed

    # Convert XYZ to RGB
    lab_R = 3.2406 * lab_X - 1.5372 * lab_Y - 0.4986 * lab_Z
    lab_G = -0.9689 * lab_X + 1.8758 * lab_Y + 0.0415 * lab_Z
    lab_B = 0.0557 * lab_X - 0.2040 * lab_Y + 1.0570 * lab_Z

    # Apply gamma correction
    if lab_R > 0.0031308:
        lab_R = 1.055 * (lab_R ** (1 / 2.4)) - 0.055
    else:
        lab_R = 12.92 * lab_R
    if lab_G > 0.0031308:
        lab_G = 1.055 * (lab_G ** (1 / 2.4)) - 0.055
    else:
        lab_G = 12.92 * lab_G
    if lab_B > 0.0031308:
        lab_B = 1.055 * (lab_B ** (1 / 2.4)) - 0.055
    else:
        lab_B = 12.92 * lab_B

    # Clamp RGB values to [0, 255]
    lab_R = min(max(0, int(lab_R * 255)), 255)
    lab_G = min(max(0, int(lab_G * 255)), 255)
    lab_B = min(max(0, int(lab_B * 255)), 255)

    return (lab_R, lab_G, lab_B)

def rgb2lab(rgb_r, rgb_g, rgb_b):
    # Convert RGB values to the range 0-1
    rgb_r, rgb_g, rgb_b = rgb_r/255.0, rgb_g/255.0, rgb_b/255.0

    # Convert RGB to XYZ
    xyz_x = 0.412453*rgb_r + 0.357580*rgb_g + 0.180423*rgb_b
    xyz_y = 0.212671*rgb_r + 0.715160*rgb_g + 0.072169*rgb_b
    xyz_z = 0.019334*rgb_r + 0.119193*rgb_g + 0.950227*rgb_b

    # Convert XYZ to LAB
    xyz_x /= 0.950456
    xyz_y /= 1.0
    xyz_z /= 1.088754

    lab_fx =  xyz_x**(1/3) if xyz_x > 0.008856 else (7.787*xyz_x + 16/116)
    lab_fy =  xyz_y**(1/3) if xyz_y > 0.008856 else (7.787*xyz_y + 16/116)
    lab_fz =  xyz_z**(1/3) if xyz_z > 0.008856 else (7.787*xyz_z + 16/116)

    lab_L = 116*lab_fy - 16
    lab_a = 500*(lab_fx - lab_fy)
    lab_b = 200*(lab_fy - lab_fz)


    return int(lab_L), int(lab_a), int(lab_b)

nanos_start = time.time_ns();
millis = 0
last_millis = 0;
index = 0

thresholds = [];
rgbs = []
last_rgb = (0, 0, 0);

firstLoop = True
printOnce = True
while(True):
    clock.tick()
    millis = (time.time_ns() - nanos_start) // 1000000

    if firstLoop:
        img = sensor.snapshot();
        pixel_value = img.get_pixel(x, y) # Center of the image

        img.draw_circle(x, y, 25, color=(pixel_value), thickness=3, fill=False)
        img.draw_circle(x, y, 1, color=(0, 0, 0), thickness=3, fill=True)

        img.draw_rectangle(0, 210, 320, 240, fill=True)
        img.draw_string(0, 210, "Starting in: {}".format(5000 - millis), color=(0, 0, 0))
        if (5000 - millis) <= 0:
            firstLoop = False
            nanos_start = time.time_ns();
            millis = 0;
            last_millis = 0;
            index = 0;
            rgbs = [];
            thresholds = [];
            last_rgb = (0, 0, 0);
    else:
        if index >= 20:
            img = sensor.snapshot();

            min_l = min(thresholds, key=lambda x: x[0])[0]
            max_l = max(thresholds, key=lambda x: x[0])[0]

            min_a = min(thresholds, key=lambda x: x[1])[1]
            max_a = max(thresholds, key=lambda x: x[1])[1]

            min_b = min(thresholds, key=lambda x: x[2])[2]
            max_b = max(thresholds, key=lambda x: x[2])[2]

            img.draw_rectangle(0, 0, 320, 240, fill=True)
            img.draw_string(0, 20, "Thresholds in Serial Terminal", color=(0, 0, 0))
            img.draw_string(0, 40, "[({}, {}, {}, {}, {}, {})]".format(min_l, max_l, min_a, max_a, min_b, max_b), color=(0, 0, 0))
            if printOnce:
                print("Final threshold: [({}, {}, {}, {}, {}, {})]".format(min_l, max_l, min_a, max_a, min_b, max_b))
                printOnce = False
        else:
            img = sensor.snapshot()
            if (millis // 1000 > index):
                thresholds = [rgb2lab(last_rgb[0], last_rgb[1], last_rgb[2])] + thresholds
                rgbs = [last_rgb] + rgbs;
                print(thresholds)
                index += 1


            pixel_value = img.get_pixel(x, y) # Center of the image

            rgb_r = pixel_value[0]
            rgb_g = pixel_value[1]
            rgb_b = pixel_value[2]

            last_rgb = (rgb_r, rgb_g, rgb_b)
            lab_l = rgb2lab(rgb_r, rgb_g, rgb_b)[0]
            lab_a = rgb2lab(rgb_r, rgb_g, rgb_b)[1]
            lab_b = rgb2lab(rgb_r, rgb_g, rgb_b)[2]

            img.draw_circle(x, y, 25, color=(pixel_value), thickness=3, fill=False)
            img.draw_circle(x, y, 1, color=(0, 0, 0), thickness=3, fill=True)

            img.draw_rectangle(0, 0, 320, 30, fill=True)
            img.draw_rectangle(0, 210, 320, 240, fill=True)
            img.draw_string(10, 10, "LAB: {}, {}, {}".format(lab_l, lab_a, lab_b), color=(0, 0, 0))
            img.draw_string(160, 10, "RGB: {}, {}, {}".format(rgb_r, rgb_g, rgb_b), color=(0, 0, 0))
            img.draw_string(0, 210, "Next frame capture in {}".format(1000 + ((index * 1000) - millis)), color=(0, 0, 0))
            img.draw_string(0, 225, "Frames captured {}".format(index), color=(0, 0, 0))

            img.draw_rectangle(290, 60, 320, 90, thickness=3, fill=False)
            img.draw_rectangle(290, 90, 320, 120, thickness=3, fill=False)
            img.draw_rectangle(290, 120, 320, 150, thickness=3, fill=False)
            img.draw_rectangle(290, 150, 320, 180, thickness=3, fill=False)
            img.draw_rectangle(290, 180, 320, 210, thickness=3, fill=False)

            if index > 0:    img.draw_rectangle(290, 60, 320, 90, color=rgbs[0], thickness=2, fill=True);
            if index > 1:    img.draw_rectangle(290, 90, 320, 120, color=rgbs[1], thickness=2, fill=True);
            if index > 2:    img.draw_rectangle(290, 120, 320, 150, color=rgbs[2], thickness=2, fill=True);
            if index > 3:    img.draw_rectangle(290, 150, 320, 180, color=rgbs[3], thickness=2, fill=True);
            if index > 4:    img.draw_rectangle(290, 180, 320, 210, color=rgbs[4], thickness=2, fill=True);

            last_millis = millis

        # index + 1000 - millis
