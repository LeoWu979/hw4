# hw4

### Setup<br>
- xbee : TX<->D10, RX<->D9<br>
- openMV : P4<->D0, P5<->D1<br>
- Ping : D12<br>
- encoder : D11<br>
- continuous servo : D5、D6<br>

### 1. Parking<br>
- use "sudo python3 xbee.py" to compile it.<br>
- use "/park/run d2 d1 k" to sen RPC command, where k is the direction(1 or 2).<br>
- Then remote xbee will receive the position parameter and start to park.<br>

Some result : <br>

### 2. Line Following<br>
- Put the car on the straight line and the car will follow the line.<br>
