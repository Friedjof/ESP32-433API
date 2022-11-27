// this function send the transmit-api-call to the ESP32
function transmit(){
    // if the popup is already open and the user clicks the button again, do nothing
    if(document.getElementById("sending").style.display === "block" || document.getElementById("sending").style.opacity !== "1"){
        return;
    }

    // Show the popup
    showPopup("sending");
    // Animate the loading bar
    animateLoadingBar("sending", 0.1, 100, 25);

    // get params from the transmit section
    const api_key = document.getElementById("api-key").value;
    const transmit_msg = document.getElementById("transmit-msg").value;
    const transmit_msg_format = document.getElementById("transmit-msg-format").value;

    // Log the params
    console.log("Sending transmit request to ESP32 with the following params:");
    console.log("API Key: " + api_key);
    console.log("Transmit Message: " + transmit_msg);
    console.log("Transmit Message Format: " + transmit_msg_format);

    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "http://" + getDomain() + "/transmit?" + "key=" + api_key + "&msg=" + transmit_msg + "&type=" + transmit_msg_format, true)
    xhttp.send();
}

// this function will return the current domain of the browser address bar
// it will be used at every other function to send the data to the ESP32
function getDomain(){
    return window.location.hostname;
}

function showPopup(popup_id) {
    document.getElementById(popup_id).style.display = "block";
}

function closePopup(popup_id) {
    document.getElementById(popup_id).style.display = "none";
}

// fade the popup out
function fadeOutPopup(popup_id){
    const elem = document.getElementById(popup_id);
    let opacity = 1;
    const id = setInterval(frame, 50);

    function frame() {
        if (opacity <= 0) {
            clearInterval(id);
            opacity = 1;
            // hide the popup
            closePopup(popup_id);
            elem.style.opacity = opacity;
        } else {
            opacity -= 0.1;
            elem.style.opacity = opacity;
        }
    }
}

// Animate the loading bar
function animateLoadingBar(popup_id, step_size = 1, max_step = 100, timeout = 50){
    const elem = document.getElementById(popup_id);
    console.log(elem);
    const loading_bar = elem.getElementsByClassName("loading-bar")[0];
    let width = step_size;
    const id = setInterval(frame, timeout);

    function frame() {
        if (width >= max_step) {
            clearInterval(id);
            width = step_size;
            // hide the popup
            fadeOutPopup(popup_id);
        } else {
            width++;
            loading_bar.style.width = width + '%';
        }
    }
}