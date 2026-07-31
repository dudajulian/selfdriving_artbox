// =======================================================
// Configuration
// =======================================================

const LEFT_IP = "192.168.8.50";
const RIGHT_IP = "192.168.8.51";

const MAX_PWM = 1000;
const MIN_PWM = 350;



const MOTOR_UPDATE_PERIOD = 10;      // 50 Hz
const ENCODER_PERIOD = 200;          // 5 Hz
const HEARTBEAT_PERIOD = 300;        // ms
const ACCELERATION_STEP = 100;        // PWM per update

// =======================================================
// Desired Motion
// =======================================================

let desiredForward = 0;
let desiredTurn = 0;

// =======================================================
// Current Motion
// =======================================================

let currentForward = 0;
let currentTurn = 0;

// =======================================================
// Wheel Speeds
// =======================================================

let leftPWM = 0;
let rightPWM = 0;

let lastLeftPWM = null;
let lastRightPWM = null;

let lastHeartbeat = 0;

// =======================================================
// Communication
// =======================================================

function sendMotor(ip, speed)
{
    fetch(`http://${ip}/motor?speed=${speed}`)
        .catch(error => console.error(error));
}

function readEncoder(ip, elementID)
{
    fetch(`http://${ip}/encoder`)
        .then(response => response.json())
        .then(data =>
        {
            document.getElementById(elementID).textContent = data.count;
        })
        .catch(error => console.error(error));
}

// =======================================================
// Helper
// =======================================================

function approach(current, target, step)
{
    if(current < target)
        return Math.min(current + step, target);

    if(current > target)
        return Math.max(current - step, target);

    return current;
}

// =======================================================
// Motion Controller
// =======================================================

function updateMotion()
{
    currentForward = approach(currentForward,
                              desiredForward,
                              ACCELERATION_STEP);

    currentTurn = approach(currentTurn,
                           desiredTurn,
                           ACCELERATION_STEP);
}

// =======================================================
// Differential Drive
// =======================================================
function mapPWM(value)
{
    if (value === 0)
        return 0;

    const sign = Math.sign(value);

    value = Math.abs(value);

    // value is between 0 and MAX_PWM
    value = MIN_PWM + (value / MAX_PWM) * (MAX_PWM - MIN_PWM);

    return Math.round(sign * value);
}
function calculateWheelSpeeds()
{
    let left = currentForward + currentTurn;
    let right = currentForward - currentTurn;

    left = Math.max(-MAX_PWM, Math.min(MAX_PWM, left));
    right = Math.max(-MAX_PWM, Math.min(MAX_PWM, right));

    leftPWM = mapPWM(left);
    rightPWM = mapPWM(right);
}

// =======================================================
// Communication
// =======================================================

function sendMotorCommands()
{
    const now = Date.now();

    const heartbeat =
        (now - lastHeartbeat) >= HEARTBEAT_PERIOD;

    if(leftPWM !== lastLeftPWM || heartbeat)
    {
        sendMotor(LEFT_IP, leftPWM);
        lastLeftPWM = leftPWM;
    }

    if(rightPWM !== lastRightPWM || heartbeat)
    {
        sendMotor(RIGHT_IP, rightPWM);
        lastRightPWM = rightPWM;
    }

    if(heartbeat)
        lastHeartbeat = now;
}

// =======================================================
// Display
// =======================================================

function updateDisplay()
{
    document.getElementById("leftSpeed").textContent = leftPWM;
    document.getElementById("rightSpeed").textContent = rightPWM;
}

// =======================================================
// Main Motor Loop
// =======================================================

setInterval(function()
{
    updateMotion();

    calculateWheelSpeeds();

    sendMotorCommands();

    updateDisplay();

}, MOTOR_UPDATE_PERIOD);

// =======================================================
// Encoder Loop
// =======================================================

setInterval(function()
{
    readEncoder(LEFT_IP, "leftEncoder");
    readEncoder(RIGHT_IP, "rightEncoder");

}, ENCODER_PERIOD);

// =======================================================
// Keyboard
// =======================================================

document.addEventListener("keydown", function(event)
{
    switch(event.key.toLowerCase())
    {
        case "w":
            desiredForward = MAX_PWM;
            break;

        case "s":
            desiredForward = -MAX_PWM;
            break;

        case "a":
            //desiredTurn = -MAX_PWM / 2 ;
            desiredTurn = -800 ;
            break;

        case "d":
            //desiredTurn = MAX_PWM / 2 ;
            desiredTurn = 800 ;
            break;
    }
});

document.addEventListener("keyup", function(event)
{
    switch(event.key.toLowerCase())
    {
        case "w":
        case "s":
            desiredForward = 0;
            break;

        case "a":
        case "d":
            desiredTurn = 0;
            break;
    }
});