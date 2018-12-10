var robot = (() => {
    var _ip = '192.168.4.1';
    var _time = 0;
    return {
        leftSpeed: 0,
        rightSpeed: 0,
        setLeftSpeed: (speed) => {
            speed = parseInt(speed) + 100;
            if(!speed) {
                speed = 100;
            }
            var img = new Image();
            img.src = `http://${_ip}/wheel/left?speed=${speed}&time=${_time}`;
            this.leftSpeed = speed - 100;
            _time++;
        },
        setRightSpeed: (speed) => {
            speed = parseInt(speed) + 100;
            if(!speed) {
                speed = 100;
            }
            var img = new Image();
            img.src = `http://${_ip}/wheel/right?speed=${speed}&time=${_time}`;
            this.rightSpeed = speed - 100;
            _time++;
        },
        setIP: (ip) => {
            _ip = ip;
        }
    }
})();