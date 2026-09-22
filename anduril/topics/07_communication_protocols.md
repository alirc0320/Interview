---
topic: Serial communication protocols — UART, SPI, I2C, CAN
date_written: <YYYY-MM-DD>
---

# Topic: Communication Protocols (UART, SPI, I2C, CAN)

Maps to GFG Q31-35 (sync vs. async serial, SPI, I2C, CAN, UART). Mostly
conceptual — the coding-shaped version of this topic is
`problems/uart_frame_parser` (parsing a framed byte stream, which is what
you'd actually write once bytes have already come off a UART peripheral).

## 1. Concept explanation — synchronous vs. asynchronous serial
Q: Explain the core difference between synchronous and asynchronous serial communication: does a dedicated clock signal get transmitted alongside the data, or do both ends have to agree on timing some other way? For UART specifically (asynchronous), explain how the receiver knows where each byte starts and ends without a shared clock line — what do the start bit, stop bit(s), and baud rate actually do for you, and what happens if the two ends' baud rates don't match closely enough?
A:

## 2. Concept explanation — SPI
Q: Describe the SPI wiring: name the (typically 4) signal lines (MOSI/SDO, MISO/SDI, SCLK, and CS/SS) and what each one does, including why CS is usually a separate line per slave device rather than shared. Explain that SPI is full-duplex (data can be shifted in and out simultaneously on the same clock edges) and give one concrete advantage of SPI over I2C (speed) and one concrete disadvantage (more pins needed per additional device, no built-in addressing/acknowledgment).
A:

## 3. Concept explanation — I2C
Q: Describe the I2C wiring (just two lines: SDA and SCL, both open-drain with pull-up resistors) and explain what "open-drain with a shared pull-up" means for how multiple devices can coexist on the same two wires without electrical contention. Explain 7-bit addressing (how the master selects which of possibly many slaves on the bus it's talking to) and give one concrete advantage of I2C over SPI (fewer pins, multi-master/multi-slave on 2 wires) and one disadvantage (slower, more protocol overhead per transaction, addressing conflicts to manage).
A:

## 4. Concept explanation — CAN
Q: Explain what makes CAN (Controller Area Network) different in character from UART/SPI/I2C: it's a multi-master broadcast bus (every node sees every message) built around message IDs rather than point-to-point addressing, with built-in arbitration (lower numeric ID wins access to the bus when two nodes transmit simultaneously) and built-in error detection. Give a concrete reason CAN specifically is common in vehicles/aircraft/robotics (noise immunity over long harness runs, multi-node broadcast without a central master, deterministic priority-based arbitration) versus why you would NOT use it for something like a simple two-chip sensor-to-MCU link where SPI/I2C would be simpler and cheaper.
A:

## 5. Concept explanation — choosing between them
Q: You're given a short list of hardware requirements and have to pick a protocol for each — walk through your reasoning, not just the answer: (a) a single temperature sensor a few centimeters from the MCU on a board with very limited pin count, (b) a high-speed ADC/DAC that needs to stream samples continuously with minimal per-transaction overhead, (c) a debug/telemetry link to a ground-station laptop over a point-to-point cable, (d) multiple independent subsystem controllers on a shared harness across a vehicle, where any node might need to broadcast a status/fault message that others should react to.
A:

## 6. Debugging — a UART framing bug
Q: A ground station and a flight computer disagree about incoming telemetry: individual bytes look corrupted or the whole stream is off by a byte here and there, but only intermittently and only under certain conditions. List at least three distinct root causes worth checking for a UART link specifically (mismatched baud rate — including how close is "close enough," given both ends independently divide down a clock; mismatched parity/stop-bit/data-bit configuration; a receiver that can't keep up with the incoming byte rate and drops bytes with no flow control to signal backpressure), and for each, say what symptom you'd expect to see that would help you tell them apart.
A:

---

## Corrections
Reopen the material. Fix anything wrong above (don't erase the original
wrong answer — strike it through or note what was wrong, so the mistake
stays visible on review).

-

## Missed on first pass?
- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
