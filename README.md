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
![S__41271298](https://user-images.githubusercontent.com/79573591/121385939-28a59500-c97c-11eb-8878-33f44277d4a5.jpg)
![S__41271300](https://user-images.githubusercontent.com/79573591/121385960-2e9b7600-c97c-11eb-9fd1-31e1a415f818.jpg)
![S__41271301](https://user-images.githubusercontent.com/79573591/121385983-322efd00-c97c-11eb-8e6a-944b59672cca.jpg)


### 2. Line Following<br>
- Put the car on the straight line and the car will follow the line.<br>

Some result : <br>
![S__41271302](https://user-images.githubusercontent.com/79573591/121386316-7a4e1f80-c97c-11eb-8ec9-6ca0ccdd208a.jpg)
![S__41271304](https://user-images.githubusercontent.com/79573591/121386334-7cb07980-c97c-11eb-8c34-d73edf8f61a5.jpg)



### 3. Apriltag navigation<br>
- Let the car face apriltag by some dergree or directly, then the car will follow apriltag.<br>

Some result : <br>
![S__41271306](https://user-images.githubusercontent.com/79573591/121387070-fa748500-c97c-11eb-94eb-fb9d30eae4c2.jpg)
![S__41271308](https://user-images.githubusercontent.com/79573591/121387086-fe080c00-c97c-11eb-9dc9-40a24062f3af.jpg)
![S__41271309](https://user-images.githubusercontent.com/79573591/121387098-006a6600-c97d-11eb-8dbe-11e3786dae5a.jpg)
![S__41271305](https://user-images.githubusercontent.com/79573591/121387112-03fded00-c97d-11eb-88f4-de3109048de2.jpg)

