import sensor, image, time, pyb
enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
#sensor.set_windowing((10,0,140,50))
sensor.skip_frames(time = 2000)
clock = time.clock()
threshold_val = (0,45)

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.

while(True):
    clock.tick()
    img = sensor.snapshot().binary([threshold_val])
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

   # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
   # merging is done. At 1, any line 1 pixel away from another is merged... and so
   # on as you increase this value. You may wish to merge lines as line segment
   # detection produces a lot of line segment results.

   # `max_theta_diff` controls the maximum amount of rotation difference between
   # any two lines about to be merged. The default setting allows for 15 degrees.
    l = img.get_regression([(255,255)], roi = (10,0,140,50), robust = True)
    if l and l.magnitude() > 10 :
        img.draw_line(l.line(), color = 127)
        print_args = (l.x1(), l.y1(), l.x2(), l.y2())
        uart.write(("A%03d %03d %03d %03dE" % print_args).encode())
        print(l.line())
    else :
        uart.write(("BE").encode())

    time.sleep(0.2)
