let url = 'https://io.adafruit.com/api/v2/cmforever426/feeds/projectone';
let counter = 0;

let clicked = false;

//let font;

let data;

let myButton;

let spagetti1, spagetti2, spagetti3, spagetti4;
let bass;
let pasta;

let fft;
let w;
let amp;
let volHistory = [];
let button;
let start;
let resizeR = 0;
let resizeG = 0;
let resizeB = 0;
let bgcR = 0;
let bgcG = 0;
let bgcB = 0;
let change = 0;

//while our audio is playing,
//this will be set to true
let sampleIsLooping = false;

let outData = '0';

function preload() {
    soundFormats('wav');
    spagetti1 = loadSound('assets/spagetti1');
    spagetti2 = loadSound('assets/spagetti2');
    spagetti3 = loadSound('assets/spagetti3');
    spagetti4 = loadSound('assets/spagetti4');
    bass = loadSound('assets/bass');
    pasta = loadSound('assets/pasta');
    //font = loadFont('assets/papyrus.ttc');
}

function setup() {
    createCanvas(600, 600);
    amp = new p5.Amplitude();
}

function draw() {
    
    let volume1 = true;
    let volume2 = false;
    let volume3 = false;
    let volume4 = false;
    
    
    spagetti1.amp(0.8);
    spagetti2.amp(0.0);
    spagetti3.amp(0.0);
    spagetti4.amp(0.0);
    bass.amp(0.8);
    pasta.amp(0.8);    
    
    background(0);
    noStroke();
    textAlign(CENTER);
    fill(255);
    textSize(60);
    textFont('papyrus');
    start = text('CLICK HERE!', width / 2, height / 2);
    
    if (counter % 80 == 0) {
        getData();
    }
    
    counter++;
    
    if (sampleIsLooping) {
        background(bgcR, bgcG, bgcB);
        let vol = amp.getLevel();
        fill(resizeR, resizeG, 255);
        ellipse(300, 300, 600, vol * 600);
        
        if (data == '0' && volume1 == true) {
                volume1 = false;
                volume2 = true;
                spagetti1.amp(0.0);
                spagetti3.amp(0.8);
                spagetti4.amp(0.0);
                console.log("type b");
            }
    }
    
}

function mouseClicked() {
    if (!sampleIsLooping) {
        spagetti1.loop();
        spagetti2.loop();
        spagetti3.loop();
        spagetti4.loop();
        bass.loop();
        pasta.loop();
        sampleIsLooping = true;
    }
}

function getData() {
    httpGet(url, 'json', function(response) {
        console.log(response);
        data = response.last_value;
        resizeR = map(data, 1, 0, 255, 50);
        resizeG = map(data, 1, 0, 255, 180);
        bgcR = map(data, 1, 0, 0, 5);
        bgcG = map(data, 1, 0, 0, 10);
        bgcB = map(data, 1, 0, 0, 50);
        console.log(data);
    });
}
