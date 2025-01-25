# Wokwi Frequency Counter Custom Chip Demo

[This simulation](https://wokwi.com/projects/406335146945275905) provides an example of a hardware frequency counter
based on a Wokwi custom chip.  

It functions by using the [https://docs.wokwi.com/chips-api/time](https://docs.wokwi.com/chips-api/time) API 
to measure a 1 second period and a [https://docs.wokwi.com/chips-api/gpio](https://docs.wokwi.com/chips-api/gpio) to watch for pin changes and count falling ones.

It was built from the inverter custom chip from:
* https://github.com/wokwi/inverter-chip
* https://wokwi.com/projects/327458636089524820

On the sketch side, you can change the frequency of the toggling bu changing the schem and
the period in milliseconds or microsecond.

# Results:
With Period=0 delays:
| scheme | frequency |
|---|---|
|0 |   76470/sec |
|1 |  147277/sec |
|2 | 2272286/sec |
|3 | 1590610/sec |
|4 | 2651015/sec |
|5 | 7213229/sec |
|6 | 8000000/sec | 

# See also

* https://wokwi.com/projects/369677772506408961 -- Urish: PWM freq and duty analyzer
* https://wokwi.com/projects/390819301187622913 -- An implementation of Dlloyd's Scope (also measures frequencies)
* https://discord.com/channels/787627282663211009/954892209486966825/1273827006999695472 thread on Discord

