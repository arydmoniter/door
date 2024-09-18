nfcrfid_sucsese = 0
door_openclosed = 0

def on_forever():
    global nfcrfid_sucsese
    if nfcrfid_sucsese == 1:
        pins.digital_write_pin(DigitalPin.P0, 0)
        basic.pause(5000)
        nfcrfid_sucsese = 0
    if input.logo_is_pressed():
        nfcrfid_sucsese = 1
    elif nfcrfid_sucsese == 0:
        if door_openclosed == 1:
            pins.digital_write_pin(DigitalPin.P0, 1)
    if nfcrfid_sucsese == 0:
        if door_openclosed == 0:
            pins.digital_write_pin(DigitalPin.P0, 0)
basic.forever(on_forever)

def on_forever2():
    global door_openclosed
    if pins.digital_read_pin(DigitalPin.P1) == 0:
        door_openclosed = 0
    if pins.digital_read_pin(DigitalPin.P1) == 1:
        door_openclosed = 1
basic.forever(on_forever2)
