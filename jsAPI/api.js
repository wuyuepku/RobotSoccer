var robot = (() => {
    var _ip = '192.168.4.1',
    return {
        leftSpeed: 0,
        rightSpeed: 0,
        setLeftSpeed: (speed) => {
            var img = new Image();
            img.src = `http://${_ip}/wheel/left?speed=${speed}`;
            this.leftSpeed = speed;
        },
        setRightSpeed: (speed) => {
            var img = new Image();
            img.src = `http://${_ip}/wheel/right?speed=${speed}`;
            this.rightSpeed = speed;
        },
        setIP: (ip) => {
            _ip = ip;
        }
    }
})();



