# Teufenzeiger
In this project, I wanted to find out the tiniest of components I am able to populate by hand. So everything is a bit smaller.
## Architecture
Using an [ESP32-S2](https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf) together with three or four [NVT2008BQ](https://www.nxp.com/docs/en/data-sheet/NVT2008_NVT2010.pdf) level shifters. These level shifters can easily be used because each IO on the Teufenzeigers is pulled up to 5 V using 10 k. Because the Teufzeiger requires 23 pins to be used, the project uses 3 8-channel level shifters.A

A supply voltage of 3.3 V can be generated from the 5 V provided by the Teufenzeiger. The power supply on the Teufenzeiger can provide up to 1 A, this is sufficient for the ESP32. Since the datasheet of the ESP32 indicates a supply current of up to 500 mA, the LDO has to burn `(5V - 3.3V) * 0.5A = 0.85W`. Because this much current is only drawn for very brief amounts of time, pretty much any LDO should be able to handle this. So the LDO I decided to use is the [MIC5528](https://ww1.microchip.com/downloads/en/DeviceDoc/MIC5528-High-Performance-500mA-LDO-in-Thin-and-Extra-Thin-DFN-Packages-DS20005982B.pdf). This device is not explicitly rated for a power dissipation of 0.85 W, but because this much heat is only generated for a brief amount of time, I assume this to be okay.

Most of the ESP32 designs use PCB trace antennas. For ease of use, I decided to use a SMD mounted WIFI antenna. The exact part is the [Molex 1461750001](https://www.deutsch.molex.com/molex/products/part-detail/antennas/1461750001). But using an SMD antenna is only half of the work. 


# Interfacing

## Writing to Display

1. LED_ADDR[0..2] => D[0..2]
2. nCE0[3..1]: D[3..5] = 5..7
3. PIN11 = 1, PIN1 = 0, D[7] = 1
4. Load_Addr = 1
5. Load_Addr = 0
5. nWR = 0
6. Set D[0..7]
7. nWR = 7. nWR = 1
