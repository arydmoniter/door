let nfcrfid_sucsese = 0
let door_openclosed = 0
basic.forever(function on_forever() {
    
    if (nfcrfid_sucsese == 1) {
        pins.digitalWritePin(DigitalPin.P0, 0)
        basic.pause(5000)
        nfcrfid_sucsese = 0
    }
    
    if (input.logoIsPressed()) {
        nfcrfid_sucsese = 1
    } else if (nfcrfid_sucsese == 0) {
        if (door_openclosed == 1) {
            pins.digitalWritePin(DigitalPin.P0, 1)
        }
        
    }
    
    if (nfcrfid_sucsese == 0) {
        if (door_openclosed == 0) {
            pins.digitalWritePin(DigitalPin.P0, 0)
        }
        
    }
    
})
basic.forever(function on_forever2() {
    
    if (pins.digitalReadPin(DigitalPin.P1) == 0) {
        door_openclosed = 0
    }
    
    if (pins.digitalReadPin(DigitalPin.P1) == 1) {
        door_openclosed = 1
    }
    
})
