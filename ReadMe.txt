one sentence description of the project
why I wrote it
how to assemble it
schematics
parts


------
clean-tune is a motion-sensor enabled, 20 second timer used to wash your hand. While the timer is counting down, it plays a melody. This projects is inspired by [link]

## parts
1x arduino auno
1x ultrasound sensor
1x pizzo buzzer
1x 7 segment display
xx jumpter wire

## design




Because I want a more accurate timer, I used millis() instead of delay(). However, after benchmarking it, I noticed that the time is off about 2 ms per second

todo:
	- learn about arduino RTC
	- learn about I2C
